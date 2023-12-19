#include "BowAimbot.h"

#include "../../../../Utils/Target.h"
#include "../../../DrawUtils.h"

std::vector<Entity*> targetList;

BowAimbot::BowAimbot() : IModule(0, Category::COMBAT, "Aimbot, but for bows") {
	registerBoolSetting("silent", &this->silent, this->silent);
	registerBoolSetting("predict", &this->predict, this->predict);
	registerBoolSetting("visualize", &this->visualize, this->visualize);
	registerFloatSetting("Height", &this->height, 1.0f, 0.1f, 2.0f);
}

BowAimbot::~BowAimbot() {
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

const char* BowAimbot::getModuleName() {
	return ("BowAimbot");
}

std::string BowAimbot::getModeText() {
	if (predict)
		return "Predict";
	else
		return "Normal";
}

void BowAimbot::onPostRender(MinecraftUIRenderContext* renderCtx) {
	targetList.clear();

	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (!localPlayer)
		return;

	if (localPlayer->getSelectedItemId() != 300 && localPlayer->getSelectedItemId() != 575)  // Bow in hand?
		return;

	if (!((GameData::isRightClickDown() || localPlayer->getSelectedItemId() == 575) && GameData::canUseMoveKeys()))  // is aiming?
		return;

	Game.forEachEntity([&](Entity* ent) {
		if (!Target::isValidTarget(ent, false, false))
			return;

		float dist = (*ent->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < 130) {
			targetList.push_back(ent);
		}
	});

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		Vec3 origin = Game.getLocalPlayer()->getEyePos();  // TODO: sort list
		Entity* entity = targetList[0];
		Vec3 pos = entity->aabbShape->aabb.centerPoint();
		if (predict) {
			Vec3 velocity = entity->getPos()->sub(*entity->getPosOld());
			velocity.x *= origin.dist(pos) / 2.f;
			velocity.z *= origin.dist(pos) / 2.f;
			pos = pos.add(velocity);
		}
		if (visualize)
			DrawUtils::drawBox(pos.sub(0.5), pos.add(0.5), 0.3f, true);
		pos = pos.sub(origin);
		float yaw = (atan2f(pos.z, pos.x) * DEG_RAD) - 90;
		float len = pos.magnitudexz();
		float g = 0.03f * this->height;  // Vanilla = 0.002, Nukkit = 0.012, NetherGames = 0.005, HyperLands = 0.065
		float tmp = 1 - g * (g * (len * len) + 2 * pos.y);
		float pitch = DEG_RAD * -atanf((1 - sqrtf(tmp)) / (g * len));

		if (this->silent) {
			this->angle = Vec2(pitch, yaw);
			MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
			p.pitch = angle.x;
			p.yaw = angle.y;
			p.headYaw = angle.y;
			Game.getLoopbackPacketSender()->sendToServer(&p);
		} else {
			if (pitch < 89 && pitch > -89) {
				Vec2 angles = Vec2(pitch, yaw);

				Vec2 appl = angles.sub(localPlayer->viewAnglesPtr->viewAngles).normAngles();
				appl.x = -appl.x;
				appl = appl.div(7);  // Smooth dat boi

				localPlayer->applyTurnDelta(&appl);
			}
		}
	}
}

void BowAimbot::onSendPacket(Packet* packet) {
	if (targetList.empty()) return;
	if (!silent) return;

		if (packet->getId() == PacketID::MovePlayer) {
			auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
			movePacket->pitch = this->angle.x;
			movePacket->headYaw = this->angle.y;
			movePacket->yaw = this->angle.y;
		}

		if (packet->getId() == PacketID::PlayerAuthInput) {
			auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authPacket->rot = Vec2(angle.x, angle.y);
			authPacket->headYaw = angle.y;
		}
	
}