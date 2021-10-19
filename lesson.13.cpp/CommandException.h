#pragma once

#include <stdexcept>

class CommandException : public std::invalid_argument
{
public:
	explicit CommandException(const std::string& message) : invalid_argument(message.c_str()) {}
	explicit CommandException(const char* message) : invalid_argument(message) {}
};
