/*
 * slave-rtu.h
 *
 *  Created on: Jul 17, 2013
 *      Author: agu
 */

#ifndef SLAVE_RTU_H_
#define SLAVE_RTU_H_

#include <cstdlib>
#include <cstring>

#include "stm32f10x.h"

#include "tim/tim.h"
#include "usart/usart.h"
#include "nvic/nvic.h"
#include "gpio/gpio.h"
#include "common/bit_op.h"

#include "crc.h"

class SlaveRtu {
public:
	SlaveRtu(Usart & usart, Tim & tim, uint8_t address,
			uint8_t * supportted_functions);
	~SlaveRtu();
	void init();
	void handler();
	void handleTimIrq();

	void setCoil(uint16_t index, BitAction state);
	BitAction getCoil(uint16_t index);

	void setDiscreteInput(uint16_t index, BitAction state);
	BitAction getDiscreteInput(uint16_t index);


private:
	Usart & _usart;
	Tim & _tim;
	volatile bool _is_receiving;
	static const uint16_t _BUFF_LENGTH = 256;
	uint8_t _buff_rx[_BUFF_LENGTH];
	uint8_t _buff_tx[_BUFF_LENGTH];
	const uint8_t _address;
	uint8_t * _supportted_functions;

	uint16_t _coils_length;
	uint8_t * _coils;

	uint16_t _dis_length;	// Discrete Inputs
	uint8_t * _dis;

	bool checkFrameCrc(const uint8_t *p, uint8_t length);
	bool isFunctionSupportted(uint8_t function);
	void appendCrcAndReply(uint8_t length);

	uint8_t responseReadCoils(uint8_t * length);
	uint8_t responseWriteCoils(uint8_t * length);

	uint8_t responseReadDiscreteInputs(uint8_t * length);
};

#endif /* SLAVE_RTU_H_ */