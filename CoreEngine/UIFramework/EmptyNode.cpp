#include "stdafx.h"
#include <base_define.h>
#include <EmptyNode.h>
EmptyNode::EmptyNode(std::string name):UIObject(name)
{}

EmptyNode::~EmptyNode() {}

uint8_t EmptyNode::getType()
{
	return EMPTY_NODE;
}

void EmptyNode::onInit(VoidType&&)
{}

void EmptyNode::onDraw(VoidType&&)
{}

void EmptyNode::onClean(VoidType&&)
{}