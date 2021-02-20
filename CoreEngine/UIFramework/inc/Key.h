#pragma once
#include "creator.h"
#include <unordered_map>
#include <functional>

class Key;
typedef std::shared_ptr<Key> KeyPtr;
typedef typename std::unordered_map<uint32_t, KeyPtr>::iterator KeyIterator;

class Key final : public creator<Key>
{
public:
	typedef std::function<void()> callableType;
private:
	uint32_t 		m_keyCode = 0;
	callableType 	m_keyReleaseCallback = nullptr;

	std::unordered_map<uint32_t, KeyPtr> m_subKey;

	Key(uint32_t, std::string);
public:
	~Key();
	friend class creator<Key>;

	void addKey(KeyPtr);
	void addKey(std::list<uint32_t>, KeyPtr);

	void removeKey(uint32_t);
	void removeKey(std::list<uint32_t>);

	void onRelease();

	uint32_t keyCode() const;
	KeyPtr getNextKey(uint32_t);
};