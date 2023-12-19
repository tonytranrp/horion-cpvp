#pragma once
#include "../Module.h"
class TrueSight : public IModule {
public:
	TrueSight();
	~TrueSight();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
#pragma once
