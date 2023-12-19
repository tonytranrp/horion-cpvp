#pragma once
#include "ICommand.h"

class GodPotionCommand : public IMCCommand {
public:
	GodPotionCommand();
	~GodPotionCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
