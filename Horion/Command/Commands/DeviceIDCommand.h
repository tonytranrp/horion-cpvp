#pragma once

#include "../../../SDK/Entity.h"
#include "ICommand.h"

class DeviceIDCommand : public IMCCommand {
public:
	DeviceIDCommand();

	virtual bool execute(std::vector<std::string>* args) override;
};
