/*
 * node.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: agu
 */

#include "node.h"

Node::Node(UsartRs485Modbus & usart, uint8_t address) :
		SlaveRtu(usart, address) {
	this->initBitInputs(0);
	this->initShortInputs(0);
	this->initCoils(0);
	this->initHoldings(0);
}

Node::~Node() {
}

void Node::init() {

}
