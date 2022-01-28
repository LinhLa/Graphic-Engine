#include "stdafx.h"
#include "LibrarySignalMethod.h"
#include "SignalDefine.h"

LibrarySignalMethod::LibrarySignalMethod(SignalTable* pSignalTable):m_pSignalTable(pSignalTable)
{
	if (!m_pSignalTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

LibrarySignalMethod::~LibrarySignalMethod() {}

void LibrarySignalMethod::init()
{
	m_pSignalTable->AddSignal(ON_UPDATE_SIGNAL, Signal<VoidType>::create());
}
