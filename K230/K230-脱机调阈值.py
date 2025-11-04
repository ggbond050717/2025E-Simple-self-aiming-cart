import time, os, sys, gc
import math
import cv_lite  # 导入cv_lite扩展模块
import ulab.numpy as np  # 导入numpy库
from media.sensor import *
from media.display import *
from media.media import *
from machine import UART, FPIOA, TOUCH, Pin

# --------------------------- 硬件初始化 ---------------------------
# 串口初始化
fpioa = FPIOA()
fpioa.set_function(11, FPIOA.UART2_TXD)
fpioa.set_function(12, FPIOA.UART2_RXD)
uart = UART(UART.UART2, 115200)  # 设置串口号2和波特率

# 按键初始化
fpioa.set_function(21, FPIOA.GPIO21)  # 按键引脚
KEY = Pin(21, Pin.IN, Pin.PULL_UP)  # 按键对象

# 屏幕分辨率设置
lcd_width = 800
lcd_height = 480

# 触摸初始化
touch = TOUCH(0)
adjust_threshold_mode = False

# 摄像头初始化
sensor = Sensor(id=2,width=1280, height=960)
sensor.reset()
sensor.set_framesize(width=320, height=240)  # 降低分辨率提高帧率
#sensor.set_vflip(True)
#sensor.set_hmirror(True)
sensor.set_pixformat(Sensor.RGB565)

# 显示初始化
Display.init(Display.ST7701, width=lcd_width, height=lcd_height, to_ide=True)
MediaManager.init()
sensor.run()

# --------------------------- 配置参数 ---------------------------
# 矩形检测核心参数
canny_thresh1      = 50
canny_thresh2      = 150
approx_epsilon     = 0.04
area_min_ratio     = 0.005
max_angle_cos      = 0.3
gaussian_blur_size = 3

# 筛选参数
MIN_AREA = 650
MAX_AREA = 45000
MIN_ASPECT_RATIO = 0.5
MAX_ASPECT_RATIO = 2.5

# 灰度阈值
L_min = 0
L_max = 100

# 虚拟坐标与圆形参数
BASE_RADIUS = 45
POINTS_PER_CIRCLE = 24
PURPLE_THRESHOLD = (20, 60, 15, 70, -70, -20)

# 基础矩形参数
RECT_WIDTH = 210
RECT_HEIGHT = 95

# --------------------------- 阈值配置 ---------------------------
# 默认阈值
current_thresholds = {
    "L_min": 0, "L_max": 100,
    "MIN_AREA": 650, "MAX_AREA": 45000
}

# 滑块配置 - 只保留4个滑块
SLIDER_CONFIG = {
    "L_min": {
        "track_x": (100, 500), "track_y": 100,
        "min_val": 0, "max_val": 100,
        "touch_area": (50, 500, 90, 30)
    },
    "L_max": {
        "track_x": (100, 500), "track_y": 180,
        "min_val": 0, "max_val": 100,
        "touch_area": (50, 500, 170, 30)
    },
    "MIN_AREA": {
        "track_x": (100, 500), "track_y": 260,
        "min_val": 100, "max_val": 40000,
        "touch_area": (50, 500, 250, 30)
    },
    "MAX_AREA": {
        "track_x": (100, 500), "track_y": 340,
        "min_val": 1000, "max_val": 100000,
        "touch_area": (50, 500, 330, 30)
    }
}

# 功能按钮区域
FUNCTION_BUTTONS = {
    "重置": (620, 100, 150, 50),
    "保存": (620, 180, 150, 50)
}

# 退出按钮区域
EXIT_BUTTON = {
    "x": 650, "y": 400,
    "w": 120, "h": 50,
    "text": "退出"
}

# 加载保存的阈值
def load_thresholds_from_file(filename='/sdcard/thresholds.txt'):
    try:
        try:
            with open(filename, 'r') as test_file:
                pass
        except OSError:
            print(f"未找到文件 {filename}，使用默认阈值")
            return None

        print(f"找到文件 {filename}，开始读取...")
        loaded_thresholds = None
        with open(filename, 'r') as f:
            content = f.read()
            lines = content.strip().split('\n')
            for line in lines:
                if line.strip():  # 跳过空行
                    values = [int(x) for x in line.strip().split(',')]
                    if len(values) == 4:
                        loaded_thresholds = {
                            "L_min": values[0], "L_max": values[1],
                            "MIN_AREA": values[2], "MAX_AREA": values[3]
                        }

        if loaded_thresholds:
            print(f"成功加载阈值: {loaded_thresholds}")
            return loaded_thresholds
        else:
            print(f"文件为空或格式错误")
            return None
    except Exception as e:
        print(f"加载阈值失败，错误详情: {e}")
        return None

