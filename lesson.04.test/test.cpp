#include "pch.h"

#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"

#include "../lesson.04.cpp/ThreadableAdapter.h"
#include "../lesson.04.cpp/StartThread.h"
#include "../lesson.04.cpp/QueueCommand.h"
#include "../lesson.04.cpp/HardStopThread.h"
#include "../lesson.04.cpp/SoftStopThread.h"
#include "../lesson.04.cpp/JoinThread.h"
#include "../lesson.04.cpp/Generic.h"
#include "../lesson.04.cpp/LockQueue.h"
#include "../lesson.04.cpp/TwoLockQueue.h"
#include "../lesson.04.cpp/LockFreeRingArray.h"

TEST(Threadable, StartThread) {
	std::list<IQueue<IExecuteable::Ptr>::Ptr> tests;
	tests.push_back(std::make_shared<LockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<TwoLockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<LockFreeRingArray<IExecuteable::Ptr>>());

	for (auto pQueue : tests) {
		UObject::Ptr pUObject = std::make_shared<UObject>();
		IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
		IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

		pMoveable->SetPosition({ 1,2 });
		pMoveable->SetVelocity({ 2,1 });

		IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable, pQueue);
		IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);

		pStartThread->Execute();

		ExecuteableQueueThread::Ptr pQueue = pThreadable->GetQueueThread();
		pQueue->Put(pMove);
		pQueue->SoftStop();
		pQueue->Join();

		Vector position = pMoveable->GetPosition();

		EXPECT_DOUBLE_EQ(position.m_x, 3);
		EXPECT_DOUBLE_EQ(position.m_y, 3);
	}
}

TEST(Threadable, HardStopInner) {
	std::list<IQueue<IExecuteable::Ptr>::Ptr> tests;
	tests.push_back(std::make_shared<LockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<TwoLockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<LockFreeRingArray<IExecuteable::Ptr>>());

	for (auto pQueue : tests) {
		UObject::Ptr pUObject = std::make_shared<UObject>();
		IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
		IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

		pMoveable->SetPosition({ 1,2 });
		pMoveable->SetVelocity({ 2,1 });

		IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable, pQueue);
		IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);
		IExecuteable::Ptr pHardStopThread = std::make_shared<HardStopThread>(pThreadable);
		IExecuteable::Ptr pJoinThread = std::make_shared<JoinThread>(pThreadable);

		IExecuteable::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
		IExecuteable::Ptr pQueueHardStopThread = std::make_shared<QueueCommand>(pThreadable, pHardStopThread);

		pStartThread->Execute();
		pQueueMove->Execute();
		pQueueHardStopThread->Execute();
		pQueueMove->Execute();
		pJoinThread->Execute();

		Vector position = pMoveable->GetPosition();

		EXPECT_DOUBLE_EQ(position.m_x, 3);
		EXPECT_DOUBLE_EQ(position.m_y, 3);
	}
}

TEST(Threadable, HardStopOuter) {
	std::list<IQueue<IExecuteable::Ptr>::Ptr> tests;
	tests.push_back(std::make_shared<LockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<TwoLockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<LockFreeRingArray<IExecuteable::Ptr>>());

	for (auto pQueue : tests) {
		UObject::Ptr pUObject = std::make_shared<UObject>();
		IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
		IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

		std::mutex lock;
		std::condition_variable cv;
		bool pulse = false;

		pMoveable->SetPosition({ 1,2 });
		pMoveable->SetVelocity({ 2,1 });

		IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable, pQueue);
		IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);
		IExecuteable::Ptr pHardStopThread = std::make_shared<HardStopThread>(pThreadable);
		IExecuteable::Ptr pJoinThread = std::make_shared<JoinThread>(pThreadable);
		IExecuteable::Ptr pFence = std::make_shared<Generic>(pUObject, [&](UObject::Ptr) {
			{
				std::unique_lock<std::mutex> ul(lock);
				cv.wait(ul, [&] {return pulse; });
				pulse = false;
			}
			cv.notify_one();
			{
				std::unique_lock<std::mutex> ul(lock);
				cv.wait(ul, [&] {return pulse; });
			}
			});

		IExecuteable::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
		IExecuteable::Ptr pQueueFence = std::make_shared<QueueCommand>(pThreadable, pFence);

		pStartThread->Execute();
		pQueueMove->Execute();
		pQueueFence->Execute();
		pQueueMove->Execute();

		{
			std::lock_guard<std::mutex> lg(lock);
			pulse = true;
		}
		cv.notify_one();
		{
			std::unique_lock<std::mutex> ul(lock);
			cv.wait(ul, [&] {return !pulse; });
			pulse = true;
			pHardStopThread->Execute();
		}
		cv.notify_one();

		pJoinThread->Execute();

		Vector position = pMoveable->GetPosition();

		EXPECT_DOUBLE_EQ(position.m_x, 3);
		EXPECT_DOUBLE_EQ(position.m_y, 3);
	}
}

