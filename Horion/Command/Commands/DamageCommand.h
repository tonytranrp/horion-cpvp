#pragma once
#include "ICommand.h"
#include "../../../SDK/Packet.h"

class DamageCommand : public IMCCommand {
public:
	DamageCommand();
	~DamageCommand() = default;

	virtual bool execute(std::vector<std::string>* args) override;
};
