#include "pch.h"

#include "../lesson.23.cpp/QueueStatefullThread.h"
#include "../lesson.23.cpp/QueueState.h"
#include "../lesson.23.cpp/HardStopCommand.h"
#include "../lesson.23.cpp/MoveToCommand.h"
#include "../lesson.23.cpp/RunCommand.h"

#include "../lesson.02.cpp/Vector.h"
#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/IMoveable.h"
#include "../lesson.03.cpp/Move.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.04.cpp/IQueue.h"
#include "../lesson.04.cpp/TwoLockQueue.h"

#include "LambdaCommand.h"
#include "Waiter.h"

TEST(QueueStatefullThread, HardStop) {
	IExecuteableQueue::Ptr pQueue = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState->SetNextState(pRunQueueState);
	QueueStatefullThread::Ptr pQueueThread = std::make_shared<QueueStatefullThread>(pQueue, pRunQueueState);

	pQueueThread->Run();
	pQueueThread->Start();

	bool bBeforeHardStop = false;
	bool bAfterHardStop = false;
	Waiter waiter;

	pQueueThread->Put(std::make_shared<LambdaCommand>([&waiter]() {waiter.Wait(); }));
	pQueueThread->Put(std::make_shared<LambdaCommand>([&bBeforeHardStop]() {bBeforeHardStop = true; }));
	pQueueThread->Put(std::make_shared<HardStopCommand>(pQueueThread));
	pQueueThread->Put(std::make_shared<LambdaCommand>([&bAfterHardStop]() {bAfterHardStop = true; }));

	EXPECT_TRUE(!bBeforeHardStop);
	EXPECT_TRUE(!bAfterHardStop);

	waiter.Pulse();

	pQueueThread->Join();

	EXPECT_TRUE(bBeforeHardStop);
	EXPECT_TRUE(!bAfterHardStop);
}

TEST(QueueStatefullThread, ReplaceRunWithMoveTo) {
	IExecuteableQueue::Ptr pQueue1 = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState1 = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState1->SetNextState(pRunQueueState1);
	QueueStatefullThread::Ptr pQueueThread1 = std::make_shared<QueueStatefullThread>(pQueue1, pRunQueueState1);

	IExecuteableQueue::Ptr pQueue2 = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState2 = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState2->SetNextState(pRunQueueState2);
	QueueStatefullThread::Ptr pQueueThread2 = std::make_shared<QueueStatefullThread>(pQueue2, pRunQueueState2);

	pQueueThread1->Run();
	pQueueThread2->Run();

	pQueueThread1->Start();
	pQueueThread2->Start();

	Waiter waiter1, waiter2;
	std::thread::id id1, id2, id3;

	pQueueThread1->Put(std::make_shared<LambdaCommand>([&id1]() {id1 = std::this_thread::get_id(); }));
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&waiter1]() {waiter1.Wait(); }));

	pQueueThread2->Put(std::make_shared<LambdaCommand>([&id2]() {id2 = std::this_thread::get_id(); }));
	pQueueThread2->Put(std::make_shared<LambdaCommand>([&waiter1]() {waiter1.Pulse(); }));

	pQueueThread1->Put(std::make_shared<MoveToCommand>(pQueueThread1, pQueueThread2));
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&id3]() {id3 = std::this_thread::get_id(); }));

	pQueueThread1->Put(std::make_shared<HardStopCommand>(pQueueThread1));
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&waiter2]() {waiter2.Pulse(); }));

	waiter2.Wait();

	pQueueThread1->Put(std::make_shared<LambdaCommand>([]() {})); // need NOOP here, to initiate state switch
	pQueueThread2->Put(std::make_shared<HardStopCommand>(pQueueThread2));

	pQueueThread1->Join();
	pQueueThread2->Join();

	EXPECT_EQ(id2, id3);
}

TEST(QueueStatefullThread, ReplaceMoveToWithRun) {
	IExecuteableQueue::Ptr pQueue1 = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState1 = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState1->SetNextState(pRunQueueState1);
	QueueStatefullThread::Ptr pQueueThread1 = std::make_shared<QueueStatefullThread>(pQueue1, pRunQueueState1);

	IExecuteableQueue::Ptr pQueue2 = std::make_shared<TwoLockQueue<IExecuteable::Ptr>>();
	QueueState::Ptr pRunQueueState2 = std::make_shared<QueueState>([](IExecuteable::Ptr pCmd) { pCmd->Execute(); });
	pRunQueueState2->SetNextState(pRunQueueState2);
	QueueStatefullThread::Ptr pQueueThread2 = std::make_shared<QueueStatefullThread>(pQueue2, pRunQueueState2);

	pQueueThread1->Run();
	pQueueThread2->Run();

	pQueueThread1->Start();
	pQueueThread2->Start();

	Waiter waiter1, waiter2, waiter3;
	std::thread::id id1, id2, id3;

	pQueueThread1->Put(std::make_shared<LambdaCommand>([&id1]() {id1 = std::this_thread::get_id(); }));
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&waiter1]() {waiter1.Wait(); }));

	pQueueThread2->Put(std::make_shared<LambdaCommand>([&id2]() {id2 = std::this_thread::get_id(); }));
	pQueueThread2->Put(std::make_shared<LambdaCommand>([&waiter1]() {waiter1.Pulse(); }));

	pQueueThread1->Put(std::make_shared<MoveToCommand>(pQueueThread1, pQueueThread2));
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&waiter2]() {waiter2.Wait(); }));
	pQueueThread1->Put(std::make_shared<RunCommand>(pQueueThread1));
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&waiter3]() {waiter3.Pulse(); }));

	waiter2.Pulse();
	waiter3.Wait();

	pQueueThread1->Put(std::make_shared<LambdaCommand>([&id3]() {})); // need NOOP here, to initiate state switch
	pQueueThread1->Put(std::make_shared<LambdaCommand>([&id3]() {id3 = std::this_thread::get_id(); }));

	pQueueThread1->Put(std::make_shared<HardStopCommand>(pQueueThread1));
	pQueueThread2->Put(std::make_shared<HardStopCommand>(pQueueThread2));

	pQueueThread1->Join();
	pQueueThread2->Join();

	EXPECT_EQ(id1, id3);
}