# --------------------------- 存储阈值 ---------------------------
def save_thresholds_to_file(thresholds, filename='/sdcard/thresholds.txt'):
    try:
        with open(filename, 'w') as f:
            values = [
                thresholds["L_min"], thresholds["L_max"],
                thresholds["MIN_AREA"], thresholds["MAX_AREA"]
            ]
            f.write(','.join(map(str, values)) + '\n')
        print(f"阈值已保存到 {filename}")
        return True
    except Exception as e:
        print(f"保存阈值失败: {e}")
        return False

# 尝试加载阈值，如果加载失败则使用默认值
loaded_thresholds = load_thresholds_from_file()
if loaded_thresholds:
    current_thresholds = loaded_thresholds
    L_min = current_thresholds["L_min"]
    L_max = current_thresholds["L_max"]
    MIN_AREA = current_thresholds["MIN_AREA"]
    MAX_AREA = current_thresholds["MAX_AREA"]
    print(f"从文件加载的当前阈值: {current_thresholds}")
else:
    print(f"使用默认阈值: {current_thresholds}")

# --------------------------- 辅助函数 ---------------------------
def calculate_distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])** 2)

def calculate_center(points):
    if not points:
        return (0, 0)
    sum_x = sum(p[0] for p in points)
    sum_y = sum(p[1] for p in points)
    return (sum_x / len(points), sum_y / len(points))

def is_valid_rect(corners):
    edges = [calculate_distance(corners[i], corners[(i+1)%4]) for i in range(4)]

    ratio1 = edges[0] / max(edges[2], 0.1)
    ratio2 = edges[1] / max(edges[3], 0.1)
    valid_ratio = 0.5 < ratio1 < 1.5 and 0.5 < ratio2 < 1.5

    area = 0
    for i in range(4):
        x1, y1 = corners[i]
        x2, y2 = corners[(i+1) % 4]
        area += (x1 * y2 - x2 * y1)
    area = abs(area) / 2
    valid_area = MIN_AREA < area < MAX_AREA

    width = max(p[0] for p in corners) - min(p[0] for p in corners)
    height = max(p[1] for p in corners) - min(p[1] for p in corners)
    aspect_ratio = width / max(height, 0.1)
    valid_aspect = MIN_ASPECT_RATIO < aspect_ratio < MAX_ASPECT_RATIO

    return valid_ratio and valid_area and valid_aspect

def send_center_and_area(center, area):
    """只发送圆心坐标和矩形面积"""
    if not center:
        return
    cx, cy = map(int, center)
    msg = f"$$C,{cx},{cy},{area}##"
    uart.write(msg)
    print(f"发送数据: {msg}")

def get_perspective_matrix(src_pts, dst_pts):
    """计算透视变换矩阵"""
    A = []
    B = []
    for i in range(4):
        x, y = src_pts[i]
        u, v = dst_pts[i]
        A.append([x, y, 1, 0, 0, 0, -u*x, -u*y])
        A.append([0, 0, 0, x, y, 1, -v*x, -v*y])
        B.append(u)
        B.append(v)

    n = 8
    for i in range(n):
        max_row = i
        for j in range(i, len(A)):
            if abs(A[j][i]) > abs(A[max_row][i]):
                max_row = j
        A[i], A[max_row] = A[max_row], A[i]
        B[i], B[max_row] = B[max_row], B[i]

        pivot = A[i][i]
        if abs(pivot) < 1e-8:
            return None
        for j in range(i, n):
            A[i][j] /= pivot
        B[i] /= pivot

        for j in range(len(A)):
            if j != i and A[j][i] != 0:
                factor = A[j][i]
                for k in range(i, n):
                    A[j][k] -= factor * A[i][k]
                B[j] -= factor * B[i]

    return [
        [B[0], B[1], B[2]],
        [B[3], B[4], B[5]],
        [B[6], B[7], 1.0]
    ]

