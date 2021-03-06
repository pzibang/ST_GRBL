#ifndef __MAIN_H
#define __MAIN_H

#define USE_FREERTOS_RTOS

#ifdef STM32G070xx
    #include "stm32g0xx_hal.h"
#elif defined(STM32F429xx)
    #include "stm32f4xx_hal.h"
    #include "bsp_hal/bsp_f429ig/hal_f429_system.h"
#elif defined(STM32G0B0xx)
    #include "stm32g0xx_hal.h"
    #include "bsp_hal/bsp_g0b0ce/hal_g0b0_system.h"
#endif

#if defined(USE_FREERTOS_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#endif

#include "bsp_hal/grbl_hal.h"
#include "bsp_hal/hal_uart.h"
#include "bsp_hal/grbl_config.h"
#include "bsp_hal/hal_gpio.h"
#include "bsp_hal/hal_tim.h"
#include "bsp_hal/hal_flash_eeprom.h"
#include "bsp_hal/hal_spi.h"

#include "bsp_hal/hal_w25qxx.h"
#include "ex_dev/lcd/tft_lcd_dev.h"
#include "ex_dev/sd/sdcard.h"

#include "grbl/grbl.h"

#ifdef STM32F429xx
#define SYSTEM_INTI()       hal_f429_system_init()
#elif defined(STM32G0B0xx)
#define SYSTEM_INTI()       hal_g0b0_system_init()
#endif

#define SYSTEM_UART()       hal_uart_init()
#define SYSTEM_LASER()      hal_pwm_init()
#define SYSTEM_FLASH()      w25qxx_init()
#define SYSTEM_LCD()        dev_lcd_init()

void _delay_ms(uint32_t tick);
void _delay_us(uint32_t tick);

#if defined(USE_FREERTOS_RTOS)
void enter_grbl_task(void *parg);
#else
void enter_grbl_task(void);
#endif

#endif
