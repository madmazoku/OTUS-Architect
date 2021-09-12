#include "pch.h"

#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"

#include "../lesson.04.cpp/ThreadableAdapter.h"
#include "../lesson.04.cpp/StartThread.h"
#include "../lesson.04.cpp/Wait.h"
#include "../lesson.04.cpp/QueueCommand.h"
#include "../lesson.04.cpp/HardStopThread.h"
#include "../lesson.04.cpp/SoftStopThread.h"

TEST(Threadable, StartThread) {
	UObject::Ptr pUObject = std::make_shared<UObject>();
	IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
	IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

	pMoveable->SetPosition({ 1,2 });
	pMoveable->SetVelocity({ 2,1 });

	StartThread::Ptr pStartThread = std::make_shared<StartThread>(pThreadable);
	Move::Ptr pMove = std::make_shared<Move>(pMoveable);
	Wait::Ptr pWait = std::make_shared<Wait>(10);

	pStartThread->Execute();

	ExecuteableQueue::Ptr pQueue = pThreadable->GetQueue();
	pQueue->Put(pMove);
	pQueue->Put(pMove);
	pQueue->Put(pMove);
	pWait->Execute();

	Vector position = pMoveable->GetPosition();

	EXPECT_DOUBLE_EQ(position.m_x, 7);
	EXPECT_DOUBLE_EQ(position.m_y, 5);

	pQueue->HardStop();
}

TEST(Threadable, HardStop) {
	UObject::Ptr pUObject = std::make_shared<UObject>();
	IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
	IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

	pMoveable->SetPosition({ 1,2 });
	pMoveable->SetVelocity({ 2,1 });

	StartThread::Ptr pStartThread = std::make_shared<StartThread>(pThreadable);
	Move::Ptr pMove = std::make_shared<Move>(pMoveable);
	Wait::Ptr pWait = std::make_shared<Wait>(10);
	HardStopThread::Ptr pHardStopThread = std::make_shared<HardStopThread>(pThreadable);

	QueueCommand::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
	QueueCommand::Ptr pQueueWait = std::make_shared<QueueCommand>(pThreadable, pWait);

	pStartThread->Execute();
	pQueueMove->Execute();
	pQueueWait->Execute();
	pQueueWait->Execute();
	pQueueMove->Execute();
	pWait->Execute();
	pHardStopThread->Execute();

	Vector position = pMoveable->GetPosition();

	EXPECT_DOUBLE_EQ(position.m_x, 3);
	EXPECT_DOUBLE_EQ(position.m_y, 3);
}

TEST(Threadable, SoftStop) {
	UObject::Ptr pUObject = std::make_shared<UObject>();
	IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pUObject);
	IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);

	pMoveable->SetPosition({ 1,2 });
	pMoveable->SetVelocity({ 2,1 });

	StartThread::Ptr pStartThread = std::make_shared<StartThread>(pThreadable);
	Move::Ptr pMove = std::make_shared<Move>(pMoveable);
	Wait::Ptr pWait = std::make_shared<Wait>(10);
	SoftStopThread::Ptr pSoftStopThread = std::make_shared<SoftStopThread>(pThreadable);

	QueueCommand::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);
	QueueCommand::Ptr pQueueWait = std::make_shared<QueueCommand>(pThreadable, pWait);

	pStartThread->Execute();
	pQueueMove->Execute();
	pQueueWait->Execute();
	pQueueWait->Execute();
	pQueueMove->Execute();
	pWait->Execute();
	pSoftStopThread->Execute();

	Vector position = pMoveable->GetPosition();

	EXPECT_DOUBLE_EQ(position.m_x, 5);
	EXPECT_DOUBLE_EQ(position.m_y, 4);
}