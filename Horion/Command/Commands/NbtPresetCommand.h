#pragma once
#include "ICommand.h"

class NbtPresetCommand : public IMCCommand {
public:
	NbtPresetCommand();
	~NbtPresetCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
