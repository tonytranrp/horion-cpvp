#pragma once
#include "../Module.h"
class BowAimbot : public IModule {
private:
	bool silent = true;
	bool predict = false;
	bool visualize = false;
	float height = 1.0f;
	Vec2 angle;

public:
	BowAimbot();
	~BowAimbot();

	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet) override;
};
