#include "key.h"

static uint8_t readColumns(void) {
    uint8_t colState = 0;
    
    if (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_0) != 0) colState |= 0x01;
    if (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_1) != 0) colState |= 0x02;
    if (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_12) != 0) colState |= 0x04;
    if (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_13) != 0) colState |= 0x08;
    
    return colState;
}

// 扫描一行并返回按键值，无按键返回0
static uint8_t scanRow(uint8_t row) {
    uint8_t colState = readColumns();
    
    switch (colState) {
        case 0x01: return row * 4 + 1;  // 第1列
        case 0x02: return row * 4 + 2;  // 第2列
        case 0x04: return row * 4 + 3;  // 第3列
        case 0x08: return row * 4 + 4;  // 第4列
        default: return 0;              // 无按键或多按键
    }
}

// 读取矩阵按键值，返回1-16，无按键返回0
uint8_t Key_Read(void) {
    static uint8_t lastKey = 0;
    uint8_t currentKey = 0;
    uint8_t tempKey;
    
    // 逐行扫描（拉高当前行，其余行保持低电平）
    // 扫描第1行
    DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_6);       // 拉高行1
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_7);     // 行2保持低
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_23);     // 行3保持低
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_27);     // 行4保持低
    tempKey = scanRow(0);
    if (tempKey != 0) currentKey = tempKey;
    
    // 扫描第2行
    if (currentKey == 0) {
        DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_6); // 行1恢复低
        DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_7);   // 拉高行2
        tempKey = scanRow(1);
        if (tempKey != 0) currentKey = tempKey;
    }
    
    // 扫描第3行
    if (currentKey == 0) {
        DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_7); // 行2恢复低
        DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_23);   // 拉高行3
        tempKey = scanRow(2);
        if (tempKey != 0) currentKey = tempKey;
    }
    
    // 扫描第4行
    if (currentKey == 0) {
        DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_23); // 行3恢复低
        DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_27);   // 拉高行4
        tempKey = scanRow(3);
        if (tempKey != 0) currentKey = tempKey;
    }
    
    // 恢复所有行为低电平
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_6);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_7);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_23);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_27);
    
  
    return currentKey;
  
}

