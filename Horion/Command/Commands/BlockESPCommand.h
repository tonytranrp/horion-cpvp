#pragma once
#include "ICommand.h"

class BlockESPCommand : public IMCCommand {
public:
	BlockESPCommand();
	~BlockESPCommand() = default;

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
