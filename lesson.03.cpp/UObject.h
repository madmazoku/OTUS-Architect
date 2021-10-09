#pragma once

#include <any>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <mutex>
#include <shared_mutex>

class UObject
{
public:
	typedef std::shared_ptr<UObject> Ptr;

protected:
	typedef std::map<std::string, std::pair<std::any, bool>> str2value;

	str2value m_properties;
	mutable std::shared_mutex m_lock;

public:

	bool HasProperty(const std::string& key) const {
		std::shared_lock<std::shared_mutex> sl(m_lock);

		str2value::const_iterator it = m_properties.find(key);
		return it != m_properties.end();
	}
	const std::any& GetProperty(const std::string& key) const {
		std::shared_lock<std::shared_mutex> sl(m_lock);

		str2value::const_iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		return it->second.first;
	}
	void SetProperty(const std::string& key, const std::any& value, bool readonly = false) {
		std::unique_lock<std::shared_mutex> ul(m_lock);

		str2value::iterator it = m_properties.find(key);
		if (it != m_properties.end())
			if (it->second.second)
				throw std::invalid_argument(key);
			else
				it->second = str2value::mapped_type(value, readonly);
		else
			m_properties.emplace(str2value::value_type(key, str2value::mapped_type(value, readonly)));
	}
	void SetPropertyReadonly(const std::string& key, bool readonly = true) {
		std::unique_lock<std::shared_mutex> ul(m_lock);

		str2value::iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		it->second.second = readonly;
	}
	void RemoveProperty(const std::string& key) {
		std::unique_lock<std::shared_mutex> ul(m_lock);

		str2value::iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		m_properties.erase(it);
	}
};
