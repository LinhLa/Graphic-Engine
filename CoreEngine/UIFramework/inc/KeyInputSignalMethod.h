#pragma once
#include <SDL.h>
#include <string>
#include "creator.h"
#include "ISignalMethod.h"
#include "Signal.h"

class KeyInputSignalMethod final: public ISignalMethod, public creator<KeyInputSignalMethod>
{
private:
	SignalTable* m_pSignalTable = nullptr;
protected:
	KeyInputSignalMethod(SignalTable*);
public:
	~KeyInputSignalMethod();
	friend class creator<KeyInputSignalMethod>;

	void init() override;
};

typedef std::shared_ptr<KeyInputSignalMethod> KeyInputSignalMethodPtr;