def transform_points(points, matrix):
    """应用透视变换"""
    transformed = []
    for (x, y) in points:
        x_hom = x * matrix[0][0] + y * matrix[0][1] + matrix[0][2]
        y_hom = x * matrix[1][0] + y * matrix[1][1] + matrix[1][2]
        w_hom = x * matrix[2][0] + y * matrix[2][1] + matrix[2][2]
        if abs(w_hom) > 1e-8:
            transformed.append((x_hom / w_hom, y_hom / w_hom))
    return transformed

def sort_corners(corners):
    """将矩形角点按左上、右上、右下、左下顺序排序"""
    center = calculate_center(corners)
    sorted_corners = sorted(corners, key=lambda p: math.atan2(p[1]-center[1], p[0]-center[0]))

    if len(sorted_corners) == 4:
        left_top = min(sorted_corners, key=lambda p: p[0]+p[1])
        index = sorted_corners.index(left_top)
        sorted_corners = sorted_corners[index:] + sorted_corners[:index]
    return sorted_corners

# 检查是否点击退出按键
def is_click_exit(x, y):
    return (EXIT_BUTTON["x"] <= x <= EXIT_BUTTON["x"] + EXIT_BUTTON["w"] and
            EXIT_BUTTON["y"] <= y <= EXIT_BUTTON["y"] + EXIT_BUTTON["h"])

# 检查是否点击功能按钮
def is_click_function(x, y, button_name):
    x1, y1, w1, h1 = FUNCTION_BUTTONS[button_name]
    return (x1 <= x <= x1 + w1 and y1 <= y <= y1 + h1)

# 绘制退出按键
def draw_exit_button(img):
    img.draw_rectangle(
        EXIT_BUTTON["x"], EXIT_BUTTON["y"],
        EXIT_BUTTON["w"], EXIT_BUTTON["h"],
        color=(255, 0, 0), thickness=0, fill=True
    )
    text_x = EXIT_BUTTON["x"] + EXIT_BUTTON["w"] // 2 - 25
    text_y = EXIT_BUTTON["y"] + EXIT_BUTTON["h"] // 2 - 15
    img.draw_string_advanced(
        text_x, text_y, 28,
        EXIT_BUTTON["text"],
        color=(255, 255, 255)
    )

# 绘制功能按钮
def draw_function_buttons(img):
    # 重置按钮
    x, y, w, h = FUNCTION_BUTTONS["重置"]
    img.draw_rectangle(x, y, w, h, (255, 0, 0), fill=True)
    img.draw_string_advanced(x + 30, y + 10, 24, "重置", (255, 255, 255))

    # 保存按钮
    x, y, w, h = FUNCTION_BUTTONS["保存"]
    img.draw_rectangle(x, y, w, h, (0, 255, 0), fill=True)
    img.draw_string_advanced(x + 30, y + 10, 24, "保存", (255, 255, 255))

# 绘制滑块
def draw_sliders(img):
    for slider in SLIDER_CONFIG:
        config = SLIDER_CONFIG[slider]
        track_left, track_right = config["track_x"]
        track_y = config["track_y"]
        track_width = track_right - track_left

        # 绘制轨道
        img.draw_rectangle(track_left, track_y, track_width, 10, (0, 0, 255), fill=True)

        # 计算滑块位置
        val_range = config["max_val"] - config["min_val"]
        if val_range == 0:
            slider_x = track_left
        else:
            ratio = (current_thresholds[slider] - config["min_val"]) / val_range
            slider_x = track_left + int(ratio * track_width)

        # 绘制滑块
        img.draw_rectangle(slider_x - 10, track_y - 10, 20, 30, (255, 0, 0), fill=True)

        # 绘制标签和值
        img.draw_string_advanced(30, track_y - 10, 22, slider, (255, 255, 255))
        img.draw_string_advanced(520, track_y - 10, 22, str(current_thresholds[slider]), (255, 255, 0))

