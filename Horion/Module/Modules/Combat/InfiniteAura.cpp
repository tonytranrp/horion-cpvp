#include "InfiniteAura.h"
#include "../../../../Utils/TimerUtils.h"

InfiniteAura::InfiniteAura() : IModule(0, Category::COMBAT, "Killaura with infinite reach") {
	this->registerBoolSetting("Multiaura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("mob", &this->mob, this->mob);
	this->registerFloatSetting("Range", &this->range, this->range, 15, 100);
	this->registerIntSetting("Cps", &this->delay, this->delay, 1, 20);
}

InfiniteAura::~InfiniteAura() {
}

const char* InfiniteAura::getModuleName() {
	return ("InfiniteAura");
}

std::string InfiniteAura::getModeText() {
	if (isMulti)
		return "Multi";
	else
		return "Single";
}

static std::vector<Entity*> targetList0;

void findEntities(Entity* currentEntity) {
	static auto infiniteAuraMod = moduleMgr->getModule<InfiniteAura>("InfiniteAura");

	if (!Target::isValidTarget(currentEntity, infiniteAuraMod->shouldTargetMobs(), false))
		return;
	
	if (infiniteAuraMod->shouldTargetMobs() && currentEntity->getEntityTypeId() == 63) return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < infiniteAuraMod->range) {
		targetList0.push_back(currentEntity);
	}
}

void InfiniteAura::onTick(GameMode* gm) {
	targetList0.clear();
	Game.forEachEntity(findEntities);

	if (targetList0.size() > 0) {
		if (TimerUtils::hasTimeElapsed((float)1000.f / delay)) {
		Game.getLocalPlayer()->swing();

		float calcYaw = (gm->player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
		float calcPitch = (gm->player->viewAnglesPtr->viewAngles.x) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
		MovePlayerPacket teleportPacket;

		if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "mco.cubecraft.net") == 0) {
			Vec3 pos = *Game.getLocalPlayer()->getPos();

			MovePlayerPacket movePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getLoopbackPacketSender()->sendToServer(&movePlayerPacket);

			pos.y += 0.35f;

			movePlayerPacket = MovePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getLoopbackPacketSender()->sendToServer(&movePlayerPacket);
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++) {
				Vec3 pos = *targetList0[i]->getPos();
				teleportPacket = MovePlayerPacket(Game.getLocalPlayer(), Vec3(pos.x - teleportX, pos.y, pos.z - teleportZ));
				Game.getLoopbackPacketSender()->sendToServer(&teleportPacket);
				Game.getGameMode()->attack(targetList0[i]);
				teleportPacket = MovePlayerPacket(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
				Game.getLoopbackPacketSender()->sendToServer(&teleportPacket);
			}
		} else {
			Vec3 pos = *targetList0[0]->getPos();
			teleportPacket = MovePlayerPacket(Game.getLocalPlayer(), Vec3(pos.x - teleportX, pos.y, pos.z - teleportZ));
			Game.getLoopbackPacketSender()->sendToServer(&teleportPacket);
			Game.getGameMode()->attack(targetList0[0]);
			teleportPacket = MovePlayerPacket(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
			Game.getLoopbackPacketSender()->sendToServer(&teleportPacket);
			}
		}
	}
}
