#pragma once//

#include <memory>
#include <mutex>
#include <functional>

#include "../lesson.03.cpp/IExecuteable.h"

// класс-состояние для очереди комманд
// при вызове Handle выполняет переданную в конструктор лямбду над
// командой и возвращает заданное в SetNextState следующее состояние.
class QueueState
{
public:
	typedef std::shared_ptr<QueueState> Ptr;
	typedef std::function<void(IExecuteable::Ptr)> Lambda;

protected:
	Lambda m_lambda;
	Ptr m_pNextState;
	std::mutex m_lock;

public:
	QueueState(Lambda lambda) : m_lambda(lambda) {}

	void SetNextState(QueueState::Ptr pNextState) {
		std::lock_guard lg(m_lock);
		m_pNextState = std::move(pNextState);
	}

	QueueState::Ptr Handle(IExecuteable::Ptr pCmd) {
		m_lambda(pCmd);
		std::lock_guard lg(m_lock);
		return m_pNextState;
	}
};
