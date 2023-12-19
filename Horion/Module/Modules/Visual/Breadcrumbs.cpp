#include "Breadcrumbs.h"
#include "../../../DrawUtils.h"

Breadcrumbs::Breadcrumbs() : IModule(0x0, Category::VISUAL, "Leaves a trail behind you so that you dont get lost") {
	registerBoolSetting("ClearOnDisable", &clearOndisable, clearOndisable);
	registerBoolSetting("StopBreadcrumbs", &stopBreadCrumbs, stopBreadCrumbs);
}

Breadcrumbs::~Breadcrumbs() {
}

const char* Breadcrumbs::getModuleName() {
	return ("Breadcrumbs");
}

void Breadcrumbs::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	Vec3 floorPos = *Game.getLocalPlayer()->getPos();
	floorPos.y = Game.getLocalPlayer()->getAABB()->lower.y + 0.2f;

	if (!stopBreadCrumbs)
	linePoints.push_back(floorPos);
}

void Breadcrumbs::onLevelRender() {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (!linePoints.empty()) {
		DrawUtils::SetColor(1.f, 0.f, 0.f, 1.f);
		DrawUtils::drawLinestrip3D(linePoints);
	}
}

void Breadcrumbs::onDisable() {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (clearOndisable)
	linePoints.clear();
}