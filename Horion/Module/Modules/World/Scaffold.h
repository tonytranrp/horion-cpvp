#pragma once
#include "../Module.h"
class Scaffold : public IModule {
private:
	bool autoSelect = false;
	bool down = false;
	bool highlight = true;
	bool bypass = true;
	bool tryScaffold(Vec3 blockBelow);
	bool tryClutchScaffold(Vec3 blockBelow);
	bool findBlock();

public:
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onPostRender(MinecraftUIRenderContext* ctx) override;
};
