#include "Target.h"
#include "../Horion/Module/ModuleManager.h"

LocalPlayer** localPlayer;

void Target::init(LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(Entity* ent, bool mobs, bool visual) {
	auto localPlayer = Game.getLocalPlayer();
	auto entityTypeId = ent->getEntityTypeId();
	static auto antibot = moduleMgr->getModule<AntiBot>("AntiBot");
	static auto hitboxMod = moduleMgr->getModule<Hitbox>("Hitbox");
	static auto teams = moduleMgr->getModule<Teams>("Teams");
	static auto noFriends = moduleMgr->getModule<NoFriends>("NoFriends");

	if (!ent)
		return false;

	if (ent == localPlayer)
		return false;

	if (!ent->isAlive())
		return false;

	if (mobs) {
		std::vector<int> idList = {51, 56, 61, 62, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 102, 103, 106, 107, 117, 120, 218};
		for (int id: idList) {
			if (entityTypeId == id) return false;
		}
	} else {
		if (entityTypeId != 63) return false;
	}

	if (entityTypeId == 63 && !visual) {
		if (teams->isColorCheckEnabled()) {
			auto targetName = ent->getNameTag();
			auto localName = localPlayer->getNameTag();
			if (targetName.size() > 2 && localName.size() > 2) {
				auto colorTargetName = std::regex_replace(targetName.c_str(), std::regex("\\§r"), "");
				auto colorLocalName = std::regex_replace(localName.c_str(), std::regex("\\§r"), "");
				if (colorTargetName.at(2) == colorLocalName.at(2))
					return false;
			}
		}
		if (teams->isAlliedCheckEnabled()) {
			if (localPlayer->isAlliedTo(ent)) return false;
		}
	}

	if (!noFriends->isEnabled() && FriendList::findPlayer(ent->getNameTag().c_str()) && !visual)
		return false;

	if (antibot->isInvisibleCheckEnabled() && ent->isInvisible() && entityTypeId != 63)
		return false;

	if (antibot->isOtherCheckEnabled() && (ent->isSilent() || ent->isImmobile()))
		return false;

	if (!hitboxMod->isEnabled() && antibot->isHitboxCheckEnabled() && entityTypeId == 63)
		if ((ent->aabbShape->height < 1.5f || ent->aabbShape->width < 0.49f || ent->aabbShape->height > 2.1f || ent->aabbShape->width > 0.9f))
			return false;

	if (ent->aabbShape->width <= 0.01f || ent->aabbShape->height <= 0.01f)
		return false;

	if (!localPlayer->canAttack(ent))
		return false;

	if (antibot->isExtraCheckEnabled() && !ent->canShowNameTag())
		return false;

	return true;
}

bool Target::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}