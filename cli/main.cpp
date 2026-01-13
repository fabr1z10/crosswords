#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "core/solver.h"
#include <filesystem>
#include "yaml-cpp/yaml.h"

namespace fs = std::filesystem;

using namespace std;


int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << " Missing parameter: dictionary file" << std::endl;
		exit(1);
	} else if (argc < 3) {
		std::cout << " Missing parameter: schema file" << std::endl;
		exit(1);
	} else if (argc < 4) {
		std::cout << " Missing parameter: schema id" << std::endl;
		exit(1);
	}

	// check files exist
	if (!fs::exists(argv[1])) {
		std::cout << " Cannot find: " << argv[1] << "\n";
		exit(1);
	}
	if (!fs::exists(argv[2])) {
		std::cout << " Cannot find: " << argv[2] << "\n";
		exit(1);
	}

	Dictionary d(argv[1]); //"/home/fabrizio/playground/crossword/dict/ita");


	// open schema
	int width = 0;
	int height = 0;
	std::vector<int> data;
	YAML::Node config = YAML::LoadFile(argv[2]);
	for (const auto& items : config) {
		auto name = items.first.as<std::string>();
		if (name == argv[3]) {
			auto size = items.second["size"].as<std::vector<int>>();
			width = size[0]; height = size[1];
			data = items.second["black"].as<std::vector<int>>();
		}
	}
	if (width > 0) {

		auto g = Grid(width, height, data);
		Solver s;
		s.solve(g, d);
	} else {
		std::cout << " Cannot find schema: " << argv[2] << "\n";
		exit(1);
	}
	return 0;


}