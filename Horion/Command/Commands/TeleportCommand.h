#pragma once
#include "ICommand.h"
class TeleportCommand : public IMCCommand {
public:
	TeleportCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
