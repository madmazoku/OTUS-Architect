#include "pch.h"

#include "../lesson.14.cpp/Fabric.h"
#include "../lesson.28.cpp/LambdaCommand.h"

Fabric::Ptr InitializeIoC();

Fabric::Ptr IoC = InitializeIoC();

Fabric::Ptr InitializeIoC() {
	Fabric::Ptr pFabric = std::make_shared<Fabric>();

	Fabric::Register(
		pFabric,
		"Default.Scope.Enter",
		[](Fabric::Args args) {
			IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([] {
				Fabric::Ptr pCurrentScope = IoC;
				Fabric::Ptr pNewScope = IoC->Resolve<Fabric::Ptr>("Default.Scope.New", { IoC });
				Fabric::Register(
					pNewScope,
					"Default.Scope.Leave",
					[pCurrentScope](Fabric::Args args) {
						IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([pCurrentScope] {
							IoC = pCurrentScope;
							});
						return pCommand;
					}
				);
				IoC = pNewScope;
				});
			return pCommand;
		}
	);

	return pFabric;
}

class ScopeGuard {
public:
	ScopeGuard() {
		IoC->Resolve<IExecuteable::Ptr>("Default.Scope.Enter")->Execute();
	}

	~ScopeGuard() {
		IoC->Resolve<IExecuteable::Ptr>("Default.Scope.Leave")->Execute();
	}
};

#include "../lesson.28.cpp/Tokenizer.h"
#include "../lesson.03.cpp/UObject.h"
#include "../lesson.28.cpp/IdentifyableAdapter.h"
#include "../lesson.28.cpp/OwnableAdapter.h"
#include "../lesson.28.cpp/SetDirectionByConstant.h"
#include "../lesson.28.cpp/SetPlayerByRange.h"
#include "../lesson.28.cpp/SetPositionByOffset.h"
#include "../lesson.28.cpp/InterpreterSetupCommand.h"
#include "../lesson.28.cpp/TankIntitializerSetupCommand.h"

class NopeCommand : public IExecuteable {
public:
	virtual void Execute() override {}

	static void RegisterAs(std::string command) {
		Fabric::Register(
			IoC,
			command,
			[](Fabric::Args args) {
				IExecuteable::Ptr pCommand = std::make_shared<NopeCommand>();
				return pCommand;
			}
		);
	}
};

TEST(TestTokenizer, EmptyString) {
	Tokenizer tokenizer("");

	EXPECT_TRUE(tokenizer.IsEndOfString());
	EXPECT_THROW({ tokenizer.Next(); }, std::invalid_argument);
}

TEST(TestTokenizer, OneTokenString) {
	Tokenizer tokenizer("abc");

	EXPECT_TRUE(!tokenizer.IsEndOfString());
	EXPECT_EQ(tokenizer.Next(), "abc");
	EXPECT_TRUE(tokenizer.IsEndOfString());
	EXPECT_THROW({ tokenizer.Next(); }, std::invalid_argument);
}

TEST(TestTokenizer, OneTokenStringWithSpaces) {
	Tokenizer tokenizer(" \tabc \t\n\r");

	EXPECT_TRUE(!tokenizer.IsEndOfString());
	EXPECT_EQ(tokenizer.Next(), "abc");
	EXPECT_TRUE(tokenizer.IsEndOfString());
	EXPECT_THROW({ tokenizer.Next(); }, std::invalid_argument);
}

TEST(TestTokenizer, TwoTokenStringWithSpaces) {
	Tokenizer tokenizer(" \tabc \t\n\rdef\r ");

	EXPECT_TRUE(!tokenizer.IsEndOfString());
	EXPECT_EQ(tokenizer.Next(), "abc");
	EXPECT_EQ(tokenizer.Next(), "def");
	EXPECT_TRUE(tokenizer.IsEndOfString());
	EXPECT_THROW({ tokenizer.Next(); }, std::invalid_argument);
}

