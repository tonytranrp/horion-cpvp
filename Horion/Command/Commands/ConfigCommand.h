#pragma once
#include "..\..\Config\ConfigManager.h"
#include "ICommand.h"

class ConfigCommand : public IMCCommand {
public:
	ConfigCommand();
	~ConfigCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};