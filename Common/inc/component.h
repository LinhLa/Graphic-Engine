#pragma once
#include <list>
#include <memory>
template<class I>
class Component
{
    protected:
    std::list<std::shared_ptr<I>> m_ComponentList;
    public:
	void addComponent(std::shared_ptr<I> pComponent)
	{
	    if (pComponent)
	    {
		m_ComponentList.push_back(pComponent);
	    }
	}

	template<class C>
	void removeComponent()
	{
		for (auto existedComponent = m_ComponentList.begin(); existedComponent != m_ComponentList.end(); ++existedComponent)
		{
			if (std::dynamic_pointer_cast<C>(*existedComponent))
			{
				m_ComponentList.erase(existedComponent);
				return;
			}
		}
	}

	template<class C>
	std::shared_ptr<C> getComponent()
	{
		for (auto &existedComponent : m_ComponentList)
		{
			if (std::dynamic_pointer_cast<C>(existedComponent))
			{
				return std::dynamic_pointer_cast<C>(existedComponent);
			}
		}
		return nullptr;
	}
};