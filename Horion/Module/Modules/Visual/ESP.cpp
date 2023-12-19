#include "ESP.h"
#include "../../../../Utils/Target.h"
#include "../../../../resource.h"

ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you") {
	this->registerBoolSetting("Rainbow", &this->doRainbow, this->doRainbow);
	this->registerBoolSetting("MobEsp", &this->isMobEsp, this->isMobEsp);
	type.addEntry(EnumEntry("Outline", 0));
	type.addEntry(EnumEntry("3d", 1));
	type.addEntry(EnumEntry("2d", 2));
   type.addEntry(EnumEntry("Von", 3));
	registerEnumSetting("Type", &type, 0);
   registerColorSetting("Color", &this->color, this->color);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

std::string ESP::getModeText() {
	return type.GetSelectedEntry().GetName();
}


static float rcolors[4];

void renderVon(Entity* ent, float size) {
	Vec4 rect = DrawUtils::getRectForAABB(ent->aabbShape->aabb);

	auto vroy = DrawUtils::resourceToTexturePtr(VROY_DATA, "Vroy");
	DrawUtils::drawImageFromTexturePtr(vroy, Vec2(rect.x - 5.f, rect.y), Vec2(rect.z - rect.x + 5.f, rect.w - rect.y), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
	DrawUtils::flushImages();
}

void renderEsp(Entity* ent) {
	static auto espMod = moduleMgr->getModule<ESP>("ESP");
	static auto noFriendsModule = moduleMgr->getModule<NoFriends>("NoFriends");
	auto localPlayer = Game.getLocalPlayer();

	if (ent == localPlayer)
		return;

	if (Target::isValidTarget(ent, espMod->isMobEsp, true)) {
		if (espMod->doRainbow) {
			DrawUtils::SetColor(rcolors[0], rcolors[1], rcolors[2], 1.f);
		} else if (!noFriendsModule->isEnabled() && FriendList::findPlayer(ent->getNameTag().c_str())) {
				DrawUtils::SetColor(0.1f, 0.9f, 0.1f, 1.f);
		} else
			DrawUtils::SetColor(espMod->color.r, espMod->color.g, espMod->color.b, 1.f);

		if (espMod->type.selected == 2)
			DrawUtils::draw2D(ent, (float)fmax(0.3f, 1 / (float)fmax(1, localPlayer->getPos()->dist(*ent->getPos()) * 3.f)));
		else if (espMod->type.selected == 3)
			renderVon(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getPos()->dist(*ent->getPos()))));
		else
			DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getPos()->dist(*ent->getPos()))), espMod->type.selected == 0);
	}
}

void ESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer && GameData::canUseMoveKeys()) {
		{
			if (rcolors[3] < 1) {
				rcolors[0] = 0.2f;
				rcolors[1] = 0.2f;
				rcolors[2] = 1.f;
				rcolors[3] = 1;
			}

			Utils::ApplyRainbow(rcolors, 0.0015f);
		}
		Game.forEachEntity(renderEsp);
	}
}