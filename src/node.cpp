/*
 * node.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: agu
 */

#include "node.h"

#include "i2c/i2c.h"
#include "adxl345/adxl345.h"
#include "at24c/at24c.h"

Gpio i2c_scl(GPIOB, GPIO_Pin_6, RCC_APB2Periph_GPIOB);
Gpio i2c_sda(GPIOB, GPIO_Pin_7, RCC_APB2Periph_GPIOB);

I2c i2c(I2C1, RCC_APB1Periph_I2C1);
Adxl345 accelerometer(i2c);
At24c eeprom(i2c, 16);

Node::Node(UsartRs485Modbus & usart, uint8_t address) :
		SlaveRtu(usart, address) {
	this->initBitInputs(0);
	this->initShortInputs(3);
	this->initCoils(0);
	this->initHoldings(3);
}

Node::~Node() {
}

void Node::init() {
	i2c_scl.init(GPIO_Mode_AF_OD);
	i2c_sda.init(GPIO_Mode_AF_OD);
	i2c.init(I2C_Mode_I2C, 400000);

	accelerometer.init();

	eeprom.read(0x00, _holdings, 6);
	for (uint8_t i = 0; i < 3; i++)
		accelerometer.setOffsets(i, this->getHolding(i));
}

uint8_t Node::updateShortInputs(uint16_t index, uint16_t length) {
	for (uint16_t i = 0; i < length; i++) {
		accelerometer.measure(index + i);
		this->setShortInput(index + i,
				*(accelerometer.getAcceleratins() + index + i));
	}

	return 0;
}

uint8_t Node::updateHoldings(uint16_t index, uint16_t length) {
	for (uint16_t i = 0; i < length; i++) {
		uint16_t c = this->getHolding(index + i);
		eeprom.write((index + i) * sizeof(uint16_t), &c, sizeof(uint16_t));
		accelerometer.setOffsets(index+i, c);
	}
	return 0;
}
