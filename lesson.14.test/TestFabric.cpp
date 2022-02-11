#include "pch.h"

#include "../lesson.14.cpp/Fabric.h"
#include "../lesson.03.cpp/IExecuteable.h"

TEST(Fabric, Register) {
	Fabric::Ptr pFabric = std::make_shared<Fabric>();
	pFabric->Resolve<IExecuteable::Ptr>(
		"Default.Register",
		{
			pFabric,
			std::string("Test.Func"),
			(Fabric::Lambda)[](Fabric::Args args) { return std::any(-std::any_cast<int>(args[0])); }
		}
	)->Execute();
	EXPECT_EQ(pFabric->Resolve<int>("Test.Func", { 123 }), -123);
}

TEST(Fabric, ScopeKnownRegister) {
	Fabric::Ptr pFabricParent = std::make_shared<Fabric>();
	pFabricParent->Resolve<IExecuteable::Ptr>(
		"Default.Register",
		{
			pFabricParent,
			std::string("Test.Func"),
			(Fabric::Lambda)[](Fabric::Args args) { return std::any(-std::any_cast<int>(args[0])); }
		}
	)->Execute();

	Fabric::Ptr pFabric = pFabricParent->Resolve< Fabric::Ptr>("Default.Scope.New", { pFabricParent });
	pFabric->Resolve<IExecuteable::Ptr>(
		"Default.Register",
		{
			pFabric,
			std::string("Test.Func"),
			(Fabric::Lambda)[](Fabric::Args args) { return std::any(-2 * std::any_cast<int>(args[0])); }
		}
	)->Execute();

	EXPECT_EQ(pFabric->Resolve<int>("Test.Func", { 123 }), -246);
}

TEST(Fabric, ScopeParentCall) {
	Fabric::Ptr pFabricParent = std::make_shared<Fabric>();
	pFabricParent->Resolve<IExecuteable::Ptr>(
		"Default.Register",
		{
			pFabricParent,
			std::string("Test.Func1"),
			(Fabric::Lambda)[](Fabric::Args args) { return std::any(-std::any_cast<int>(args[0])); }
		}
	)->Execute();

	Fabric::Ptr pFabric = pFabricParent->Resolve< Fabric::Ptr>("Default.Scope.New", { pFabricParent });
	pFabric->Resolve<IExecuteable::Ptr>(
		"Default.Register",
		{
			pFabric,
			std::string("Test.Func2"),
			(Fabric::Lambda)[](Fabric::Args args) { return std::any(-2 * std::any_cast<int>(args[0])); }
		}
	)->Execute();

	EXPECT_EQ(pFabric->Resolve<int>("Test.Func1", { 123 }), -123);
}

TEST(Fabric, KnownRegisterError) {
	Fabric::Ptr pFabric = std::make_shared<Fabric>();
	pFabric->Resolve<IExecuteable::Ptr>(
		"Default.Register",
		{
			pFabric,
			std::string("Test.Func"),
			(Fabric::Lambda)[](Fabric::Args args) { return std::any(-std::any_cast<int>(args[0])); }
		}
	)->Execute();

	EXPECT_THROW({
		pFabric->Resolve<IExecuteable::Ptr>(
			"Default.Register",
			{
				pFabric,
				std::string("Test.Func"),
				(Fabric::Lambda)[](Fabric::Args args) { return std::any(-2 * std::any_cast<int>(args[0])); }
			}
		)->Execute();
		}, std::invalid_argument);
}

TEST(Fabric, UnknownCallError) {
	Fabric::Ptr pFabric = std::make_shared<Fabric>();
	EXPECT_THROW({ pFabric->Resolve<int>("Test.Func1", { 123 }); }, std::invalid_argument);
}