#include "hal_gpio.h"
// #include "grbl.h"


void hal_led_gpio_init(void) {
	
    GPIO_InitTypeDef GPIO_Init; 
	
	LASER_PIN_CLK_ENABLE();

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pin = LASER_PIN;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(LASER_PORT, &GPIO_Init);
}

void hal_led_on(void) {
	HAL_GPIO_WritePin(LASER_PORT, LASER_PIN, GPIO_PIN_RESET);
}

void hal_led_off(void) {
	HAL_GPIO_WritePin(LASER_PORT, LASER_PIN, GPIO_PIN_SET);
}

void hal_led_toggle(void) {
	HAL_GPIO_TogglePin(LASER_PORT, LASER_PIN);
}

/*
	Choose limits pins need know, if use st mcu ,you must konw pins interrupt 
	int whitch exit line? you'd best make the pins in the same exit line! than 
	you just need to use the same irq handle function to deal with the job.
*/
void hal_limit_gpio_init(void) {

	GPIO_InitTypeDef GPIO_Init = {0};

	GPIO_Init.Mode = GPIO_MODE_IT_RISING;
	GPIO_Init.Pull = GPIO_NOPULL;
#ifdef LIMIT_X_PIN
	GPIO_Init.Pin = LIMIT_X_PIN;
    HAL_GPIO_Init(LIMIT_X_PORT, &GPIO_Init);
#endif

#ifdef LIMIT_Y_PIN
	GPIO_Init.Pin = LIMIT_Y_PIN;
    HAL_GPIO_Init(LIMIT_Y_PORT, &GPIO_Init);
#endif

#ifdef LIMIT_Z_PIN 
	GPIO_Init.Pin = LIMIT_Z_PIN;
    HAL_GPIO_Init(LIMIT_Z_PORT, &GPIO_Init);
#endif
}

