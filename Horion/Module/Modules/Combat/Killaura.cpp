#include "Killaura.h"
#include "../../../../Utils/TimerUtils.h"
#include <chrono>

static std::vector<Entity*> targetList;

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically") {
	RotMode.addEntry(EnumEntry("None", 0));
	RotMode.addEntry(EnumEntry("Silent", 1));
	RotMode.addEntry(EnumEntry("Client", 2));
#ifdef HORION_DEBUG
	RotMode.addEntry(EnumEntry("Actual", 3));
#endif
	registerEnumSetting("Rotations", &RotMode, 0);
	RotType.addEntry(EnumEntry("Normal", 0));
	RotType.addEntry(EnumEntry("Predict", 1));
	RotType.addEntry(EnumEntry("Average", 2));
	registerEnumSetting("Rotation Type", &RotType, 0);
	HitType.addEntry(EnumEntry("Multi", 0));
	HitType.addEntry(EnumEntry("Single", 1));
	HitType.addEntry(EnumEntry("Switch", 2));
	registerEnumSetting("Hit Type", &HitType, 0);
	this->registerFloatSetting("Range", &this->range, this->range, 2.f, 10.f);
	this->registerIntSetting("CPS", &this->aps, this->aps, 1, 20);
	this->registerBoolSetting("Auto Weapon", &this->autoweapon, this->autoweapon);
	this->registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	this->registerBoolSetting("TeamCheck", &this->teams, this->teams);
	this->registerBoolSetting("Death Disable", &this->DisableOnDeath, this->DisableOnDeath);
	this->registerBoolSetting("Visual Range", &this->visualRange, this->visualRange);
	this->registerBoolSetting("Prefer Swords", &this->preferSword, this->preferSword);
#ifdef HORION_DEBUG
	// True no slow, because the game won't even handle the knockback shit. I could probably set some attribute like knockback resistance(?) but idfk atp
	// Devs now have a slight upper hand in HVH, maybe B)
	this->registerBoolSetting("Packet", &this->packet, this->packet);
#endif
	registerColorSetting("Visual Range Color", &color, color);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

std::string Killaura::getModeText() {
	return HitType.GetSelectedEntry().GetName();
}

