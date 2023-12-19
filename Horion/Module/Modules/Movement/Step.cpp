#include "Step.h"

Step::Step() : IModule(0, Category::MOVEMENT, "Modifiy your step") {
	mode = (SettingEnum(this)).addEntry({"Vanilla", 0})/*.addEntry(EnumEntry{"Cool", 1})*/;
	registerEnumSetting("Mode", &mode, 0);
	registerFloatSetting("Step Height", &this->height, this->height, 1.f, 10.f);
	registerIntSetting("Reverse Distance", &this->revHeight, this->revHeight, 1, 30);
	registerBoolSetting("Reverse", &this->reverse, this->reverse);
	registerBoolSetting("Packet (Cool)", &this->packets, this->packets);
	registerBoolSetting("Smooth (Cool)", &this->smoothing, this->smoothing);
}

Step::~Step() {
}

const char* Step::getModuleName() {
	return ("Step");
}

std::string Step::getModeText() {
	std::string speedStr = Utils::formatFloat(this->height);
	speedStr.insert(0, "Height: ");
	return speedStr;
}

void Step::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (!player)
		return;

	const Vec3 pos{player->getPos()->x, player->getAABB()->lower.y, player->getPos()->z},
		floorPos{pos.x, floorf(pos.y), pos.z};
	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};

	constexpr auto checkBlock = [](Vec3 poss, bool x = false) -> bool {
		if (Game.getRakNetConnector()->isOnAServer())
			poss.y += (x ? -1 : 1) * 0.0001f; // fixes shit but breaks shit. wtffff

		auto& region = *Game.getRegion();
		auto material = region.getMaterial(poss);
		bool isCorrectBlock = material->isBlockingMotion && material->isSolid && !region.getBlock(poss)->blockLegacy->canBeBuiltOver(region, poss);
		return isCorrectBlock;
	};

	if (!--smoothTicks)
		*Game.getMinecraft()->timer = 20.f;

	if (reverse && !player->isOnGround() && player->wasOnGround() && player->stateVector->velocity.y < 0 && !Game.getRegion()->getMaterial(pos.sub({0.f, 0.01f, 0.f}))->isBlockingMotion) {
		for (int i = 0; i < this->revHeight; i++) {
			if (checkBlock(pos.sub({0, i, 0}))) {
				player->stateVector->velocity.y = -3;
				player->setOnGround(true);
				return;
			}
		}
		return;
	}

	if (mode.selected == 1 && player->isOnGround() && player->collidedVertically() && player->collidedHorizontally() && moveVec2d.magnitude() > 0.f && !input->isJumping /* && !checkBlock(floorPos.sub({0.f, 0.5f, 0.f})) */) {
		// Predict motion a little further, required to be quite large like .45f because when mc functions (getBlock or getMaterial idk) round to int this has to go far enough into the block
		constexpr float speed = .45f;
		float yaw = player->viewAnglesPtr->viewAngles.y + 90.f;
		float mX = cosf(yaw * PI / 180.f),
			mZ = sinf(yaw * PI / 180.f);
		Vec2 fwdPred{moveVec2d.x * speed * mX - moveVec2d.y * speed * mZ, moveVec2d.x * speed * mZ + moveVec2d.y * speed * mX};

		constexpr std::array<float, 3> offsets{.42f, .27f, .33f};
		//for (float y = 0; y <= this->height - 1; y += 0.125f) {
		for (float y = 0.f; y < (this->height - 1.f + 0.0001f); y += 1.f) {
			// Check if clear above head
			if (checkBlock(pos.add({0.f, y + 2.f, 0.f})))
				break;

			auto stepBlockPos = floorPos.add(fwdPred.x, y, fwdPred.y);
			auto stepBlock = Game.getRegion()->getBlock(stepBlockPos);
			bool goodStepBlock = checkBlock(stepBlockPos, true);

			// Check for player space
			bool spaceAbove = !checkBlock(pos.add(fwdPred.x, y + 1.f, fwdPred.y)) && !checkBlock(pos.add(fwdPred.x, y + 2.f, fwdPred.y));

			//debugLogF("block check %d and space above %d", goodStepBlock, spaceAbove);

			if (spaceAbove && goodStepBlock) {
				AABB stepBlockAABB{{0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}};
				const Vec3i blockPos{static_cast<int>(pos.x + fwdPred.x), static_cast<int>(pos.y + y), static_cast<int>(pos.z + fwdPred.y)};
				stepBlock->blockLegacy->getCollisionShape(&stepBlockAABB, stepBlock, Game.getRegion(), &blockPos);

				float stepBlockHeight = stepBlockAABB.upper.y - stepBlockAABB.lower.y;

				float lmao = pos.y - floorPos.y;
				//debugLogF("ye %0g h %0g p %0g", y, stepBlockHeight, lmao);
				if (lmao == 0.9375)
					lmao = 0;
				player->getAABB()->offset({fwdPred.x, y + stepBlockHeight + lmao, fwdPred.y});

				if (this->smoothing) {
					*Game.getMinecraft()->timer = 6.f;
					smoothTicks = 2 + (1 * static_cast<int>(y));
				}

				if (this->packets) {
					MovePlayerPacket pkt(player, *player->getPos());
					for (int x = 0; x <= y; ++x) {
						for (auto offset : offsets) {
							pkt.Position.y += offset;
							Game.getLoopbackPacketSender()->sendToServer(&pkt);
						}
					}
				}

				break;
			}
		}
	}
}

void Step::onDisable() {
}