void hal_limit_gpio_irq_enable(void) {

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(LIMIT_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(LIMIT_IRQn);
}

void hal_limit_gpio_irq_disable(void) {

	HAL_NVIC_DisableIRQ(LIMIT_IRQn);
}

void hal_probe_gpio_init(void) {

	GPIO_InitTypeDef GPIO_Init = {0};

	GPIO_Init.Mode = MODE_INPUT;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Pin = PROBE_PIN;
    HAL_GPIO_Init(PROBE_PORT, &GPIO_Init);
}

uint8_t hal_probe_gpio_read(void) {

	return HAL_GPIO_ReadPin(PROBE_PORT, PROBE_PIN);
}

uint8_t hal_limits_get_gpio_status(uint8_t axis) {
#ifdef LIMIT_X_PIN
	if(axis == X_AXIS) 			return HAL_GPIO_ReadPin(LIMIT_X_PORT, LIMIT_X_PIN);
#ifdef LIMIT_Y_PIN
		else if(axis == Y_AXIS)		return HAL_GPIO_ReadPin(LIMIT_Y_PORT, LIMIT_Y_PIN);
#endif

#ifdef LIMIT_Z_PIN
	else if(axis == Z_AXIS)		return HAL_GPIO_ReadPin(LIMIT_Z_PORT, LIMIT_Z_PIN);
#endif
    else return 0;
#else 
	return 0;
#endif
}

uint8_t hal_get_all_limits_status(uint8_t bit_select) {

	uint8_t limit_status = 0x00;
	// if(bit_select) limit_status = 0x0;
	// else limit_status = 0xff;

	limit_status |= HAL_GPIO_ReadPin(LIMIT_X_PORT, LIMIT_X_PIN) << 0;
	limit_status |= (HAL_GPIO_ReadPin(LIMIT_Y_PORT, LIMIT_Y_PIN)) << 1;
#ifdef LIMIT_Z_PIN
	limit_status |= (HAL_GPIO_ReadPin(LIMIT_Y_PORT, LIMIT_Y_PIN)) << 2;
#endif
	return limit_status;
}


static void hal_motor_en_gpio_init(void) {

	GPIO_InitTypeDef GPIO_Init; 

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	GPIO_Init.Pin = STEP_EN_PIN;
    HAL_GPIO_Init(STEP_EN_PORT, &GPIO_Init);
}

static void hal_motor_dir_gpio_init(void) {

	GPIO_InitTypeDef GPIO_Init; 
	
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	GPIO_Init.Pin = MOTOR_X_DIR_PIN;
    HAL_GPIO_Init(MOTOR_X_DIR_PORT, &GPIO_Init);

	GPIO_Init.Pin = MOTOR_Y_DIR_PIN;
    HAL_GPIO_Init(MOTOR_Y_DIR_PORT, &GPIO_Init);
#ifdef MOTOR_Z_DIR_PIN
	GPIO_Init.Pin = MOTOR_Z_DIR_PIN;
    HAL_GPIO_Init(MOTOR_Z_DIR_PORT, &GPIO_Init);
#endif
}

static void hal_motor_axis_gpio_init(void) {

	GPIO_InitTypeDef GPIO_Init; 

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	GPIO_Init.Pin = MOTOR_X_AXIS_PIN;
    HAL_GPIO_Init(MOTOR_X_AXIS_PORT, &GPIO_Init);

	GPIO_Init.Pin = MOTOR_Y_AXIS_PIN;
    HAL_GPIO_Init(MOTOR_Y_AXIS_PORT, &GPIO_Init);
#ifdef MOTOR_Z_AXIS_PIN
	GPIO_Init.Pin = MOTOR_Z_AXIS_PIN;
    HAL_GPIO_Init(MOTOR_Z_AXIS_PORT, &GPIO_Init);
#endif
}


void hal_motor_gpio_init(void) {
	hal_motor_en_gpio_init();
	hal_motor_dir_gpio_init();
	hal_motor_axis_gpio_init();
}

void hal_step_en_gpio_set(bool status) {
	if(status) { HAL_GPIO_WritePin(STEP_EN_PORT, STEP_EN_PIN, GPIO_PIN_SET); }
	else { HAL_GPIO_WritePin(STEP_EN_PORT, STEP_EN_PIN, GPIO_PIN_RESET); }
}

uint8_t hal_return_axix_gpio_status(uint8_t axis) {
	uint8_t mask = 0;
	switch(axis) {
		case X_AXIS: mask = READ_BIT(MOTOR_X_AXIS_PORT->ODR, MOTOR_X_AXIS_PIN); break;
		case Y_AXIS: mask = READ_BIT(MOTOR_Y_AXIS_PORT->ODR, MOTOR_Y_AXIS_PIN); break;
	#ifdef MOTOR_Z_AXIS_PIN
		case Z_AXIS: mask = READ_BIT(MOTOR_Z_AXIS_PORT->ODR, MOTOR_Z_AXIS_PIN); break;
	#endif
	}
	return mask;
}

void hal_set_dir_gpio_status(uint8_t mask) {

	if(mask & 0x01) {HAL_GPIO_WritePin(MOTOR_X_DIR_PORT, MOTOR_X_DIR_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_X_DIR_PORT, MOTOR_X_DIR_PIN, GPIO_PIN_RESET);}

	if(mask & 0x02) {HAL_GPIO_WritePin(MOTOR_Y_DIR_PORT, MOTOR_Y_DIR_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_Y_DIR_PORT, MOTOR_Y_DIR_PIN, GPIO_PIN_RESET);}

#ifdef MOTOR_Z_AXIS_PIN
	if(mask & 0x04) {HAL_GPIO_WritePin(MOTOR_Z_DIR_PORT, MOTOR_Z_DIR_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_Z_DIR_PORT, MOTOR_Z_DIR_PIN, GPIO_PIN_RESET);}
#endif
}

void hal_set_step_gpio_status(uint8_t mask) {

	if(mask & 0x01) {HAL_GPIO_WritePin(MOTOR_X_AXIS_PORT, MOTOR_X_AXIS_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_X_AXIS_PORT, MOTOR_X_AXIS_PIN, GPIO_PIN_RESET);}

	if(mask & 0x02) {HAL_GPIO_WritePin(MOTOR_Y_AXIS_PORT, MOTOR_Y_AXIS_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_Y_AXIS_PORT, MOTOR_Y_AXIS_PIN, GPIO_PIN_RESET);}

	#ifdef MOTOR_Z_AXIS_PIN
		if(mask & 0x04) {HAL_GPIO_WritePin(MOTOR_Z_AXIS_PORT, MOTOR_Z_AXIS_PIN, GPIO_PIN_SET);}
		else {HAL_GPIO_WritePin(MOTOR_Z_AXIS_PORT, MOTOR_Z_AXIS_PIN, GPIO_PIN_RESET);}
	#endif
}


uint8_t hal_get_moter_dir_gpio_mask(uint8_t axis) {
	if(axis == X_AXIS) return 0x01;
	else if(axis == Y_AXIS)	return 0x02;
	else return 0x04;
}

uint8_t hal_get_moter_axis_gpio_mask(uint8_t axis) {
	
	if(axis == X_AXIS) return 0x01;
	else if(axis == Y_AXIS)	return 0x02;
	else return 0x04;
}








