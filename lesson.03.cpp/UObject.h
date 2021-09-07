#pragma once

#include <any>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>

typedef std::map<std::string, std::any> str2any;

class UObject
{
public:
	typedef std::shared_ptr<UObject> Ptr;

protected:
	str2any m_properties;

public:

	const std::any& GetProperty(const std::string& key) const { 
		str2any::const_iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		return it->second; 
	}
	void SetProperty(const std::string& key, const std::any& value) {
		m_properties[key] = value;
	}
};



