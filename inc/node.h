/*
 * node.h
 *
 *  Created on: Jul 23, 2013
 *      Author: agu
 */

#ifndef NODE_H_
#define NODE_H_

#include <cstdlib>

#include "modbus/slave-rtu.h"
#include "gpio/gpio.h"
#include "adc/adc.h"
#include "dma/dma.h"

class Node: public SlaveRtu {
public:
	Node(UsartRs485Modbus & usart, uint8_t address);
	~Node();
	void init();

private:

	uint8_t updateBitInputs(uint16_t index, uint16_t length) {return 0;}
	uint8_t updateShortInputs(uint16_t index, uint16_t length) {return 0;}
	uint8_t updateCoils(uint16_t index, uint16_t length) {return 0;}
	uint8_t updateHoldings(uint16_t index, uint16_t length) {return 0;}
};

#endif /* NODE_H_ */
