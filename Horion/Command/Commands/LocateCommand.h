#pragma once
#include "ICommand.h"

class LocateCommand : public IMCCommand {
public:
	LocateCommand();
	~LocateCommand();

	std::unordered_map<std::string, StructureFeatureType> StructureMap;

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