# --------------------------- 阈值调整模式 ---------------------------
def lab_threshold_adjust():
    global current_thresholds, L_min, L_max, MIN_AREA, MAX_AREA
    print("阈值调整模式已启动...")

    adjust_threshold_mode = True
    initial_thresholds = current_thresholds.copy()
    current_slider = None
    last_update_time = 0
    update_delay = 50
    debounce_time = 300
    last_button_time = 0
    file_status_msg = ""
    status_timer = 0

    # 创建全屏图像用于显示滑块UI
    ui_width = 800
    ui_height = 480

    while adjust_threshold_mode:
        try:
            # 获取相机图像
            cam_img = sensor.snapshot()
            if cam_img is None:
                time.sleep_ms(100)
                continue

            # 创建一个全屏图像用于UI
            ui_img = image.Image(ui_width, ui_height, image.RGB565)
            ui_img.clear()

            # 应用灰度阈值处理
            gray_img = cam_img.to_grayscale()

            # 应用二值化处理 - 使用当前的L_min和L_max
            binary_img = gray_img.binary([(current_thresholds["L_min"], current_thresholds["L_max"])])

            # 转换回RGB以便显示
            binary_rgb = binary_img.to_rgb565()

            # 在UI图像中央显示二值化后的图像
            cam_x = (ui_width - binary_rgb.width()) // 2
            cam_y = (ui_height - binary_rgb.height()) // 2
            ui_img.draw_image(binary_rgb, cam_x, cam_y)

            # 标题
            ui_img.draw_string_advanced(300, 30, 30, "阈值调整", (255, 0, 0))

            # 绘制UI元素
            draw_sliders(ui_img)
            draw_function_buttons(ui_img)
            draw_exit_button(ui_img)

            # 显示文件操作状态
            current_time = time.ticks_ms()
            if file_status_msg and current_time - status_timer < 3000:
                ui_img.draw_string_advanced(200, 420, 24, file_status_msg, (255, 255, 0))
            else:
                file_status_msg = ""

            # 显示图像 - 全屏显示
            Display.show_image(ui_img)

            # 处理触摸事件
            points = touch.read()
            if points:
                tx, ty = points[0].x, points[0].y

                # 点击退出按钮
                if is_click_exit(tx, ty):
                    adjust_threshold_mode = False
                    break

                # 处理功能按钮
                if current_time - last_button_time > debounce_time:
                    if is_click_function(tx, ty, "重置"):
                        current_thresholds = initial_thresholds.copy()
                        last_button_time = current_time
                        continue

                    if is_click_function(tx, ty, "保存"):
                        # 保存阈值
                        L_min = current_thresholds["L_min"]
                        L_max = current_thresholds["L_max"]
                        MIN_AREA = current_thresholds["MIN_AREA"]
                        MAX_AREA = current_thresholds["MAX_AREA"]
                        success = save_thresholds_to_file(current_thresholds)
                        if success:
                            file_status_msg = "阈值已保存"
                        else:
                            file_status_msg = "保存阈值失败"
                        status_timer = current_time
                        last_button_time = current_time
                        continue

                # 处理滑块
                if current_slider is None:
                    for slider in SLIDER_CONFIG:
                        config = SLIDER_CONFIG[slider]
                        x_min, x_max, y_pos, height = config["touch_area"]
                        if (x_min <= tx <= x_max and y_pos <= ty <= y_pos + height):
                            current_slider = slider
                            break
                else:
                    if current_time - last_update_time > update_delay:
                        config = SLIDER_CONFIG[current_slider]
                        track_left, track_right = config["track_x"]
                        track_width = track_right - track_left

                        tx_clamped = max(track_left, min(tx, track_right))
                        val_range = config["max_val"] - config["min_val"]
                        if val_range > 0:
                            ratio = (tx_clamped - track_left) / track_width
                            new_val = config["min_val"] + int(ratio * val_range)
                            current_thresholds[current_slider] = new_val
                        last_update_time = current_time
            else:
                current_slider = None

            time.sleep_ms(16)

        except Exception as e:
            print(f"阈值调整错误: {e}")
            time.sleep_ms(100)

    print("阈值调整模式已退出")

# --------------------------- 主循环 ---------------------------
clock = time.clock()
image_shape = [sensor.height(), sensor.width()]
last_key_press = 0
KEY_DEBOUNCE_TIME = 300