TEST(Threadable, SoftStopInner) {
	std::list<IQueue<IExecuteable::Ptr>::Ptr> tests;
	tests.push_back(std::make_shared<LockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<TwoLockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<LockFreeRingArray<IExecuteable::Ptr>>());

	for (auto pQueue : tests) {
		UObject::Ptr pUObject = std::make_shared<UObject>();
		IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
		IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

		pMoveable->SetPosition({ 1,2 });
		pMoveable->SetVelocity({ 2,1 });

		IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable, pQueue);
		IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);
		IExecuteable::Ptr pSoftStopThread = std::make_shared<SoftStopThread>(pThreadable);
		IExecuteable::Ptr pJoinThread = std::make_shared<JoinThread>(pThreadable);

		IExecuteable::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
		IExecuteable::Ptr pQueueHardStopThread = std::make_shared<QueueCommand>(pThreadable, pSoftStopThread);

		pStartThread->Execute();
		pQueueMove->Execute();
		pQueueHardStopThread->Execute();
		pQueueMove->Execute();
		pJoinThread->Execute();

		Vector position = pMoveable->GetPosition();

		EXPECT_DOUBLE_EQ(position.m_x, 5);
		EXPECT_DOUBLE_EQ(position.m_y, 4);
	}
}

TEST(Threadable, SoftStopOuter) {
	std::list<IQueue<IExecuteable::Ptr>::Ptr> tests;
	tests.push_back(std::make_shared<LockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<TwoLockQueue<IExecuteable::Ptr>>());
	tests.push_back(std::make_shared<LockFreeRingArray<IExecuteable::Ptr>>());

	for (auto pQueue : tests) {
		UObject::Ptr pUObject = std::make_shared<UObject>();
		IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
		IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

		std::mutex lock;
		std::condition_variable cv;
		bool pulse = false;

		pMoveable->SetPosition({ 1,2 });
		pMoveable->SetVelocity({ 2,1 });

		IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable, pQueue);
		IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);
		IExecuteable::Ptr pSoftStopThread = std::make_shared<SoftStopThread>(pThreadable);
		IExecuteable::Ptr pJoinThread = std::make_shared<JoinThread>(pThreadable);
		IExecuteable::Ptr pFence = std::make_shared<Generic>(pUObject, [&](UObject::Ptr) {
			{
				std::unique_lock<std::mutex> ul(lock);
				cv.wait(ul, [&] {return pulse; });
				pulse = false;
			}
			cv.notify_one();
			{
				std::unique_lock<std::mutex> ul(lock);
				cv.wait(ul, [&] {return pulse; });
			}
			});

		IExecuteable::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
		IExecuteable::Ptr pQueueFence = std::make_shared<QueueCommand>(pThreadable, pFence);

		pStartThread->Execute();
		pQueueMove->Execute();
		pQueueFence->Execute();
		pQueueMove->Execute();

		{
			std::lock_guard<std::mutex> lg(lock);
			pulse = true;
		}
		cv.notify_one();
		{
			std::unique_lock<std::mutex> ul(lock);
			cv.wait(ul, [&] {return !pulse; });
			pulse = true;
			pSoftStopThread->Execute();
		}
		cv.notify_one();

		pJoinThread->Execute();

		Vector position = pMoveable->GetPosition();

		EXPECT_DOUBLE_EQ(position.m_x, 5);
		EXPECT_DOUBLE_EQ(position.m_y, 4);
	}
}