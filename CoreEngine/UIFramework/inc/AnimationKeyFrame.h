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
#include "bezier.h"

typedef enum {
		_bezier_curve,
        _linear,
        _ease_out,
        _ease_in,
        _ease_in_out,
} key_type;

template<typename T> class AnimationKeyFrame;

class IAnimationKeyFrame
{
public:
	IAnimationKeyFrame();
	virtual ~IAnimationKeyFrame();
	virtual const std::type_info& getTypeInfo() const = 0;
	virtual void onAnimation(std::chrono::milliseconds, UIObjectPtr, std::string) = 0;
	virtual void onReverseAnimation(std::chrono::milliseconds, UIObjectPtr, std::string) = 0;
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
	void addLinearKeyframe(std::chrono::milliseconds time, const U& value)
	{
		return DynamicTypeCheck<U>()->addLinearKeyframe(time, std::forward<const U&>(value));
	}

	template<typename U>
	void addLinearKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const U& from, const U& to)
	{
		return DynamicTypeCheck<U>()->addLinearKeyframe(start, duration, std::forward<const U&>(from), std::forward<const U&>(to));
	}

	template<typename U>
	void addSplineKeyframe(std::chrono::milliseconds time, const U& value, key_type type = _ease_in_out)
	{
		return  DynamicTypeCheck<U>()->addSplineKeyframe(time, std::forward<const U&>(value), type)
	}


	template<typename U>
	void addBezierKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const U& from, const U& to, const glm::dvec2& p0, const glm::dvec2& p1)
	{
		return DynamicTypeCheck<U>()->addBezierKeyframe(start, duration, std::forward<const U&>(from), std::forward<const U&>(to), p0, p1);
	}

	template<typename U>
	void addSplineKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const U& from, const U& to, key_type type = _ease_in_out)
	{
		return DynamicTypeCheck<U>()->addSplineKeyframe(start, duration, std::forward<const U&>(from), std::forward<const U&>(to), type);
	}
};

typedef std::shared_ptr<IAnimationKeyFrame> IKeyFramePtr;

template<typename T>
class AnimationKeyFrame final :public IAnimationKeyFrame, public creator<AnimationKeyFrame<T>>
{
public:
    class keyframe
    {
    public:
		T m_value;
    	key_type m_type;
    public:
		keyframe() {}
    	keyframe(key_type type, T value):m_type(type), m_value(value){}
    	~keyframe(){}
    };

private:
	std::map<std::chrono::milliseconds, keyframe>	m_keyframeList;
	std::map<std::chrono::milliseconds, std::vector<glm::dvec2>>	m_PointList;
	typename std::map<std::chrono::milliseconds, keyframe>::iterator m_currentKeyFrameItr = m_keyframeList.end();
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

	void onAnimation(std::chrono::milliseconds delta, UIObjectPtr pObject, std::string property_name) override
	{
		if (m_keyframeList.empty())
		{
			return;
		}
		else if (!m_bAnimationStarted)
		{
			m_currentKeyFrameItr = m_keyframeList.begin();
			m_bAnimationStarted = true;
		}
		else if (m_keyframeList.end() == m_currentKeyFrameItr)
		{
			return;
		}

		m_currentKeyFrameItr = std::find_if(m_currentKeyFrameItr, m_keyframeList.end(),
			[&](const std::pair<std::chrono::milliseconds, keyframe> &t) -> bool {return t.first >= delta; });
		if (m_keyframeList.end() == m_currentKeyFrameItr)
		{
			//aniamtion ended
			m_currentKeyFrameItr = (--m_keyframeList.end());
			if (m_keyframeList.end() != m_currentKeyFrameItr)
			{
				pObject->SetPropertyValue<T>(property_name, m_currentKeyFrameItr->second.m_value);
			}
		}
		else if (m_keyframeList.begin() == m_currentKeyFrameItr)
		{
			pObject->SetPropertyValue<T>(property_name, m_currentKeyFrameItr->second.m_value);
		}
		else
		{
			if (_bezier_curve == m_currentKeyFrameItr->second.m_type)
			{
				auto previousKeyFrameItr = std::prev(m_currentKeyFrameItr, 1);
				std::chrono::milliseconds duration = m_currentKeyFrameItr->first - previousKeyFrameItr->first;
				auto itr = m_PointList.find(previousKeyFrameItr->first);
				if (itr != m_PointList.end())
				{
					std::vector<glm::dvec2>& points = (itr->second);
					T value = static_cast<T>(bezier
					(points,
						::smootherstep
						(
											0.0,
											static_cast<double>(duration.count()),
											static_cast<double>(delta.count())
						)
					).x);
					pObject->SetPropertyValue<T>(property_name, value);
				}
			}
			else
			{
				auto previousKeyFrameItr = std::prev(m_currentKeyFrameItr, 1);
				T value = m_currentKeyFrameItr->second.m_value;
				double from = static_cast<double>(previousKeyFrameItr->second.m_value);
				double change = static_cast<double>(m_currentKeyFrameItr->second.m_value - from);
				std::chrono::milliseconds duration = m_currentKeyFrameItr->first - previousKeyFrameItr->first;
				switch (m_currentKeyFrameItr->second.m_type)
				{
				case _linear:
					value = static_cast<T>(linearTween(static_cast<double>(delta.count()), from, change, static_cast<double>(duration.count())));
					break;
				case _ease_in_out:
					value = static_cast<T>(easeInOutQuart(static_cast<double>(delta.count()), from, change, static_cast<double>(duration.count())));
					break;
				default:
					break;
				}
				pObject->SetPropertyValue<T>(property_name, value);
			}
		}
		
	}

