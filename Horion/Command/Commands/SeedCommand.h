#pragma once
#include "ICommand.h"

class SeedCommand : public IMCCommand {
public:
	SeedCommand();

	virtual bool execute(std::vector<std::string>* args) override;
};
