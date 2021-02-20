#pragma once
/**
 * The base State class declares methods that all Concrete State should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */

class ContextBase;

class IState {
	/**
	 * @var Context
	 */
protected:
	ContextBase *context_;

public:
	IState();
	virtual ~IState();
	virtual void Handle() = 0;
	void set_context(ContextBase *context);
};


/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */
class ContextBase {
	/**
	 * @var State A reference to the current state of the Context.
	 */
private:
	IState *state_;

public:
	ContextBase(IState *state);
	~ContextBase();

	/**
	 * The Context allows changing the State object at runtime.
	 */
	void TransitionTo(IState *state);

};


/**
* Concrete States implement various behaviors, associated with a state of the
* Context.
*/

class ConcreteStateA : public IState {
public:
	void Handle() override
	{
	//	std::cout << "ConcreteStateA handles request2.\n";
	}
};

class ConcreteStateB : public IState {
public:
	void Handle() override
	{
		//std::cout << "ConcreteStateB handles request2.\n";
		//std::cout << "ConcreteStateB wants to change the state of the context.\n";
		this->context_->TransitionTo(new ConcreteStateA);
	}
};