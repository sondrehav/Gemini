#include <string>

namespace gemini {
	namespace helper {
		std::string replace(std::string input, std::string textToReplace, std::string replacement)
		{
			size_t finder;
			std::string output = input;
			while ((finder = output.rfind(textToReplace)) != std::string::npos)
			{
				output.replace(finder, textToReplace.length(), replacement);
			}
			return output;
		}
	}
}