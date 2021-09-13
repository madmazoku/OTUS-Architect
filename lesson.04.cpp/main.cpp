#include <iostream>

#include "../lesson.03.cpp/UObject.h"

#include "ThreadableAdapter.h"

#include "StartThread.h"
#include "QueueCommand.h"
#include "SoftStopThread.h"
#include "HardStopThread.h"
#include "JoinThread.h"
#include "Wait.h"

#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.03.cpp/Move.h"

#include "../lesson.02.cpp/Vector.h"

int main()
{
	UObject::Ptr pTank = std::make_shared<UObject>();
	pTank->SetProperty("position", Vector({ 1,2 }));
	pTank->SetProperty("velocity", Vector({ 2,1 }));

	IThreadable::Ptr pThreadable = std::make_shared<ThreadableAdapter>(pTank);
	IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pTank);

	IExecuteable::Ptr pStartThread = std::make_shared<StartThread>(pThreadable);
	IExecuteable::Ptr pSoftStopThread = std::make_shared<SoftStopThread>(pThreadable);
	IExecuteable::Ptr pHardStopThread = std::make_shared<HardStopThread>(pThreadable);
	IExecuteable::Ptr pJoinThread = std::make_shared<JoinThread>(pThreadable);
	IExecuteable::Ptr pMove = std::make_shared<Move>(pMoveable);
	IExecuteable::Ptr pWait = std::make_shared<Wait>(100);

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