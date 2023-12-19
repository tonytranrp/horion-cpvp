#pragma once

#pragma comment(lib, "runtimeobject")

#include "../../Utils/Json.hpp"
#include "../Command/CommandMgr.h"
#include "../Module/ModuleManager.h"

using json = nlohmann::json;

class ConfigManager {
private:
	std::wstring roamingFolder;

public:
	static std::wstring GetRoamingFolderPath();

private:
	json currentConfigObj;

public:
	ConfigManager();
	~ConfigManager();

	std::string currentConfig = "default";

	void loadConfig(std::string name, bool create);
	void saveConfig();
	void init();
};

extern ConfigManager* configMgr;
