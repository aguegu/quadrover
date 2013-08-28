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

#include "usart-rs485-modbus.h"
#include "nvic/nvic.h"
#include "gpio/gpio.h"
#include "common/bit_op.h"

#include "crc.h"

class SlaveRtu {
public:
	SlaveRtu(UsartRs485Modbus & usart, uint8_t address);
	virtual ~SlaveRtu();

	void init();
	void handler();
	void handleTimIrq();

	void initBitInputs(uint16_t length);
	void initShortInputs(uint16_t length);

	void initCoils(uint16_t length);
	void initHoldings(uint16_t length);

	void setBitInput(uint16_t index, BitAction state);
	BitAction getBitInput(uint16_t index);

	void setShortInput(uint16_t index, uint16_t val);
	uint16_t getShortInput(uint16_t index);

	void setCoil(uint16_t index, BitAction state);
	BitAction getCoil(uint16_t index);

	void setHolding(uint16_t index, uint16_t val);
	uint16_t getHolding(uint16_t index);

protected:

	uint8_t * _bit_inputs;
	uint16_t * _short_inputs;
	uint8_t * _coils;
	uint16_t * _holdings;

	int16_t _coil_length;
	uint16_t _bit_input_length;
	uint16_t _short_input_length;
	uint16_t _holding_length;

	virtual uint8_t updateBitInputs(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateShortInputs(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateCoils(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateHoldings(uint16_t index, uint16_t length) {return 0;};

private:
	UsartRs485Modbus & _usart;

	static const uint16_t _BUFF_LENGTH = 256;
	uint8_t _buff_rx[_BUFF_LENGTH];
	uint8_t _buff_tx[_BUFF_LENGTH];

	const uint8_t _address;



	bool checkFrameCrc(const uint8_t *p, uint8_t length);
	void appendCrcAndReply(uint8_t length_tx);

	uint8_t onReadBitInputs(uint8_t * p_length_tx);
	uint8_t onReadShortInputs(uint8_t * p_length_tx);

	uint8_t onReadCoils(uint8_t * p_length_tx);
	uint8_t onWriteSingleCoil(uint8_t * p_length_tx);
	uint8_t onWriteMultipleCoils(uint8_t length_rx, uint8_t * p_length_tx);

	uint8_t onReadHoldings(uint8_t * p_length_tx);
	uint8_t onWriteSingleHolding(uint8_t * p_length_tx);
	uint8_t onWriteMultipleHoldings(uint8_t length_rx, uint8_t * p_length_tx);
};

#endif /* SLAVE_RTU_H_ */
