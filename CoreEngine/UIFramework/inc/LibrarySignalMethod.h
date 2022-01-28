#pragma once
#include <string>
#include "creator.h"
#include "ISignalMethod.h"
#include "Signal.h"

class LibrarySignalMethod final: public ISignalMethod, public creator<LibrarySignalMethod>
{
private:
	SignalTable* m_pSignalTable = nullptr;
protected:
	LibrarySignalMethod(SignalTable*);
public:
	~LibrarySignalMethod();
	friend class creator<LibrarySignalMethod>;

	void init() override;
};

typedef std::shared_ptr<LibrarySignalMethod> LibrarySignalMethodPtr;