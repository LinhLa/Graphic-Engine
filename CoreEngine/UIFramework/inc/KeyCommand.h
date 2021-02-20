#pragma once
#include "SingletonHolder.h"
#include <unordered_map>
#include <functional>

class KeyCommand final : public SingletonHolder<KeyCommand>
{
public:
	typedef std::function<void()> callableType;
private:
	std::unordered_map<std::string, callableType> m_commandMap;
	KeyCommand();
public:
	~KeyCommand();
	friend class SingletonHolder<KeyCommand>;

	void addCommand(std::string, callableType);
	callableType getCommand(std::string);
};
