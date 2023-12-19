#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class ShulkerNesting : public IModule {

public:
	ShulkerNesting();
	~ShulkerNesting();


	// Inherited via IModule
	virtual const char* getModuleName() override;
	
};