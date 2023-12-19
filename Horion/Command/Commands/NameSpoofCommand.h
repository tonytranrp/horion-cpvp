#pragma once

#include "ICommand.h"

class NameSpoofCommand : public IMCCommand {
public:
	NameSpoofCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
