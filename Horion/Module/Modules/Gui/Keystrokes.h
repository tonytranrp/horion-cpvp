#pragma once
#include "../Module.h"

class Keystrokes : public IModule {
public:
	Keystrokes();
	~Keystrokes();
	Vec2 position = Utils::percentToWindowPos(Vec2(0.003125f, 0.594810f));

	// Inherited via IModule
	virtual const char* getModuleName() override;
	static void drawKeystroke(char key, const Vec2& pos);
	static void drawLeftMouseKeystroke(Vec2 pos);
	static void drawRightMouseKeystroke(Vec2 pos);
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};
