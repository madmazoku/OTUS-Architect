#pragma once

#include <stdexcept>

class CommandException : public std::invalid_argument
{
public:
	explicit CommandException(const std::string& _Message) : invalid_argument(_Message.c_str()) {}
	explicit CommandException(const char* _Message) : invalid_argument(_Message) {}
};
