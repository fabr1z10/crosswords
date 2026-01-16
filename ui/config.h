#pragma once

#include <string>
#include <cstdint>

struct Config {
	struct Rng {
		bool deterministic = false;
		uint32_t seed = 0;
	} rng;

	struct Paths {
		std::string schema_file;
		std::string default_dictionary;
	} paths;
};

class ConfigManager {
public:
	// Access the singleton instance
	static ConfigManager& instance() {
		static ConfigManager _instance;
		return _instance;
	}

	// Load TOML file
	void load(const std::string& path);

	// Access the Config struct
	const Config& get() const { return _cfg; }

private:
	ConfigManager() = default;
	Config _cfg;
};