	void onReverseAnimation(std::chrono::milliseconds delta, UIObjectPtr pObject, std::string property_name) override
	{
		if (m_keyframeList.empty())
		{
			return;
		}
		else if (!m_bAnimationStarted)
		{
			m_currentKeyFrameItr = m_keyframeList.begin();
			m_bAnimationStarted = true;
		}
		else{}

		std::chrono::milliseconds progress = (--m_keyframeList.end())->first - (m_keyframeList.begin())->first;
		if (delta > progress)
		{
			delta = std::chrono::milliseconds::zero();
		}
		else
		{
			delta = progress - delta;
		}
		return onAnimation(delta, pObject, property_name);


		/*for (auto itr = m_currentKeyFrameItr; itr != m_keyframeList.end(); --itr)
		{
			if (itr->first <= delta)
			{
				m_currentKeyFrameItr = itr;
				break;
			}
		}*/

		//if (m_keyframeList.end() == m_currentKeyFrameItr)
		//{
		//	//aniamtion ended
		//	m_currentKeyFrameItr = m_keyframeList.begin();
		//	if (m_keyframeList.end() != m_currentKeyFrameItr)
		//	{
		//		pObject->SetPropertyValue<T>(property_name, m_currentKeyFrameItr->second.m_value);
		//	}
		//}
		//else if (m_keyframeList.begin() == m_currentKeyFrameItr)
		//{
		//	pObject->SetPropertyValue<T>(property_name, m_currentKeyFrameItr->second.m_value);
		//}
		//else
		//{
		//	if (_bezier_curve == m_currentKeyFrameItr->second.m_type)
		//	{
		//		auto nextKeyFrameItr = std::next(m_currentKeyFrameItr, 1);
		//		std::chrono::milliseconds duration = nextKeyFrameItr->first - m_currentKeyFrameItr->first;
		//		auto itr = m_PointList.find(previousKeyFrameItr->first);
		//		if (itr != m_PointList.end())
		//		{
		//			std::vector<glm::dvec2> points = (itr->second);
		//			std::reverse(points.begin(), points.end());
		//			T value = static_cast<T>(bezier
		//			(points,
		//				::smootherstep
		//				(
		//					0.0,
		//					static_cast<double>(duration.count()),
		//					static_cast<double>(delta.count())
		//				)
		//			).x);
		//			pObject->SetPropertyValue<T>(property_name, value);
		//		}
		//	}
		//	else
		//	{
		//		auto previousKeyFrameItr = std::prev(m_currentKeyFrameItr, 1);
		//		T value = m_currentKeyFrameItr->second.m_value;
		//		double from = static_cast<double>(previousKeyFrameItr->second.m_value);
		//		double change = static_cast<double>(m_currentKeyFrameItr->second.m_value - from);
		//		std::chrono::milliseconds duration = m_currentKeyFrameItr->first - previousKeyFrameItr->first;
		//		switch (m_currentKeyFrameItr->second.m_type)
		//		{
		//		case _linear:
		//			value = static_cast<T>(linearTween(static_cast<double>(delta.count()), from, change, static_cast<double>(duration.count())));
		//			break;
		//		case _ease_in_out:
		//			value = static_cast<T>(easeInOutQuart(static_cast<double>(delta.count()), from, change, static_cast<double>(duration.count())));
		//			break;
		//		default:
		//			break;
		//		}
		//		pObject->SetPropertyValue<T>(property_name, value);
		//	}
		//}

	}

	std::chrono::milliseconds getLastKeyTime() override
	{
		if (!m_keyframeList.empty())
		{
			return m_keyframeList.rbegin()->first;
		}
		return 0_ms;
	}

	void addLinearKeyframe(std::chrono::milliseconds time, const T& value)
	{
		m_keyframeList[time] = keyframe(_linear, value);
	}

	void addLinearKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const T& from, const T& to)
	{
		m_keyframeList[start] = keyframe(_linear ,from);
		m_keyframeList[(start + duration)] = keyframe(_linear, to);
	}

	void addBezierKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const T& from, const T& to, const glm::dvec2& p0, const glm::dvec2& p1)
	{
		m_keyframeList[start] = keyframe(_bezier_curve, from);
		m_keyframeList[(start + duration)] = keyframe(_bezier_curve, to);

		std::vector<glm::dvec2> points = { glm::dvec2(static_cast<double>(from), 0.0) , p0, p1, glm::dvec2(static_cast<double>(to), 0.0) };
		m_PointList[start] = points;
	}

	void addSplineKeyframe(std::chrono::milliseconds time, const T& value, key_type type = _ease_in_out)
	{
		m_keyframeList[time] = keyframe(type, value);
	}

	void addSplineKeyframe(std::chrono::milliseconds start, std::chrono::milliseconds duration, const T& from, const T& to, key_type type = _ease_in_out)
	{
		m_keyframeList[start] = keyframe(type, from);
		m_keyframeList[(start + duration)] = keyframe(type, to);
	}
};