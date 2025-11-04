/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA1
#define PWM_0_INST_IRQHandler                                   TIMA1_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA1_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                              1250000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_17
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM39)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM39_PF_TIMA1_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_16
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM38)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM38_PF_TIMA1_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for BJDJ */
#define BJDJ_INST                                                          TIMG8
#define BJDJ_INST_IRQHandler                                    TIMG8_IRQHandler
#define BJDJ_INST_INT_IRQN                                      (TIMG8_INT_IRQn)
#define BJDJ_INST_CLK_FREQ                                              40000000
/* GPIO defines for channel 0 */
#define GPIO_BJDJ_C0_PORT                                                  GPIOA
#define GPIO_BJDJ_C0_PIN                                          DL_GPIO_PIN_21
#define GPIO_BJDJ_C0_IOMUX                                       (IOMUX_PINCM46)
#define GPIO_BJDJ_C0_IOMUX_FUNC                      IOMUX_PINCM46_PF_TIMG8_CCP0
#define GPIO_BJDJ_C0_IDX                                     DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_BJDJ_C1_PORT                                                  GPIOA
#define GPIO_BJDJ_C1_PIN                                           DL_GPIO_PIN_2
#define GPIO_BJDJ_C1_IOMUX                                        (IOMUX_PINCM7)
#define GPIO_BJDJ_C1_IOMUX_FUNC                       IOMUX_PINCM7_PF_TIMG8_CCP1
#define GPIO_BJDJ_C1_IDX                                     DL_TIMER_CC_1_INDEX

/* Defines for PWM_1 */
#define PWM_1_INST                                                         TIMG6
#define PWM_1_INST_IRQHandler                                   TIMG6_IRQHandler
#define PWM_1_INST_INT_IRQN                                     (TIMG6_INT_IRQn)
#define PWM_1_INST_CLK_FREQ                                             80000000
/* GPIO defines for channel 1 */
#define GPIO_PWM_1_C1_PORT                                                 GPIOA
#define GPIO_PWM_1_C1_PIN                                         DL_GPIO_PIN_22
#define GPIO_PWM_1_C1_IOMUX                                      (IOMUX_PINCM47)
#define GPIO_PWM_1_C1_IOMUX_FUNC                     IOMUX_PINCM47_PF_TIMG6_CCP1
#define GPIO_PWM_1_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (49999U)
/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMA0)
#define TIMER_1_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                         (19999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)




/* Defines for SPI_LCD */
#define SPI_LCD_INST                                                       SPI1
#define SPI_LCD_INST_IRQHandler                                 SPI1_IRQHandler
#define SPI_LCD_INST_INT_IRQN                                     SPI1_INT_IRQn
#define GPIO_SPI_LCD_PICO_PORT                                            GPIOB
#define GPIO_SPI_LCD_PICO_PIN                                     DL_GPIO_PIN_8
#define GPIO_SPI_LCD_IOMUX_PICO                                 (IOMUX_PINCM25)
#define GPIO_SPI_LCD_IOMUX_PICO_FUNC                 IOMUX_PINCM25_PF_SPI1_PICO
/* GPIO configuration for SPI_LCD */
#define GPIO_SPI_LCD_SCLK_PORT                                            GPIOB
#define GPIO_SPI_LCD_SCLK_PIN                                     DL_GPIO_PIN_9
#define GPIO_SPI_LCD_IOMUX_SCLK                                 (IOMUX_PINCM26)
#define GPIO_SPI_LCD_IOMUX_SCLK_FUNC                 IOMUX_PINCM26_PF_SPI1_SCLK



/* Defines for ADC1 */
#define ADC1_INST                                                           ADC1
#define ADC1_INST_IRQHandler                                     ADC1_IRQHandler
#define ADC1_INST_INT_IRQN                                       (ADC1_INT_IRQn)
#define ADC1_ADCMEM_ADC_Channel0                              DL_ADC12_MEM_IDX_0
#define ADC1_ADCMEM_ADC_Channel0_REF             DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC1_ADCMEM_ADC_Channel0_REF_VOLTAGE_V                                     3.3
#define GPIO_ADC1_C0_PORT                                                  GPIOA
#define GPIO_ADC1_C0_PIN                                          DL_GPIO_PIN_15



/* Port definition for Pin Group LED1 */
#define LED1_PORT                                                        (GPIOB)

/* Defines for PIN_22: GPIOB.22 with pinCMx 50 on package pin 21 */
#define LED1_PIN_22_PIN                                         (DL_GPIO_PIN_22)
#define LED1_PIN_22_IOMUX                                        (IOMUX_PINCM50)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOB)

