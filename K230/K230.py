import time, os, sys
import math
import cv_lite  # 导入cv_lite扩展模块
import ulab.numpy as np  # 导入numpy库
from media.sensor import *
from media.display import *
from media.media import *
from machine import UART, FPIOA

# --------------------------- 硬件初始化 ---------------------------
# 串口初始化
fpioa = FPIOA()
fpioa.set_function(11, FPIOA.UART2_TXD)
fpioa.set_function(12, FPIOA.UART2_RXD)
uart = UART(UART.UART2, 115200)  # 设置串口号2和波特率

# 屏幕分辨率设置
lcd_width = 800
lcd_height = 480

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
MIN_ASPECT_RATIO =0.5
MAX_ASPECT_RATIO = 2,5

# 虚拟坐标与圆形参数
BASE_RADIUS = 45
POINTS_PER_CIRCLE = 24
PURPLE_THRESHOLD = (20, 60, 15, 70, -70, -20)

# 基础矩形参数
RECT_WIDTH = 210
RECT_HEIGHT = 95

# --------------------------- 工具函数 ---------------------------
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

# --------------------------- 主循环 ---------------------------
clock = time.clock()
image_shape = [sensor.height(), sensor.width()]
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

    # 显示图像
    Display.show_image(img,
                      x=round((lcd_width-sensor.width())/2),
                      y=round((lcd_height-sensor.height())/2))

    #print(f"FPS: {fps:.1f}")
