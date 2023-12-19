#include "Tracer.h"
#include "../../../../SDK/Camera.h"
#include "../../../DrawUtils.h"

Tracer::Tracer() : IModule('R', Category::VISUAL, "Draws lines to ESP highlighted entities") {
	registerBoolSetting("Include Mobs", &this->includeMobs, this->includeMobs);
	registerColorSetting("Color", &this->color, this->color);
}

Tracer::~Tracer() {
}

const char* Tracer::getModuleName() {
	return "Tracer";
}
void Tracer::onLevelRender() {
	static auto tracerMod = moduleMgr->getModule<Tracer>("Tracer");
	LocalPlayer* player = Game.getLocalPlayer();
	if (!player) return;

	float calcYaw = (player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	float calcPitch = (player->viewAnglesPtr->viewAngles.x) * -(PI / 180);
	Vec3 moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.2f;
	moveVec.y = sin(calcPitch) * 0.2f;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.2f;

	const Vec3 origin = Game.getLevelRenderer()->getOrigin().add(moveVec);
	Game.forEachEntity([&](Entity* ent) {
		if (ent != Game.getLocalPlayer() && Target::isValidTarget(ent, this->includeMobs, true)) {
			DrawUtils::SetColor(tracerMod->color.r, tracerMod->color.g, tracerMod->color.b, 1.f);
			DrawUtils::drawLine3D(origin, ent->getAABB()->lower, true);
		}
	});
}