#pragma once

#include "ICommand.h"

class UnlockAchievementsCommand : public IMCCommand {
public:
	UnlockAchievementsCommand();

	virtual bool execute(std::vector<std::string>* args) override;
};
