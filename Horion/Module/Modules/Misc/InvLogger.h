#pragma once
#include "../Module.h"
class InvLogger : public IModule {
public:
	InvLogger();
	~InvLogger();
	bool authInput = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
