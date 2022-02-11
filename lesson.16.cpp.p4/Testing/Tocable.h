#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Library {
	namespace Register {
		class Tocable
		{
		public:
			typedef std::shared_ptr < Tocable > Ptr;

			virtual std::string GetTitle() const = 0;
			virtual void SetTitle(const std::string&) = 0;

			virtual std::vector<std::string> GetChapters() const = 0;
			virtual void SetChapters(std::vector<std::string>) = 0;

			virtual void Finish() = 0;
		};
	}
}