TEST(TestAdapter, IdentifyableAdapter) {
	UObject::Ptr pUObject = std::make_shared<UObject>();
	IIdentifyable::Ptr pIdentifyable = std::make_shared<IdentifyableAdapter>(pUObject);

	pIdentifyable->SetIndex(123);

	EXPECT_EQ(pIdentifyable->GetIndex(), 123);
}

TEST(TestAdapter, OwnableAdapter) {
	UObject::Ptr pUObject = std::make_shared<UObject>();
	IOwnable::Ptr pOwnable = std::make_shared<OwnableAdapter>(pUObject);

	pOwnable->SetOwnerIndex(123);

	EXPECT_EQ(pOwnable->GetOwnerIndex(), 123);
}

TEST(TestStrategy, SetDirectionByConstant) {
	ScopeGuard sg;

	size_t countSetDirectionCalls = 0;
	double dirX;
	double dirY;
	Fabric::Register(
		IoC,
		"Tank.SetDirection",
		[&countSetDirectionCalls, &dirX, &dirY](Fabric::Args args) {
			UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
			EXPECT_EQ(pUObject, nullptr);
			dirX = std::stod(Fabric::GetRequiredArg<std::string>(args, 1, "invalid direction x"));
			dirY = std::stod(Fabric::GetRequiredArg<std::string>(args, 2, "invalid direction y"));
			++countSetDirectionCalls;
			IExecuteable::Ptr pCommand = std::make_shared<NopeCommand>();
			return pCommand;
		}
	);

	ISetDirection::Ptr pSetDirectionByConstant = std::make_shared<SetDirectionByConstant>(Vector{ 2,3 });

	pSetDirectionByConstant->SetDirection(nullptr);
	EXPECT_EQ(countSetDirectionCalls, 1);
	EXPECT_DOUBLE_EQ(dirX, 2);
	EXPECT_DOUBLE_EQ(dirY, 3);

	pSetDirectionByConstant->SetDirection(nullptr);
	EXPECT_EQ(countSetDirectionCalls, 2);
	EXPECT_DOUBLE_EQ(dirX, 2);
	EXPECT_DOUBLE_EQ(dirY, 3);
}

TEST(TestStrategy, SetPlayerByRange) {
	ScopeGuard sg;

	size_t countSetPlayerCalls = 0;
	size_t playerIdx;
	Fabric::Register(
		IoC,
		"Tank.SetPlayer",
		[&countSetPlayerCalls, &playerIdx](Fabric::Args args) {
			UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
			EXPECT_EQ(pUObject, nullptr);
			playerIdx = std::stoull(Fabric::GetRequiredArg<std::string>(args, 1, "invalid player"));
			++countSetPlayerCalls;
			IExecuteable::Ptr pCommand = std::make_shared<NopeCommand>();
			return pCommand;
		}
	);

	ISetPlayer::Ptr pSetPlayerByRange = std::make_shared<SetPlayerByRange>(2);

	pSetPlayerByRange->SetPlayer(nullptr);
	EXPECT_EQ(countSetPlayerCalls, 1);
	EXPECT_EQ(playerIdx, 0);

	pSetPlayerByRange->SetPlayer(nullptr);
	EXPECT_EQ(countSetPlayerCalls, 2);
	EXPECT_EQ(playerIdx, 0);

	pSetPlayerByRange->SetPlayer(nullptr);
	EXPECT_EQ(countSetPlayerCalls, 3);
	EXPECT_EQ(playerIdx, 1);

	pSetPlayerByRange->SetPlayer(nullptr);
	EXPECT_EQ(countSetPlayerCalls, 4);
	EXPECT_EQ(playerIdx, 1);
}

class TestGetPlayer : public IGetPlayer {
public:
	size_t m_countGetPlayerCalls;
	size_t m_playerIdx;

	TestGetPlayer() : m_countGetPlayerCalls(0), m_playerIdx(0) {}

	void SetPlayerIdx(size_t playerIdx) {
		m_playerIdx = playerIdx;
	}
	size_t GetCountGetPlayerCalls() {
		return m_countGetPlayerCalls;
	}

	virtual size_t Get(UObject::Ptr pUObject) override {
		++m_countGetPlayerCalls;
		EXPECT_EQ(pUObject, nullptr);
		return m_playerIdx;
	}
};

