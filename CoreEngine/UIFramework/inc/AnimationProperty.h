#pragma once
#include <unordered_map>
#include <memory>
#include "creator.h"
#include "AnimationKeyFrame.h"
#include "UIObject.h"
class AnimationProperty: public creator<AnimationProperty>
{
private:
	std::unordered_map<std::string, IKeyFramePtr> m_entryList;
protected:
	AnimationProperty();
public:
	~AnimationProperty();

	friend class creator<AnimationProperty>;

	void onAnimation(std::chrono::milliseconds, UIObjectPtr);
	void addEntry(std::string, IKeyFramePtr);

	bool isEnd();
};

typedef std::shared_ptr<AnimationProperty> AnimationPropertyPtr;