while True:
    clock.tick()
    img = sensor.snapshot()

    # 2. 矩形检测
    gray_img = img.to_grayscale()
    img_np = gray_img.to_numpy_ref()

    rects = cv_lite.grayscale_find_rectangles_with_corners(
        image_shape,
        img_np,
        canny_thresh1,
        canny_thresh2,
        approx_epsilon,
        area_min_ratio,
        max_angle_cos,
        gaussian_blur_size
    )

    # 3. 筛选最小矩形
    min_area = float('inf')
    smallest_rect = None
    smallest_rect_corners = None

    for rect in rects:
        x, y, w, h = rect[0], rect[1], rect[2], rect[3]
        corners = [
            (rect[4], rect[5]),
            (rect[6], rect[7]),
            (rect[8], rect[9]),
            (rect[10], rect[11])
        ]

        if is_valid_rect(corners):
            area = w * h
            if area < min_area:
                min_area = area
                smallest_rect = (x, y, w, h)
                smallest_rect_corners = corners

    # 4. 处理最小矩形
    circle_center = None
    rect_area = 0

    if smallest_rect and smallest_rect_corners:
        x, y, w, h = smallest_rect
        corners = smallest_rect_corners
        sorted_corners = sort_corners(corners)

        # 绘制矩形
        for i in range(4):
            x1, y1 = sorted_corners[i]
            x2, y2 = sorted_corners[(i+1) % 4]
            img.draw_line(x1, y1, x2, y2, color=(255, 0, 0), thickness=2)
        for p in sorted_corners:
            img.draw_circle(p[0], p[1], 5, color=(0, 255, 0), thickness=2)

        # 计算矩形中心
        rect_center = calculate_center(sorted_corners)
        rect_center_int = (int(round(rect_center[0])), int(round(rect_center[1])))
        img.draw_circle(rect_center_int[0], rect_center_int[1], 4, color=(0, 255, 255), thickness=2)

        # 设置虚拟矩形
        virtual_rect = [
            (0, 0),
            (RECT_WIDTH, 0),
            (RECT_WIDTH, RECT_HEIGHT),
            (0, RECT_HEIGHT)
        ]

        # 设置虚拟中心
        virtual_center = (RECT_WIDTH / 2, RECT_HEIGHT / 2)

        # 计算透视变换矩阵并映射圆心
        matrix = get_perspective_matrix(virtual_rect, sorted_corners)
        if matrix:
            mapped_center = transform_points([virtual_center], matrix)
            if mapped_center:
                cx, cy = map(int, map(round, mapped_center[0]))
                circle_center = (cx, cy)
                img.draw_circle(cx, cy, 3, color=(0, 0, 255), thickness=1)
                img.draw_string(15, 15, f"({cx},{cy})", color=(255, 255, 255))

        # 计算矩形面积
        rect_area = w * h

    # 5. 发送圆心坐标和矩形面积
    if circle_center:
        send_center_and_area(circle_center, int(rect_area))

    # 6. 显示与性能统计
    fps = clock.fps()
    img.draw_string_advanced(0, 0, 10, f"FPS: {fps:.1f}", color=(255, 255, 255))
    img.draw_circle(160, 120, 2, color=(0, 255, 0), thickness=2)
    img.draw_string_advanced(0, 20, 10, "按下按键进入调整阈值模式", color=(0, 255, 0))
    img.draw_string_advanced(0, 40, 10, f"L:{L_min}-{L_max} AREA:{MIN_AREA}-{MAX_AREA}", color=(255, 255, 0))

    # 显示图像
    Display.show_image(img,
                      x=round((lcd_width-sensor.width())/2),
                      y=round((lcd_height-sensor.height())/2))

    # --------------------------- 检测按键触发阈值调整 ---------------------------
    current_time = time.ticks_ms()
    if KEY.value() == 0 and not adjust_threshold_mode:
        time.sleep_ms(10)  # 消除抖动
        if KEY.value() == 0:  # 确认按键被按下
            print('进入阈值调整模式')
            while not KEY.value():  # 等待按键释放
                pass
            adjust_threshold_mode = True
            lab_threshold_adjust()  # 调用阈值调整函数
            adjust_threshold_mode = False
            print('退出阈值调整模式')
