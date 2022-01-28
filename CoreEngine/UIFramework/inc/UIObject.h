#pragma once
#include <list>
#include <SDL.h>

#include "IPropertyMethod.h"
#include "ISignalMethod.h"
#include "IProperty.h"

class GLRenderClipManipulator;
class RenderClipManipulator;

enum UI_TYPE : uint8_t
{
	NODE_2D_IMAGE_TYPE 		= 0U,
	NODE_2D_BUTTON_TYPE 	= 1U,
	NODE_2D_TEXT_TYPE		= 2U,
	EMPTY_NODE				= 4U,
	STACK_LAYOUT_2D			= 5U,
	NODE_3D					= 6U,
	NODE_MESH				= 7U,
	NODE_LIGHT				= 8U,
};

extern const std::map<std::string, uint8_t> gMapNodeType;


class UIObject;
typedef std::shared_ptr<UIObject> UIObjectPtr;
typedef std::list<UIObjectPtr> ObjectListType;

class UIObject : public PropertyTable, public SignalTable, public PropertyMethodTable, public SignalMethodTable, public std::enable_shared_from_this<UIObject>
{
protected:
	const std::string m_name;

	std::list<UIObjectPtr> m_childList;
	std::weak_ptr<UIObject> m_pParentUIObject;

	bool m_bleftMouseButtonDown = false;
#ifdef OPENGL_RENDERING
	glm::mat4 m_localTransform = glm::mat4(1.0f);
	glm::mat4 m_worldTransform = glm::mat4(1.0f);
#endif
	

#ifdef OPENGL_RENDERING
	std::unique_ptr<GLRenderClipManipulator> updateClipArea();
#else
	std::unique_ptr<RenderClipManipulator> updateClipArea();
#endif

	void updateBackground();
public:
	UIObject() = delete;
	UIObject(std::string name);
	virtual ~UIObject();
	virtual uint8_t getType() = 0;

	virtual UIObjectPtr clone() = 0;

	std::string getUrl() const;
	std::string getName() const;

	void moveTo(UIObjectPtr);
	void setParent(UIObjectPtr);
	void addChild(UIObjectPtr);
	void removeChild(std::string m_name);

	template<class T>
	std::shared_ptr<T> getChild(int index)
	{
		auto itr = m_childList.begin();
		std::advance(itr, index);
		if (itr == m_childList.end())
		{
			return nullptr;
		}
		else
		{
			return std::dynamic_pointer_cast<T>(*itr);
		}
	}

	template<class T>
	std::shared_ptr<T> getChild(std::string name)
	{
		std::shared_ptr<T> pObject = nullptr;
		for (auto& child : m_childList)
		{
			if (child->getName() == name)
			{
				pObject = std::dynamic_pointer_cast<T>(child);
			}
		}
		return pObject;
	}

	template<class T>
	std::shared_ptr<T> lookupNode(std::list<std::string>& url)
	{
		if (url.empty())
		{
			return nullptr;
		}

		std::string name = url.front();
		if (".." == name)
		{
			//up level
			if (m_pParentUIObject.lock())
			{
				url.pop_front();
				return m_pParentUIObject.lock()->lookupNode(url);
			}
		}
		else if ("." == name)
		{
			//current level
			url.pop_front();
			return this->lookupNode(url);
		}
		else
		{
			for (auto& child : m_childList)
			{
				if (child->getName() == name)
				{
					url.pop_front();
					if (url.empty())
					{
						return std::dynamic_pointer_cast<T>(child);
					}
					else
					{
						return child->lookupNode<T>(url);
					}
				}
			}
		}
		return nullptr;
	}

	void onInit();
	void onDraw();
	void onClean();

	void onKeyInputEvent(SDL_Event&);
#ifdef OPENGL_RENDERING
	void updateWorldTransform();
	void updateLocalTransform();
	glm::mat4 worldTransform() const;
private:
	void updateLocalTransform2D();
	void updateLocalTransform3D();
	glm::vec3 getOscillation(int, int, glm::vec2, glm::vec3);
#endif
};