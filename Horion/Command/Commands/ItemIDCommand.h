#pragma once
#include "ICommand.h"

class ItemIDCommand : public IMCCommand {
public:
	ItemIDCommand();
	~ItemIDCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};