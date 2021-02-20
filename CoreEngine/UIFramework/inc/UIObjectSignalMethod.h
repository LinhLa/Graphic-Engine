#pragma once
#include <SDL.h>
#include <string>
#include "creator.h"
#include "ISignalMethod.h"
#include "Signal.h"

class UIObjectSignalMethod final: public ISignalMethod, public creator<UIObjectSignalMethod>
{
private:
	SignalTable* m_pSignalTable = nullptr;
protected:
	UIObjectSignalMethod(SignalTable*);
public:
	~UIObjectSignalMethod();
	friend class creator<UIObjectSignalMethod>;

	void init() override;
};

typedef std::shared_ptr<UIObjectSignalMethod> UIObjectSignalMethodPtr;