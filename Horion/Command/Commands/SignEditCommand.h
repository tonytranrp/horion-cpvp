#pragma once

#include "ICommand.h"

class SignEditCommand : public IMCCommand {
public:
	SignEditCommand();
	~SignEditCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};