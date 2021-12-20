#include "QueueStatefullThread.h"
#include "QueueState.h"
#include "HardStopCommand.h"
#include "MoveToCommand.h"
#include "RunCommand.h"

#include "../lesson.02.cpp/Vector.h"
#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/IMoveable.h"
#include "../lesson.03.cpp/Move.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.04.cpp/IQueue.h"
#include "../lesson.04.cpp/TwoLockQueue.h"

#include <iostream>

class DumpCommand : public IExecuteable {
protected:
	UObject::Ptr m_pUObject;
	std::string m_msg;

public:
	DumpCommand(UObject::Ptr pUObject, std::string msg) : m_pUObject(pUObject), m_msg(std::move(msg)) {}

	virtual void Execute() override {
		std::cout << std::this_thread::get_id() << ": " << m_msg;
		if (m_pUObject != nullptr) {
			IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(m_pUObject);
			std::cout << "; Position: " << pMoveable->GetPosition() << "; " << "Velocity: " << pMoveable->GetVelocity();
		}
		std::cout << std::endl;
	}
};

class Waiter {
protected:
	std::mutex m_lock;
	std::condition_variable m_cv;
	bool m_pulse = false;

public:
	Waiter() : m_pulse(false) {}

	void Wait() {
		{
			std::unique_lock<std::mutex> ul(m_lock);
			m_cv.wait(ul, [&] {return m_pulse; });
			m_pulse = false;
		}
		m_cv.notify_one();
	}

	void Pulse() {
		{
			std::lock_guard<std::mutex> lg(m_lock);
			m_pulse = true;
		}
		m_cv.notify_one();
	}
};

class WaitCommand : public IExecuteable {
protected:
	Waiter& m_waiter;
public:
	WaitCommand(Waiter& waiter) : m_waiter(waiter) {}
	virtual void Execute() override { m_waiter.Wait(); }
};

class PulseCommand : public IExecuteable {
protected:
	Waiter& m_waiter;
public:
	PulseCommand(Waiter& waiter) : m_waiter(waiter) {}
	virtual void Execute() override { m_waiter.Pulse(); }
};

int main()
{
	UObject::Ptr pObj = std::make_shared<UObject>();

	IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pObj);
	pMoveable->SetPosition({ 1,2 });
	pMoveable->SetVelocity({ 2,1 });

	IExecuteableQueue::Ptr pQueue1 = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState1 = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState1->SetNextState(pRunQueueState1);
	QueueStatefullThread::Ptr pQueueThread1 = std::make_shared<QueueStatefullThread>(pQueue1, pRunQueueState1);

	IQueue<IExecuteable::Ptr>::Ptr pQueue2 = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState2 = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState2->SetNextState(pRunQueueState2);
	QueueStatefullThread::Ptr pQueueThread2 = std::make_shared<QueueStatefullThread>(pQueue2, pRunQueueState2);

	Waiter waiter;

	pQueueThread1->Run();
	pQueueThread1->Start();

	pQueueThread2->Run();
	pQueueThread2->Start();

	pQueueThread1->Put(std::make_shared<DumpCommand>(pObj, std::string("start")));

	pQueueThread1->Put(std::make_shared<Move>(pMoveable));
	pQueueThread1->Put(std::make_shared<DumpCommand>(pObj, std::string("1st move")));

	pQueueThread1->Put(std::make_shared<MoveToCommand>(pQueueThread1, pQueueThread2));

	pQueueThread1->Put(std::make_shared<Move>(pMoveable));
	pQueueThread1->Put(std::make_shared<DumpCommand>(pObj, std::string("2nd move")));

	pQueueThread1->Put(std::make_shared<RunCommand>(pQueueThread1));
	pQueueThread1->Put(std::make_shared<PulseCommand>(waiter));
	waiter.Wait();

	pQueueThread1->Put(std::make_shared<Move>(pMoveable));
	pQueueThread1->Put(std::make_shared<DumpCommand>(pObj, std::string("3rd move")));

	pQueueThread1->Put(std::make_shared<HardStopCommand>(pQueueThread1));

	pQueueThread1->Join();

	pQueueThread2->Put(std::make_shared<DumpCommand>(pObj, std::string("end")));
	pQueueThread2->Put(std::make_shared<HardStopCommand>(pQueueThread2));

	pQueueThread2->Join();
}