void Killaura::attack(Entity* target) {
	auto lp = Game.getLocalPlayer();
#ifdef HORION_DEBUG
	if (packet) {
		ItemUseOnActorInventoryTransaction complxTransac(
			ItemUseOnActorInventoryTransaction::ActionType::Attack,
			target->getRuntimeId(),
			lp->getSupplies()->selectedHotbarSlot,
			lp->getSelectedItem(),
			lp->stateVector->pos
		);

		InventoryTransactionPacket transPkt{&complxTransac, true};

		Game.getLoopbackPacketSender()->sendToServer(&transPkt);
	} else
#endif
	if (!Game.getGameMode()->attack(target))  // this returns a bool if it successfully attacks iirc
		return;
	lp->swing();
	Game.cpsLeft.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

bool Killaura::isValidTarget(Entity* target) {
	if (!Target::isValidTarget(target, this->isMobAura, false))
		return false;

	if (this->teams) {
		std::string targetName = target->getNameTag().c_str();
		std::string localName = Game.getLocalPlayer()->getNameTag().c_str();
		if (targetName.length() > 2 && localName.length() > 2) {
			targetName = std::regex_replace(targetName, std::regex("\\§r"), "");
			localName = std::regex_replace(localName, std::regex("\\§r"), "");
			if (targetName.at(2) == localName.at(2)) return false;
			if (Game.getLocalPlayer()->isAlliedTo(target)) return false;
		}
	}

	return (*target->getPos()).dist(*Game.getLocalPlayer()->getPos()) < this->range;
}

void Killaura::findWeapon() {
	auto* supplies = Game.getLocalPlayer()->getSupplies();
	auto* inv = supplies->inventory;
	std::vector<std::pair<ItemStack, int>> tVec;
	for (int n = 0; n < 9; n++) {
		auto* stack = inv->getItemStack(n);
		if (!stack->isValid()) {
			continue;
		}
		tVec.push_back({*stack, n}); //pushing back the pair into the vector
		std::sort(tVec.begin(), tVec.end(), [this](std::pair<ItemStack, int> lhs, std::pair<ItemStack, int> rhs) { // sorting through the vector for swords / dmg
			if (this->preferSword) {
				bool lhsSword = std::string_view((lhs.first).getItem()->tileName).find("_sword") != std::string::npos;  //finding if the name of the item is a sword or not - if u gotta better way lmk
				bool rhsSword = std::string_view((rhs.first).getItem()->tileName).find("_sword") != std::string::npos;
				if (lhsSword != rhsSword) {
					return lhsSword;
				}
			}
			return ((lhs.first).getAttackingDamageWithEnchants() > (rhs.first).getAttackingDamageWithEnchants()); //comparing damage
		});
	}
	if (!tVec.empty()) { // cant get slot if second = null
		supplies->selectedHotbarSlot = tVec[0].second;  // the best option will be sorted to be first in the container
	}
}

void Killaura::onLevelRender() {
	if (!Game.getLocalPlayer() || !Game.getGameMode()) return;
	auto plr = Game.getLocalPlayer();

	if (visualRange) {
		Vec3* start = plr->getPosOld();
		Vec3* end = plr->getPos();
		auto te = DrawUtils::getLerpTime();
		Vec3 pos = start->lerp(end, te);
		Vec3 permutations[90];
		std::vector<Vec3> posList;
		posList.reserve(90);
		for (int i = 0; i < 90; i++) {
			permutations[i] = {pos.x + range * cos(i * (PI * 2) / 45.f), plr->aabbShape->aabb.centerPoint().y, pos.z + range * sin(i * (PI * 2) / 45.f)};
		}
		for (auto& perm : permutations) {
			posList.push_back((perm));
		}
		DrawUtils::SetColor(color.r, color.g, color.b, 1.f);
		DrawUtils::drawLinestrip3D(posList);
	}

	if (DisableOnDeath && (plr->ticksAlive <= 1 || plr->getHealth() <= 0)) {
		this->setEnabled(false);
		return;
	}

	// Concept code because I don't like clearing targets on every tick DO NOT JUDGE - Void
	/*bool recheck;
	for (auto& targIter = targetList.begin(); targIter != targetList.end(); ++targIter) {
		if (!isValidTarget(*targIter)) {
			targetList.erase(targIter);
			recheck = true;
		}
	}
	if (targetList.empty() || recheck)
		Game.forEachEntity([&](Entity* currentEntity) {
			if (isValidTarget(currentEntity) && !std::count(targetList.begin(), targetList.end(), currentEntity)) // 2am code moment: std::count it, because that will have amazing performance
				targetList.push_back(currentEntity);
		});*/

	targetList.clear();
	Game.forEachEntity([&](Entity* entity) {
		if (isValidTarget(entity))
			targetList.push_back(entity);
	});

	hasTarget = !targetList.empty();

	if (!hasTarget) {
		targetIndex = 0;
		return;
	}

	if (HitType.selected != static_cast<uint8_t>(THitType::MULTI))
		std::sort(targetList.begin(), targetList.end(), [&plr](Entity* lhs, Entity* rhs) {
			return (*lhs->getPos()).dist(*plr->getPos()) < (*rhs->getPos()).dist(*plr->getPos());
		});

	if (autoweapon)
		findWeapon();

	if (TimerUtils::hasTimeElapsed(1000.f / aps)) {
		if (moduleMgr->getModule<Speed>("Criticals")->isEnabled()) doCritical();
		switch (static_cast<THitType>(HitType.selected)) {
			case THitType::MULTI: {
				RealEnt = targetList[0];
				for (auto& i : targetList)
					attack(i); // No bitches? No rotation packet for each target?
			} break;
			case THitType::SINGLE: {
				attack(RealEnt = targetList[0]);
			} break;
			case THitType::SWITCH: {
				if (plr->ticksAlive % 2 == 0)  // adds a little delay between switching instead of spamming back and fourth
					targetIndex++;
				if (targetIndex >= targetList.size())
					targetIndex = 0;
				attack(RealEnt = targetList[targetIndex]);
			} break;
		}

		if (RotMode.selected == static_cast<uint8_t>(TRotMode::SILENT) && plr->stateVector->velocity.squaredxzlen() < 0.01) { // make sure to update rotation if player is standing still
			MovePlayerPacket p(plr, *plr->getPos());
			Game.getClientInstance()->getLoopbackPacketSender()->sendToServer(&p);
		}
	}

	if (RotMode.selected == static_cast<uint8_t>(TRotMode::NONE))
		return; // dont get rotations if the selected type is none

	Vec3 rotations;
	float x, y, z;
	switch (static_cast<TRotType>(RotType.selected)) {
	case TRotType::NORMAL:
		x = RealEnt->getPos()->x;
		y = RealEnt->aabbShape->aabb.upper.y + (RealEnt->stateVector->oldPos.y - RealEnt->getPos()->y) / 2.f;
		z = RealEnt->getPos()->z;
		rotations = (Vec3(x, y, z));
		break;
	case TRotType::PREDICT:
		x = RealEnt->getPos()->x + (RealEnt->getPos()->x - RealEnt->getPosOld()->x);
		y = RealEnt->aabbShape->aabb.upper.y + (RealEnt->stateVector->oldPos.y - RealEnt->getPos()->y) / 2.f;
		z = RealEnt->getPos()->z + (RealEnt->getPos()->z - RealEnt->getPosOld()->z);
		rotations = (Vec3(x, y, z));
		break;
	case TRotType::AVERAGE:
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		for (auto ent : targetList) {
			x += ent->stateVector->pos.x;
			y += ent->aabbShape->aabb.upper.y + (ent->stateVector->oldPos.y - ent->getPos()->y) / 2.f;
			z += ent->stateVector->pos.z;
		}
		rotations = Vec3(x, y, z).div(float(targetList.size()));
		break;
	}
	angle = plr->getPos()->CalcAngle(rotations);

	if (RotMode.selected != static_cast<uint8_t>(TRotMode::SILENT)) {
		plr->viewAnglesPtr->viewAngles.x = angle.x;

		// FIXME: No BodyYaw

		//plr->yawUnused1 = angle.y;
		//plr->bodyYaw = angle.y;
	}
}

void Killaura::onDisable() {
	targetIndex = 0;
	hasTarget = false;
}

void Killaura::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) return;
	if (targetList.empty()) return;
	if (RotMode.selected == 1 || RotMode.selected == 2) {
		if (packet->getId() == PacketID::MovePlayer) {
			auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
		if (packet->getId() == PacketID::PlayerAuthInput) {
			auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authPacket->rot = Vec2(angle.x, angle.y);
			authPacket->headYaw = angle.y;
		}
	}
}

void Killaura::doCritical() {
	MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
	p.onGround = false;
	p.Position.y = Game.getLocalPlayer()->getPos()->y + 1;
	Game.getLoopbackPacketSender()->sendToServer(&p);

	MovePlayerPacket p2(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
	p2.onGround = false;
	p2.Position.y = Game.getLocalPlayer()->getPos()->y + 0.5f;
	Game.getLoopbackPacketSender()->sendToServer(&p2);
}