/* Defines for PIN_21: GPIOB.21 with pinCMx 49 on package pin 20 */
#define KEY_PIN_21_PIN                                          (DL_GPIO_PIN_21)
#define KEY_PIN_21_IOMUX                                         (IOMUX_PINCM49)
/* Port definition for Pin Group AIN1 */
#define AIN1_PORT                                                        (GPIOA)

/* Defines for A_PIN_24: GPIOA.24 with pinCMx 54 on package pin 25 */
#define AIN1_A_PIN_24_PIN                                       (DL_GPIO_PIN_24)
#define AIN1_A_PIN_24_IOMUX                                      (IOMUX_PINCM54)
/* Port definition for Pin Group AIN2 */
#define AIN2_PORT                                                        (GPIOA)

/* Defines for A_PIN_25: GPIOA.25 with pinCMx 55 on package pin 26 */
#define AIN2_A_PIN_25_PIN                                       (DL_GPIO_PIN_25)
#define AIN2_A_PIN_25_IOMUX                                      (IOMUX_PINCM55)
/* Port definition for Pin Group BIN1 */
#define BIN1_PORT                                                        (GPIOB)

/* Defines for B_PIN_24: GPIOB.24 with pinCMx 52 on package pin 23 */
#define BIN1_B_PIN_24_PIN                                       (DL_GPIO_PIN_24)
#define BIN1_B_PIN_24_IOMUX                                      (IOMUX_PINCM52)
/* Port definition for Pin Group BIN2 */
#define BIN2_PORT                                                        (GPIOB)

/* Defines for B_PIN_25: GPIOB.25 with pinCMx 56 on package pin 27 */
#define BIN2_B_PIN_25_PIN                                       (DL_GPIO_PIN_25)
#define BIN2_B_PIN_25_IOMUX                                      (IOMUX_PINCM56)
/* Port definition for Pin Group GPIO */
#define GPIO_PORT                                                        (GPIOB)

/* Defines for IN1: GPIOB.5 with pinCMx 18 on package pin 53 */
#define GPIO_IN1_PIN                                             (DL_GPIO_PIN_5)
#define GPIO_IN1_IOMUX                                           (IOMUX_PINCM18)
/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_30: GPIOA.30 with pinCMx 5 on package pin 37 */
#define GPIO_GRP_0_PIN_30_PIN                                   (DL_GPIO_PIN_30)
#define GPIO_GRP_0_PIN_30_IOMUX                                   (IOMUX_PINCM5)
/* Port definition for Pin Group ENCODER */
#define ENCODER_PORT                                                     (GPIOA)

/* Defines for E1A: GPIOA.27 with pinCMx 60 on package pin 31 */
// pins affected by this interrupt request:["E1A","E1B","E2A","E2B"]
#define ENCODER_INT_IRQN                                        (GPIOA_INT_IRQn)
#define ENCODER_INT_IIDX                        (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define ENCODER_E1A_IIDX                                    (DL_GPIO_IIDX_DIO27)
#define ENCODER_E1A_PIN                                         (DL_GPIO_PIN_27)
#define ENCODER_E1A_IOMUX                                        (IOMUX_PINCM60)
/* Defines for E1B: GPIOA.26 with pinCMx 59 on package pin 30 */
#define ENCODER_E1B_IIDX                                    (DL_GPIO_IIDX_DIO26)
#define ENCODER_E1B_PIN                                         (DL_GPIO_PIN_26)
#define ENCODER_E1B_IOMUX                                        (IOMUX_PINCM59)
/* Defines for E2A: GPIOA.31 with pinCMx 6 on package pin 39 */
#define ENCODER_E2A_IIDX                                    (DL_GPIO_IIDX_DIO31)
#define ENCODER_E2A_PIN                                         (DL_GPIO_PIN_31)
#define ENCODER_E2A_IOMUX                                         (IOMUX_PINCM6)
/* Defines for E2B: GPIOA.28 with pinCMx 3 on package pin 35 */
#define ENCODER_E2B_IIDX                                    (DL_GPIO_IIDX_DIO28)
#define ENCODER_E2B_PIN                                         (DL_GPIO_PIN_28)
#define ENCODER_E2B_IOMUX                                         (IOMUX_PINCM3)
/* Port definition for Pin Group IIC_Software */
#define IIC_Software_PORT                                                (GPIOA)

/* Defines for SCL: GPIOA.1 with pinCMx 2 on package pin 34 */
#define IIC_Software_SCL_PIN                                     (DL_GPIO_PIN_1)
#define IIC_Software_SCL_IOMUX                                    (IOMUX_PINCM2)
/* Defines for SDA: GPIOA.0 with pinCMx 1 on package pin 33 */
#define IIC_Software_SDA_PIN                                     (DL_GPIO_PIN_0)
#define IIC_Software_SDA_IOMUX                                    (IOMUX_PINCM1)
/* Port definition for Pin Group GPIO_LCD */
#define GPIO_LCD_PORT                                                    (GPIOB)

