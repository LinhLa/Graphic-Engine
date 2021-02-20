#include "stdafx.h"
#include "state.h"
#include <iostream>
#include <typeinfo>

IState::IState() {}
IState::~IState() {}

void IState::set_context(ContextBase *context)
{
	this->context_ = context;
}

ContextBase::ContextBase(IState *state) : state_(nullptr)
{
	this->TransitionTo(state);
}

ContextBase::~ContextBase()
{
	if (state_)
		delete state_;
}

/**
* The Context allows changing the State object at runtime.
*/
void ContextBase::TransitionTo(IState *state)
{
	if (this->state_ != nullptr)
		delete this->state_;
	this->state_ = state;
	this->state_->set_context(this);
}
