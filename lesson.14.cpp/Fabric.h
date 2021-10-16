#pragma once

#include <memory>
#include <map>
#include <string>
#include <any>
#include <vector>
#include <stdexcept>
#include <functional>

#include "ICommand.h"

class Fabric
{
public:
	typedef std::shared_ptr<Fabric> Ptr;
	typedef std::vector<std::any> Args;

	typedef std::function<std::any(Args)> Lambda;
	typedef std::map<std::string, Lambda> MapId2Lambda;
protected:
	Ptr m_pParent;

	MapId2Lambda m_scope;

	class RegisterCommand : public ICommand {
	protected:
		MapId2Lambda& m_scope;
		std::string m_id;
		Lambda m_value;

	public:
		RegisterCommand(MapId2Lambda& scope, std::string id, Lambda value) : m_scope(scope), m_id(id), m_value(value) {}

		virtual void Do() override {
			MapId2Lambda::iterator it = m_scope.find(m_id);
			if (it != m_scope.end())
				throw std::invalid_argument(std::string("can't register already known id: ") + m_id);
			m_scope.emplace(MapId2Lambda::value_type(m_id, m_value));
		}
	};

public:
	Fabric(Fabric::Ptr pParent = nullptr) : m_pParent(pParent) {
		Lambda lambda = [&](Args args) {
			if (args.size() == 0)
				throw std::invalid_argument(std::string("can't register: id not passed"));
			std::string id;
			try {
				id = std::any_cast<std::string>(args[0]);
			}
			catch (std::bad_any_cast e) {
				throw std::invalid_argument(std::string("can't register: id is not string"));
			}
			if (args.size() == 1)
				throw std::invalid_argument(std::string("can't register: value not passed for ") + id);
			Lambda value;
			try {
				value = std::any_cast<Lambda>(args[1]);
			}
			catch (std::bad_any_cast e) {
				throw std::invalid_argument(std::string("can't register: value is not std::any(std::vector<std::any>) for ") + id);
			}
			ICommand::Ptr pCommand = std::make_shared<RegisterCommand>(m_scope, id, value);
			return std::any(pCommand);
		};
		m_scope.emplace(MapId2Lambda::value_type("Default.Register", lambda));
	}

	template<typename T>
	T Resolve(std::string id, Args args = Args()) {
		MapId2Lambda::iterator it = m_scope.find(id);
		if (it == m_scope.end())
			if (m_pParent != nullptr)
				return m_pParent->Resolve<T>(id, args);
			else
				throw std::invalid_argument(std::string("can't resolve: unknown id: ") + id);
		Lambda lambda = it->second;
		std::any a = lambda(args);
		T value;
		try {
			value = std::any_cast<T>(a);
		}
		catch (std::bad_any_cast e) {
			throw std::invalid_argument(std::string("can't resolve: can't cast lambda call result to required type for id: ") + id);
		}
		return value;
	}
};
