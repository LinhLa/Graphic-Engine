#pragma once
#include <memory>
#include <chrono>
#include <map>
#include <algorithm>

#include <glm\glm.hpp>
#include "creator.h"
#include "UIObject.h"
#include "Configuration.h"
#include "easing.h"

template<typename T> class AnimationKeyFrame;

class IAnimationKeyFrame
{
public:
	IAnimationKeyFrame();
	virtual ~IAnimationKeyFrame();
	virtual const std::type_info& getTypeInfo() const = 0;
	virtual void onAnimation(std::chrono::milliseconds, UIObjectPtr, std::string) = 0;
	virtual std::chrono::milliseconds getLastKeyTime() = 0;
private:
	template<typename U>
	AnimationKeyFrame<U>* DynamicTypeCheck()
	{
		if (typeid(U).hash_code() != getTypeInfo().hash_code())
		{
			throw std::logic_error("invalid_argument");
		}
		return dynamic_cast<AnimationKeyFrame<U>*>(this);
	}

public:
	template<typename U>
	void addLinearKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const U& from, const U& to)
	{
		DynamicTypeCheck<U>()->addLinearKeyframe(start, duration, std::forward<const U&>(from), std::forward<const U&>(to));
	}

	template<typename U>
	void addSplineKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const U& value, const glm::dvec2& p0, const glm::dvec2& p1)
	{
		return DynamicTypeCheck<U>()->addSplineKeyframe(start, duration, std::forward<U>(value), p0, p1);
	}

	template<typename U>
	void addSplineKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const U& from, const U& to)
	{
		DynamicTypeCheck<U>()->addSplineKeyframe(start, duration, std::forward<const U&>(from), std::forward<const U&>(to));
	}
};

typedef std::shared_ptr<IAnimationKeyFrame> IKeyFramePtr;

template<typename T>
class AnimationKeyFrame final :public IAnimationKeyFrame, public creator<AnimationKeyFrame<T>>
{
private:
	std::map<std::chrono::milliseconds, T>	m_keyframeList;
	typename std::map<std::chrono::milliseconds, T>::iterator m_currentKeyFrameItr = m_keyframeList.end();
	bool m_bAnimationStarted = false;
protected:
	AnimationKeyFrame() {}
public:
	virtual ~AnimationKeyFrame(){}

	friend class creator<AnimationKeyFrame<T>>;

	const std::type_info& getTypeInfo() const override
	{
		return typeid(T);
	}

	void onAnimation(std::chrono::milliseconds time, UIObjectPtr pObject, std::string property_name) override
	{
		if (!m_bAnimationStarted)
		{
			m_currentKeyFrameItr = m_keyframeList.begin();
			m_bAnimationStarted = true;
		}
		else if (m_keyframeList.end() == m_currentKeyFrameItr)
		{
			return;
		}

		m_currentKeyFrameItr = std::find_if(m_currentKeyFrameItr, m_keyframeList.end(),
			[&](const std::pair<std::chrono::milliseconds, T> & t) -> bool {return t.first >= time; });
		if (m_keyframeList.end() == m_currentKeyFrameItr)
		{
			m_currentKeyFrameItr = (--m_keyframeList.end());
		}

		pObject->SetPropertyValue<T>(property_name, m_currentKeyFrameItr->second);
	}

	std::chrono::milliseconds getLastKeyTime() override
	{
		if (!m_keyframeList.empty())
		{
			return m_keyframeList.rbegin()->first;
		}
		return 0_ms;
	}

	void addLinearKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const T& from, const T& to)
	{
		double timeStep = double(1000) / Configuration::GetInstance()->fps + 0.5F;
		double change = static_cast<double>(to - from);
		uint64_t keyTime = 0;
		while (static_cast<int64_t>(keyTime) <= duration.count())
		{
			m_keyframeList[std::chrono::milliseconds(keyTime + start.count())] = static_cast<T>(linearTween(static_cast<double>(keyTime), from, change, static_cast<double>(duration.count())));
			keyTime += static_cast<uint64_t>(timeStep);
		}
		m_keyframeList[(start + duration)] = to;
	}

	void addSplineKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const T& value, const glm::dvec2& p0, const glm::dvec2& p1)
	{

	}

	void addSplineKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const T& from, const T& to)
	{
		double timeStep = double(1000) / Configuration::GetInstance()->fps + 0.5F;
		double change = static_cast<double>(to - from);
		uint64_t keyTime = 0;
		while (static_cast<int64_t>(keyTime) <= duration.count())
		{
			m_keyframeList[std::chrono::milliseconds(keyTime + start.count())] = static_cast<T>(easeInOutQuart(static_cast<double>(keyTime), from, change, static_cast<double>(duration.count())));
			keyTime += static_cast<uint64_t>(timeStep);
		}
		m_keyframeList[(start + duration)] = to;
	}
};