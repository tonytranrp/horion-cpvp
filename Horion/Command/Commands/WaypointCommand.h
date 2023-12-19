#pragma once

#include "../../Module/ModuleManager.h"
#include "../../Module/Modules/Visual/Waypoints.h"
#include "ICommand.h"

class WaypointCommand : public IMCCommand {
public:
	WaypointCommand();
	~WaypointCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
