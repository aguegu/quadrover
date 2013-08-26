/*
 * node.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: agu
 */

#include "node.h"

#include "i2c/i2c.h"
#include "adxl345/adxl345.h"

Gpio i2c_scl(GPIOB, GPIO_Pin_6, RCC_APB2Periph_GPIOB);
Gpio i2c_sda(GPIOB, GPIO_Pin_7, RCC_APB2Periph_GPIOB);

I2c i2c(I2C1, RCC_APB1Periph_I2C1);
Adxl345 acclerator(i2c);

Node::Node(UsartRs485Modbus & usart, uint8_t address) :
		SlaveRtu(usart, address) {
	this->initBitInputs(0);
	this->initShortInputs(3);
	this->initCoils(0);
	this->initHoldings(0);
}

Node::~Node() {
}

void Node::init() {

	this->SlaveRtu::init();

	i2c_scl.init(GPIO_Mode_AF_OD);
	i2c_sda.init(GPIO_Mode_AF_OD);
	i2c.init(I2C_Mode_I2C, 400000);

	acclerator.init();
}

uint8_t Node::updateShortInputs(uint16_t index, uint16_t length) {

	for (uint16_t i = 0; i < length; i++) {
		acclerator.measure();
		this->setShortInput(index + i,
				*(acclerator.getAcceleratins() + index + i));
	}

	return 0;
}