TEST(TestStrategy, SetPositionByOffset) {
	ScopeGuard sg;

	std::shared_ptr<TestGetPlayer> pGetPlayer = std::make_shared<TestGetPlayer>();
	Fabric::Register(
		IoC,
		"Tank.GetterPlayer",
		[pGetPlayer](Fabric::Args args) {
			IGetPlayer::Ptr pGetter = pGetPlayer;
			return pGetter;
		}
	);
	size_t countSetPositionCalls = 0;
	double posX;
	double posY;
	Fabric::Register(
		IoC,
		"Tank.SetPosition",
		[&countSetPositionCalls, &posX, &posY](Fabric::Args args) {
			++countSetPositionCalls;
			UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
			EXPECT_EQ(pUObject, nullptr);
			posX = std::stod(Fabric::GetRequiredArg<std::string>(args, 1, "invalid position x"));
			posY = std::stod(Fabric::GetRequiredArg<std::string>(args, 2, "invalid position y"));
			IExecuteable::Ptr pCommand = std::make_shared<NopeCommand>();
			return pCommand;
		}
	);

	ISetPosition::Ptr pSetPositionByOffset = std::make_shared<SetPositionByOffset>(Vector{ -1, 0 }, Vector{ 1, 0 }, Vector{ 0, 1 });

	pGetPlayer->SetPlayerIdx(0);
	pSetPositionByOffset->SetPosition(nullptr);
	EXPECT_EQ(pGetPlayer->GetCountGetPlayerCalls(), 1);
	EXPECT_EQ(countSetPositionCalls, 1);
	EXPECT_DOUBLE_EQ(posX, -1);
	EXPECT_DOUBLE_EQ(posY, 0);

	pGetPlayer->SetPlayerIdx(1);
	pSetPositionByOffset->SetPosition(nullptr);
	EXPECT_EQ(pGetPlayer->GetCountGetPlayerCalls(), 2);
	EXPECT_EQ(countSetPositionCalls, 2);
	EXPECT_DOUBLE_EQ(posX, 1);
	EXPECT_DOUBLE_EQ(posY, 0);

	pGetPlayer->SetPlayerIdx(0);
	pSetPositionByOffset->SetPosition(nullptr);
	EXPECT_EQ(pGetPlayer->GetCountGetPlayerCalls(), 3);
	EXPECT_EQ(countSetPositionCalls, 3);
	EXPECT_DOUBLE_EQ(posX, -1);
	EXPECT_DOUBLE_EQ(posY, 1);

	pGetPlayer->SetPlayerIdx(1);
	pSetPositionByOffset->SetPosition(nullptr);
	EXPECT_EQ(pGetPlayer->GetCountGetPlayerCalls(), 4);
	EXPECT_EQ(countSetPositionCalls, 4);
	EXPECT_DOUBLE_EQ(posX, 1);
	EXPECT_DOUBLE_EQ(posY, 1);
}

TEST(TestCommand, LambdaCommand) {
	bool called = false;
	IExecuteable::Ptr pLambdaCommand = std::make_shared<LambdaCommand>([&called] {called = true; });
	pLambdaCommand->Execute();
	EXPECT_TRUE(called);
}

TEST(TestCommand, InterpreterSetupCommand_Register) {
	ScopeGuard sg;

	InterpreterSetupCommand().Execute();
	EXPECT_THROW({ NopeCommand::RegisterAs("Interpreter.Interpret"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Interpreter.SelectStrategies"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Interpreter.CreateTank"); }, std::invalid_argument);
}

