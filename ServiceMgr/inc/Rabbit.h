#pragma once
#include <Node2DImage.h>
#include <NodeText2D.h>
#include <IEntity.h>
#include <base_define.h>

class Rabbit
{
private:
	Node2DImage*	m_pRabbit = nullptr;
	NodeText2D*		m_pRextStatus = nullptr;
	IEntity			m_RabbitEntity;
public:
	Rabbit();
	~Rabbit();

	void InitResource();
	void AcquireResource();
	void onKeyInputEvent(KeyInputArgumentType& arg);

	void setVisible(bool visible);
	FREE_LIST_MEMORY_MANAGER(Rabbit)
};

typedef std::shared_ptr<Rabbit> RabbitPtr;