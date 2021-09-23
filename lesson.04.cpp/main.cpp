#include <iostream>

#include "../lesson.03.cpp/UObject.h"

#include "ThreadableAdapter.h"

#include "StartThread.h"
#include "QueueCommand.h"
#include "SoftStopThread.h"
#include "HardStopThread.h"
#include "JoinThread.h"
#include "Generic.h"

#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"

#include "../lesson.02.cpp/Vector.h"

#include "QueueCommand.h"
#include "IQueue.h"
#include "LockFreeRingArray.h"
#include "TwoLockQueue.h"
#include "QueueThread.h"
#include "LockQueue.h"
#include "TwoSpinLockQueue.h"
#include <sstream>

void queueThreadTest() {
	//IQueue<std::string>::Ptr pQueue = std::make_shared<TwoLockQueue<std::string>>();
	//IQueue<std::string>::Ptr pQueue = std::make_shared<LockFreeRingArray<std::string>>();
	//IQueue<std::string>::Ptr pQueue = std::make_shared<LockQueue<std::string>>();
	IQueue<std::string>::Ptr pQueue = std::make_shared<TwoSpinLockQueue<std::string>>();
	QueueThread<std::string> queueThread(pQueue);
	queueThread.Run([](std::string str) {
		std::cout << "Thread: " << str << std::endl;
		});
	queueThread.Start();

	std::thread threads[5];
	for (int i = 0; i < 5; ++i)
		threads[i] = std::thread([i, &queueThread] {
		for (int j = 0; j < 5; ++j) {
			std::stringstream ss;
			ss << "thid: " << i << "; try: " << j;
			queueThread.Put(ss.str());
			std::this_thread::sleep_for(std::chrono::milliseconds(i * j));
		}
			});

	queueThread.Put("A");
	std::cout << "Fence Start" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Fence End" << std::endl;
	queueThread.Put("B");
	queueThread.Put("C");

	queueThread.SoftStop();
	queueThread.Join();

	for (int i = 0; i < 5; ++i)
		threads[i].join();
}

void commandTest()
{
	UObject::Ptr pTank = std::make_shared<UObject>();
	pTank->SetProperty("position", Vector({ 1,2 }));
	pTank->SetProperty("velocity", Vector({ 2,1 }));

	IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pTank);
	IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pTank);

	IQueue<IExecuteable::Ptr>::Ptr pQueue = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	//IQueue<IExecuteable::Ptr>::Ptr pQueue = std::make_shared<LockFreeRingArray<IExecuteable::Ptr>>();

	IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable, pQueue);
	IExecuteable::Ptr pSoftStopThread = std::make_shared<SoftStopThread>(pThreadable);
	IExecuteable::Ptr pHardStopThread = std::make_shared<HardStopThread>(pThreadable);
	IExecuteable::Ptr pJoinThread = std::make_shared<JoinThread>(pThreadable);
	IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);
	IExecuteable::Ptr pWait = std::make_shared<Generic>(pTank, [&](UObject::Ptr) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });

	IExecuteable::Ptr pQueueWait = std::make_shared<QueueCommand>(pThreadable, pWait);
	IExecuteable::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
	IExecuteable::Ptr pQueueSoftStop = std::make_shared<QueueCommand>(pThreadable, pSoftStopThread);
	IExecuteable::Ptr pQueueHardStop = std::make_shared<QueueCommand>(pThreadable, pHardStopThread);

	std::cout << "Postion #1: " << pMoveable->GetPosition() << std::endl;

	pStartThread->Execute();
	pQueueMove->Execute();
	pQueueSoftStop->Execute();
	pQueueMove->Execute();
	pWait->Execute();
	pJoinThread->Execute();

	std::cout << "Postion #2: " << pMoveable->GetPosition() << std::endl;

	pStartThread->Execute();
	pQueueMove->Execute();
	pQueueHardStop->Execute();
	pQueueMove->Execute();
	pWait->Execute();
	pJoinThread->Execute();

	std::cout << "Postion #3: " << pMoveable->GetPosition() << std::endl;
}

int main() {
	queueThreadTest();
	commandTest();
}