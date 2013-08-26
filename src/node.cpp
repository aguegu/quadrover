/*
 * node.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: agu
 */

#include "node.h"

Node::Node(UsartRs485Modbus & usart, uint8_t address) :
		SlaveRtu(usart, address) {

}

Node::~Node() {
}

void Node::init() {
}