/* Defines for PIN_RES: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_LCD_PIN_RES_PIN                                    (DL_GPIO_PIN_10)
#define GPIO_LCD_PIN_RES_IOMUX                                   (IOMUX_PINCM27)
/* Defines for PIN_DC: GPIOB.11 with pinCMx 28 on package pin 63 */
#define GPIO_LCD_PIN_DC_PIN                                     (DL_GPIO_PIN_11)
#define GPIO_LCD_PIN_DC_IOMUX                                    (IOMUX_PINCM28)
/* Defines for PIN_CS: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_LCD_PIN_CS_PIN                                     (DL_GPIO_PIN_14)
#define GPIO_LCD_PIN_CS_IOMUX                                    (IOMUX_PINCM31)
/* Defines for PIN_BLK: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_LCD_PIN_BLK_PIN                                    (DL_GPIO_PIN_26)
#define GPIO_LCD_PIN_BLK_IOMUX                                   (IOMUX_PINCM57)
/* Port definition for Pin Group KEY4 */
#define KEY4_PORT                                                        (GPIOB)

/* Defines for PIN_B00: GPIOB.0 with pinCMx 12 on package pin 47 */
#define KEY4_PIN_B00_PIN                                         (DL_GPIO_PIN_0)
#define KEY4_PIN_B00_IOMUX                                       (IOMUX_PINCM12)
/* Defines for PIN_B01: GPIOB.1 with pinCMx 13 on package pin 48 */
#define KEY4_PIN_B01_PIN                                         (DL_GPIO_PIN_1)
#define KEY4_PIN_B01_IOMUX                                       (IOMUX_PINCM13)
/* Defines for PIN_B12: GPIOB.12 with pinCMx 29 on package pin 64 */
#define KEY4_PIN_B12_PIN                                        (DL_GPIO_PIN_12)
#define KEY4_PIN_B12_IOMUX                                       (IOMUX_PINCM29)
/* Defines for PIN_B13: GPIOB.13 with pinCMx 30 on package pin 1 */
#define KEY4_PIN_B13_PIN                                        (DL_GPIO_PIN_13)
#define KEY4_PIN_B13_IOMUX                                       (IOMUX_PINCM30)
/* Defines for PIN_B06: GPIOB.6 with pinCMx 23 on package pin 58 */
#define KEY4_PIN_B06_PIN                                         (DL_GPIO_PIN_6)
#define KEY4_PIN_B06_IOMUX                                       (IOMUX_PINCM23)
/* Defines for PIN_B07: GPIOB.7 with pinCMx 24 on package pin 59 */
#define KEY4_PIN_B07_PIN                                         (DL_GPIO_PIN_7)
#define KEY4_PIN_B07_IOMUX                                       (IOMUX_PINCM24)
/* Defines for PIN_B23: GPIOB.23 with pinCMx 51 on package pin 22 */
#define KEY4_PIN_B23_PIN                                        (DL_GPIO_PIN_23)
#define KEY4_PIN_B23_IOMUX                                       (IOMUX_PINCM51)
/* Defines for PIN_B27: GPIOB.27 with pinCMx 58 on package pin 29 */
#define KEY4_PIN_B27_PIN                                        (DL_GPIO_PIN_27)
#define KEY4_PIN_B27_IOMUX                                       (IOMUX_PINCM58)
/* Port definition for Pin Group Gray_Address */
#define Gray_Address_PORT                                                (GPIOB)

/* Defines for PIN_0: GPIOB.2 with pinCMx 15 on package pin 50 */
#define Gray_Address_PIN_0_PIN                                   (DL_GPIO_PIN_2)
#define Gray_Address_PIN_0_IOMUX                                 (IOMUX_PINCM15)
/* Defines for PIN_1: GPIOB.3 with pinCMx 16 on package pin 51 */
#define Gray_Address_PIN_1_PIN                                   (DL_GPIO_PIN_3)
#define Gray_Address_PIN_1_IOMUX                                 (IOMUX_PINCM16)
/* Defines for PIN_2: GPIOB.4 with pinCMx 17 on package pin 52 */
#define Gray_Address_PIN_2_PIN                                   (DL_GPIO_PIN_4)
#define Gray_Address_PIN_2_IOMUX                                 (IOMUX_PINCM17)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_SYSCTL_CLK_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_BJDJ_init(void);
void SYSCFG_DL_PWM_1_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_SPI_LCD_init(void);
void SYSCFG_DL_ADC1_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