TEST(TestCommand, TankIntitializerSetupCommand_Register) {
	ScopeGuard sg;

	TankIntitializerSetupCommand().Execute();
	EXPECT_THROW({ NopeCommand::RegisterAs("Globals.Tanks"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("TankInitializer.Select.SetDirection.ByConstant"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("TankInitializer.Select.SetPlayer.ByRange"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("TankInitializer.Select.SetPosition.ByOffset"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("TankInitializer.Create.Tank"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.GetIndex"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.SetPlayer"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.GetPlayer"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.SetPosition"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.GetPosition"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.SetDirection"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.GetDirection"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.SetFuelLevel"); }, std::invalid_argument);
	EXPECT_THROW({ NopeCommand::RegisterAs("Tank.GetFuelLevel"); }, std::invalid_argument);
}

TEST(TestCommand, TankIntitializerSetupCommand_CreateTank) {
	ScopeGuard sg;

	TankIntitializerSetupCommand().Execute();

	ISetDirection::Ptr pSetDirectionByConstant = std::make_shared<SetDirectionByConstant>(Vector{ 0, 1 });
	ISetPlayer::Ptr pSetPlayerByRange = std::make_shared<SetPlayerByRange>(2);
	ISetPosition::Ptr pSetPositionByOffset = std::make_shared<SetPositionByOffset>(Vector{ -1, 0 }, Vector{ 1, 0 }, Vector{ 0, 1 });

	Fabric::Register(
		IoC,
		"Globals.SetDirection",
		[pSetDirectionByConstant](Fabric::Args args) { return pSetDirectionByConstant; }
	);
	Fabric::Register(
		IoC,
		"Globals.SetPlayer",
		[pSetPlayerByRange](Fabric::Args args) { return pSetPlayerByRange; }
	);
	Fabric::Register(
		IoC,
		"Globals.SetPosition",
		[pSetPositionByOffset](Fabric::Args args) { return pSetPositionByOffset; }
	);

	auto CheckCreateTank = [](size_t idx, size_t playerIdx, Vector pos, Vector dir) {
		UObject::Ptr pUObject = IoC->Resolve<UObject::Ptr>("TankInitializer.Create.Tank");

		IIdentifyable::Ptr pIdentifyable = std::make_shared<IdentifyableAdapter>(pUObject);
		IOwnable::Ptr pOwnable = std::make_shared<OwnableAdapter>(pUObject);
		IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);
		IDirectionRotateable::Ptr pDirectionRotateable = std::make_shared<DirectionRotateableAdapter>(pUObject);

		EXPECT_EQ(pIdentifyable->GetIndex(), idx);
		EXPECT_EQ(pOwnable->GetOwnerIndex(), playerIdx);

		Vector gotPos = pMoveable->GetPosition();
		EXPECT_DOUBLE_EQ(gotPos.m_x, pos.m_x);
		EXPECT_DOUBLE_EQ(gotPos.m_y, pos.m_y);

		Vector gotDir = pDirectionRotateable->GetDirection();
		EXPECT_DOUBLE_EQ(gotDir.m_x, dir.m_x);
		EXPECT_DOUBLE_EQ(gotDir.m_y, dir.m_y);
	};

	CheckCreateTank(0, 0, Vector{ -1, 0 }, Vector{ 0, 1 });
	CheckCreateTank(1, 0, Vector{ -1, 1 }, Vector{ 0, 1 });
	CheckCreateTank(2, 1, Vector{ 1, 0 }, Vector{ 0, 1 });
	CheckCreateTank(3, 1, Vector{ 1, 1 }, Vector{ 0, 1 });
}

TEST(TestCommand, InterpreterSetupCommand_Interpret) {
	ScopeGuard sg;

	InterpreterSetupCommand().Execute();

	bool called = false;
	Fabric::Register(
		IoC,
		"Interpreter.TestCall",
		[&called](Fabric::Args args) {
			called = true;
			std::shared_ptr<std::istream> pInputStream = Fabric::GetRequiredArg<std::shared_ptr<std::istream>>(args, 0, "invalid input stream");
			EXPECT_TRUE(pInputStream != nullptr);
			IExecuteable::Ptr pCommand = std::make_shared<NopeCommand>();
			return pCommand;
		}
	);

	std::shared_ptr<std::stringstream> pStream = std::make_shared<std::stringstream>();
	std::shared_ptr<std::istream> pInputStream = pStream;

	(*pStream) << "TestCall" << std::endl;

	IoC->Resolve<IExecuteable::Ptr>("Interpreter.Interpret", { pInputStream })->Execute();

	EXPECT_TRUE(called);
}