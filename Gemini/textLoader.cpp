#include "textLoader.h"
#include <fstream>
#include <iostream>

namespace gemini {
	namespace util {

		std::string LoadFile(std::string path){

			std::ifstream in(path);
			if (!in.good()){
				std::cerr << path << " not good!" << std::endl;
			}
			std::string contents((std::istreambuf_iterator<char>(in)),
				std::istreambuf_iterator<char>());
			in.close();
			return contents;
		}

		std::vector<std::string> LoadFileVec(std::string path){

			std::ifstream in(path);
			if (!in.good()){
				std::cerr << path << " not good!" << std::endl;
			}
			std::string line;
			std::vector<std::string> output;
			while (std::getline(in, line)){
				output.push_back(line);
			}
			in.close();
			return output;

		}

	}
}