#pragma once
#include "../Module.h"
class GhostHand : public IModule {
public:
	GhostHand();
	~GhostHand();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
