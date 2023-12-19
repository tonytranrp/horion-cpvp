#include "Tower.h"

#include "../../../DrawUtils.h"

Tower::Tower() : IModule(0, Category::WORLD, "Like scaffold, but vertically and a lot faster") {
	registerFloatSetting("motion", &this->motion, this->motion, 0.3f, 1.f);
}

Tower::~Tower() {
}

const char* Tower::getModuleName() {
	return ("Tower");
}

std::string Tower::getModeText() {
	char motChar[50];
	sprintf_s(motChar, "Motion: %.1f", motion);
	return motChar;
}

bool Tower::tryTower(Vec3 blockBelow) {
	GameSettingsInput* input = Game.getGameSettingsInput();

	if (!input)
		return false;

	blockBelow = blockBelow.floor();

	DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f);

	auto& region = *Game.getRegion();
	Vec3i blockPosBelow(blockBelow);

	Block* block = region.getBlock(blockPosBelow);
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->canBeBuiltOver(region, blockPosBelow)) {
		Vec3i blok(blockBelow);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}
		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			if (!((region.getBlock(calc)->blockLegacy))->canBeBuiltOver(region, calc)) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate && GameData::isKeyDown(*input->spaceBar.key)) {
			Vec3 moveVec;
			moveVec.x = Game.getLocalPlayer()->stateVector->velocity.x;
			moveVec.y = motion;
			moveVec.z = Game.getLocalPlayer()->stateVector->velocity.z;
			Game.getLocalPlayer()->lerpMotion(moveVec);
			Game.getGameMode()->buildBlock(blok, i);

			return true;
		}
	}
	return false;
}

void Tower::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (!Game.getLocalPlayer())
		return;
	if (!Game.canUseMoveKeys())
		return;
	auto selectedItem = Game.getLocalPlayer()->getSelectedItem();
	if (!selectedItem->isValid() || !(*selectedItem->item)->isBlock())  // Block in hand?
		return;

	Vec3 blockBelow = Game.getLocalPlayer()->getEyePos();  // Block below the player
	blockBelow.y -= Game.getLocalPlayer()->aabbShape->height;
	blockBelow.y -= 0.5f;

	// Adjustment by velocity
	float speed = Game.getLocalPlayer()->stateVector->velocity.magnitudexy();
	Vec3 vel = Game.getLocalPlayer()->stateVector->velocity;
	vel.normalize();  // Only use values from 0 - 1

	if (!tryTower(blockBelow)) {
		if (speed > 0.05f) {
			blockBelow.z -= vel.z * 0.4f;
			if (!tryTower(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryTower(blockBelow) && Game.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryTower(blockBelow);
				}
			}
		}
	}
}

void Tower::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) return;
	if (packet->getId() == PacketID::MovePlayer) {
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		movePacket->pitch = 90;
	}
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rot = Vec2(90.0f, Game.getLocalPlayer()->viewAnglesPtr->viewAngles.y);
	}
}