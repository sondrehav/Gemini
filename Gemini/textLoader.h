#pragma once
#include <string>
#include <vector>

namespace gemini {
	namespace util {
		std::string LoadFile(std::string path);
		std::vector<std::string> LoadFileVec(std::string path);
	}
}