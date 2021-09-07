#pragma once

#include <any>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>

class UObject
{
public:
	typedef std::shared_ptr<UObject> Ptr;

protected:
	typedef std::map<std::string, std::pair<std::any, bool>> str2value;

	str2value m_properties;

public:

	const std::any& GetProperty(const std::string& key) const {
		str2value::const_iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		return it->second.first;
	}
	void SetProperty(const std::string& key, const std::any& value, bool readonly = false) {
		str2value::iterator it = m_properties.find(key);
		if (it != m_properties.end())
			if (it->second.second)
				throw std::invalid_argument(key);
			else
				it->second = str2value::mapped_type(value, readonly);
		else
			m_properties.emplace(str2value::value_type(key, str2value::mapped_type(value, readonly)));
	}
	void SetReadonly(const std::string& key, bool readonly = true) {
		str2value::iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		it->second.second = readonly;
	}
};
