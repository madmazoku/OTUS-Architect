#pragma once

#include <memory>
#include <map>
#include <string>
#include <any>
#include <vector>
#include <stdexcept>
#include <functional>
#include <set>

#include "../lesson.03.cpp/IExecuteable.h"

// Универсальная фабрика. Используется для хранения билдеров объектов приложения.
// По умолчанию фабрика содержит билдеры:
// Default.Register - возвращает команду регистрации нового билдера
// Default.NewScope - возвращает новую фабрику (скоуп)
// Default.Parent - возвращает родителя текущего скоупа
class Fabric
{
public:
	typedef std::shared_ptr<Fabric> Ptr;
	typedef std::vector<std::any> Args;

	typedef std::function<std::any(Args)> Lambda;
	typedef std::map<std::string, Lambda> MapId2Lambda;

	// Хелпер для регистрации билдера lambda с именем name в скоупе pScope
	static void Register(Fabric::Ptr pScope, const std::string& name, Fabric::Lambda lambda) {
		pScope->Resolve<IExecuteable::Ptr>("Default.Register", { pScope, name, lambda })->Execute();
	}
	// Хелпер для получения обязательного аргумента
	template<typename T>
	static T GetRequiredArg(Fabric::Args& args, size_t idx, std::string errMsg) {
		if (args.size() <= idx)
			throw std::invalid_argument(std::string("argument not passed: ") + errMsg);
		try {
			return std::any_cast<T>(args[idx]);
		}
		catch (std::bad_any_cast e) {
			throw std::invalid_argument(std::string("invalid type: ") + errMsg);
		}
	}
	// Хелпер для получения необязательного аргумента
	template<typename T>
	static T GetOptionaldArg(Fabric::Args& args, size_t idx, T defaultValue, std::string errMsg) {
		if (args.size() <= idx)
			return defaultValue;
		try {
			return std::any_cast<T>(args[idx]);
		}
		catch (std::bad_any_cast e) {
			throw std::invalid_argument(std::string("invalid type: ") + errMsg);
		}
	}

protected:
	MapId2Lambda m_scope;

	// вспомогательная команда для регистрации нового билдера в фабрике
	class RegisterCommand : public IExecuteable {
	protected:
		Fabric::Ptr m_pScope;
		std::string m_id;
		Fabric::Lambda m_value;

	public:
		RegisterCommand(Fabric::Ptr pScope, std::string id, Fabric::Lambda value) : m_pScope(pScope), m_id(id), m_value(value) {}

		virtual void Execute() override {
			Fabric::MapId2Lambda::iterator it = m_pScope->m_scope.find(m_id);
			if (it != m_pScope->m_scope.end())
				throw std::invalid_argument(std::string("can't register already known id: ") + m_id);
			m_pScope->m_scope.emplace(Fabric::MapId2Lambda::value_type(m_id, m_value));
		}
	};

public:
	Fabric(Fabric::Ptr pParent = nullptr) {
		Lambda lambdaRegister = [&](Args args) {
			Fabric::Ptr pScope = GetRequiredArg<Fabric::Ptr>(args, 0, "invalid scope, can't register");
			std::string id = GetRequiredArg<std::string>(args, 1, "invalid id, can't register");
			Lambda value = GetRequiredArg<Lambda>(args, 2, "invalid lambda, can't register");
			IExecuteable::Ptr pCommand = std::make_shared<RegisterCommand>(pScope, id, value);
			return std::any(pCommand);
		};
		Lambda lambdaNewScope = [](Args args) {
			Fabric::Ptr pFabric = GetOptionaldArg<Fabric::Ptr>(args, 0, nullptr, "invalid parent scope, can't make new scope");
			return std::any(std::make_shared<Fabric>(pFabric));
		};
		Lambda lambdaParent = [pParent](Args args) {
			return std::any(pParent);
		};
		m_scope.emplace(MapId2Lambda::value_type("Default.Register", lambdaRegister));
		m_scope.emplace(MapId2Lambda::value_type("Default.Scope.New", lambdaNewScope));
		m_scope.emplace(MapId2Lambda::value_type("Default.Parent", lambdaParent));
	}

	// возвращает результат вызова билдера ассоциированного с id, с передачей ему параметров args
	// в случае отсутствия билдера в текущем вызове, пытается вызывать его у родителя
	template<typename T>
	T Resolve(std::string id, Args args = Args()) {
		MapId2Lambda::iterator it = m_scope.find(id);
		if (it == m_scope.end()) {
			Fabric::Ptr pParent = Resolve<Fabric::Ptr>("Default.Parent");
			if (pParent != nullptr)
				return pParent->Resolve<T>(id, args);
			else
				throw std::invalid_argument(std::string("can't resolve: unknown id: ") + id);
		}
		Lambda lambda = it->second;
		try {
			return std::any_cast<T>(lambda(args));
		}
		catch (std::bad_any_cast e) {
			throw std::invalid_argument(std::string("can't resolve: can't cast lambda call result to required type for id: ") + id);
		}
	}
};
