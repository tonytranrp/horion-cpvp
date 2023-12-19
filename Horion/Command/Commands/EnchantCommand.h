#pragma once
#include "ICommand.h"
class EnchantCommand : public IMCCommand {
public:
	EnchantCommand();

	virtual bool execute(std::vector<std::string>* args) override;
};
