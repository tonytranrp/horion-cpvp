#pragma once

#include "ICommand.h"
class SayCommand : public IMCCommand {
public:
	SayCommand();
	~SayCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
