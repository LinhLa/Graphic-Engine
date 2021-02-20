#include "stdafx.h"
#include "KeyCommand.h"

using callableType = KeyCommand::callableType;

KeyCommand::KeyCommand(){}

KeyCommand::~KeyCommand(){}

void KeyCommand::addCommand(std::string command, callableType callable)
{
	m_commandMap[command] = callable;
}

callableType KeyCommand::getCommand(std::string command)
{
	auto itrResult = m_commandMap.find(command);
	if (itrResult != m_commandMap.end())
	{
		return itrResult->second;
	}
	return nullptr;
}