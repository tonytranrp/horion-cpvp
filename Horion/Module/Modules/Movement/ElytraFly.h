#pragma once
#include "../Module.h"
class ElytraFly : public IModule {
public:
	ElytraFly();
	~ElytraFly();

	float speed = 1.f;
	float glideMod = -0.00f;
	float glideModEffective = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onMove(MoveInputHandler* input) override;
};
