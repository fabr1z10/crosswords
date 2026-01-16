#include "config.h"
#include <toml++/toml.h>
#include <iostream>


void ConfigManager::load(const std::string& path) {
	try {
		toml::table tbl = toml::parse_file(path);


		if (auto rng = tbl["rng"].as_table()) {
			_cfg.rng.deterministic =
					(*rng)["deterministic"].value_or(_cfg.rng.deterministic);
			_cfg.rng.seed =
					(*rng)["seed"].value_or(_cfg.rng.seed);
		}

		if (auto paths = tbl["paths"].as_table()) {
			_cfg.paths.schema_file =
					(*paths)["schema_file"].value_or(_cfg.paths.schema_file);
			_cfg.paths.default_dictionary =
					(*paths)["default_dictionary"].value_or(_cfg.paths.default_dictionary);
		}
	} catch (const toml::parse_error& err) {
		std::cerr << "TOML parse error: " << err << "\n";
	}
}