#include "stdafx.h"
#include "UIObjectSignalMethod.h"
#include "SignalDefine.h"

UIObjectSignalMethod::UIObjectSignalMethod(SignalTable* pSignalTable):m_pSignalTable(pSignalTable)
{
	if (!m_pSignalTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

UIObjectSignalMethod::~UIObjectSignalMethod() {}

void UIObjectSignalMethod::init()
{
	m_pSignalTable->AddSignal(ON_DRAW_SIGNAL, Signal<VoidType>::create());
	m_pSignalTable->AddSignal(ON_DRAW_DONE_SIGNAL, Signal<VoidType>::create());
	m_pSignalTable->AddSignal(ON_CLEAN_SIGNAL, Signal<VoidType>::create());
	m_pSignalTable->AddSignal(ON_INIT_SIGNAL, Signal<VoidType>::create());
}
