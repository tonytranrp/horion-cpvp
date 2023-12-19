#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class SignEdit : public IModule {
public:
	SignEdit();
	~SignEdit();

	int range = 10;
	std::string SignText = "Horion On Top";

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};