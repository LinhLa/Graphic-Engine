#include "stdafx.h"
#include "Signal.h"

VoidType::VoidType(){}
VoidType::~VoidType(){}

ISignal::ISignal() {}
ISignal::~ISignal() {}

SignalTable::SignalTable()
{}

SignalTable::~SignalTable()
{}

bool SignalTable::IsSignalExist(std::string signal_name)
{
	return (m_signalTable.find(signal_name) != m_signalTable.end());
}

void SignalTable::RemoveSignal(std::string signal_name)
{
	m_signalTable.erase(signal_name);
}