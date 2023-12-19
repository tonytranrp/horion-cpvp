#include "NameTags.h"

#include "../../../../Utils/Target.h"
#include "../../ModuleManager.h"
#include "../../../../SDK/BuildPlatform.h"

NameTags::NameTags() : IModule(0, Category::VISUAL, "Shows better nametags") {
	this->registerBoolSetting("Armor", &this->displayArmor, this->displayArmor);
	this->registerBoolSetting("Encase Items", &this->encaseItems, this->encaseItems);
	this->registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.f, 1.f);
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

std::string NameTags::buildPlatformToString(BuildPlatform b) {
	switch (b) {
	case BuildPlatform::WinUWP:
	case BuildPlatform::Win32:
	case BuildPlatform::Linux:
		return " [PC]";

	case BuildPlatform::Android:
	case BuildPlatform::iOS:
	case BuildPlatform::FireOS:
	case BuildPlatform::WindowsPhone:
		return " [Mobile]";

	case BuildPlatform::Xbox:
	case BuildPlatform::PS4:
	case BuildPlatform::NintendoSwitch:
		return " [Controller]";

	default:
		return "";
	}
}

std::string NameTags::getSanitizedNameTag(Entity& ent) {
	std::string finalNameTag = Utils::sanitize(ent.getNameTag());
	if (ent.isPlayer()) {
		auto buildPlatform = reinterpret_cast<Player*>(&ent)->getBuildPlatformFromPlayerList();
		finalNameTag += NameTags::buildPlatformToString(buildPlatform);
	}
	return finalNameTag;
}

namespace {
void drawNameTags(Entity* ent) {
	auto localPlayer = Game.getLocalPlayer();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>("NameTags");

	if (nameTagsMod &&
		(ent != localPlayer) &&
		!ent->getNameTag().empty() &&
		Target::isValidTarget(ent, false, true)) {

		nameTagsMod->nameTags.insert(std::move(Utils::sanitize(ent->getNameTag())));
		float dist = ent->getPos()->dist(*Game.getLocalPlayer()->getPos());
		DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist));
		DrawUtils::Flush();
	}
}
}  // namespace

void NameTags::onPreRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (!localPlayer || !GameData::canUseMoveKeys()) return;

	Game.forEachEntity(::drawNameTags);

	auto option = Game.getClientInstance()->getOptions()->getOptionByName("game_thirdperson");
	if (option && option->maxI > 0) {
		DrawUtils::drawNameTags(localPlayer, fmax(0.6f, 3.f / 4.f));
		DrawUtils::Flush();
	}
}