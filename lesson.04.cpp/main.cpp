#include <iostream>

#include "../lesson.03.cpp/UObject.h"

#include "ThreadableAdapter.h"

#include "StartThread.h"
#include "QueueCommand.h"
#include "SoftStopThread.h"
#include "HardStopThread.h"
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

	StartThread::Ptr pStartThread = std::make_shared<StartThread>(pThreadable);

	Move::Ptr pMove = std::make_shared<Move>(pMoveable);
	Wait::Ptr pWait = std::make_shared<Wait>(100);

	QueueCommand::Ptr pQueueWait = std::make_shared<QueueCommand>(pThreadable, pWait);
	QueueCommand::Ptr pQueueMove = std::make_shared<QueueCommand>(pThreadable, pMove);

	SoftStopThread::Ptr pSoftStopThread = std::make_shared<SoftStopThread>(pThreadable);
	HardStopThread::Ptr pHardStopThread = std::make_shared<HardStopThread>(pThreadable);

	std::cout << "Postion #1: " << pMoveable->GetPosition() << std::endl;

	pStartThread->Execute();
	pQueueMove->Execute();
	pQueueWait->Execute();
	pQueueWait->Execute();
	pQueueMove->Execute();
	pWait->Execute();
	pSoftStopThread->Execute();

	std::cout << "Postion #2: " << pMoveable->GetPosition() << std::endl;

	pStartThread->Execute();
	pQueueMove->Execute();
	pQueueWait->Execute();
	pQueueWait->Execute();
	pQueueMove->Execute();
	pWait->Execute();
	pHardStopThread->Execute();

	std::cout << "Postion #3: " << pMoveable->GetPosition() << std::endl;
}