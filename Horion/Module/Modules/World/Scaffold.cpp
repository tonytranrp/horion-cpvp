#include "Scaffold.h"

#include "../../../../Utils/Logger.h"
#include "../../../DrawUtils.h"
#include "../../../Module/ModuleManager.h"

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you") {
	registerBoolSetting("AutoSelect", &this->autoSelect, this->autoSelect);
	registerBoolSetting("Down", &this->down, this->down);
	registerBoolSetting("Highlight", &this->highlight, this->highlight);
	registerBoolSetting("Bypass", &this->bypass, this->bypass);
}

Scaffold::~Scaffold() {
}

const char* Scaffold::getModuleName() {
	return "Scaffold";
}

std::string Scaffold::getModeText() {
	if (down)
		return "Down";
	else
		return "Normal";
}

bool Scaffold::tryScaffold(Vec3 blockBelow) {
	blockBelow = blockBelow.floor();

	if (this->highlight)
		DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f);

	auto& region = *Game.getRegion();
	Block* block = region.getBlock(Vec3i(blockBelow));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->canBeBuiltOver(region, blockBelow)) {
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
		auto& region = *Game.getRegion();
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((region.getBlock(calc)->blockLegacy))->canBeBuiltOver(region, calc);
			if (!Y) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			if (autoSelect) findBlock();
			Game.getGameMode()->buildBlock(blok, i);

			return true;
		}
	}
	return false;
}

bool Scaffold::tryClutchScaffold(Vec3 blockBelow) {
	static std::vector<Vec3i> checkBlocks;
	if (checkBlocks.empty()) { //Only re sort if its empty
		for (int y = -4; y <= 4; y++) {
			for (int x = -4; x <= 4; x++) {
				for (int z = -4; z <= 4; z++) {
					checkBlocks.push_back(Vec3i(x, y, z));
				}
			}
		}
		//https://www.mathsisfun.com/geometry/pythagoras-3d.html c2 = x2 + y2 + z2 funny
		std::sort(checkBlocks.begin(), checkBlocks.end(), [](Vec3i first, Vec3i last) {
			return sqrtf((float)(first.x * first.x) + (float)(first.y * first.y) + (float)(first.z * first.z)) < sqrtf((float)(last.x * last.x) + (float)(last.y * last.y) + (float)(last.z * last.z));
		});
	}

	blockBelow = blockBelow.floor();

	for (const Vec3i& blockOffset : checkBlocks) {
		Vec3i currentBlock = Vec3i(blockBelow).add(blockOffset);

		//Normal tryScaffold after it sorts
		auto& region = *Game.getRegion();
		Block* block = region.getBlock(currentBlock);
		BlockLegacy* blockLegacy = (block->blockLegacy);
		if (blockLegacy->canBeBuiltOver(region, currentBlock)) {
			Vec3i blok(currentBlock);

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
				bool Y = ((region.getBlock(calc)->blockLegacy))->canBeBuiltOver(region, calc);
				if (!Y) {
					// Found a solid block to click
					foundCandidate = true;
					blok = calc;
					break;
				}
				i++;
			}
			if (foundCandidate) {
				if (autoSelect) findBlock();
				Game.getGameMode()->buildBlock(blok, i);

				return true;
			}
		}
	}
	return false;
}

bool Scaffold::findBlock() {
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	auto prevSlot = supplies->selectedHotbarSlot;
	for (int i = 0; i < 8; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->item) {
			if (stack->getItem()->isBlock()) {
				if (prevSlot != i)
					supplies->selectedHotbarSlot = i;
				return true;
			}
		}
	}
	return false;
}

void Scaffold::onPostRender(MinecraftUIRenderContext* ctx) {
	if (!Game.getLocalPlayer())
		return;
	if (!Game.canUseMoveKeys())
		return;

	auto player = Game.getLocalPlayer();
	auto selectedItem = player->getSelectedItem();
	if ((!selectedItem || selectedItem->count == 0 || !selectedItem->item || !selectedItem->getItem()->isBlock()) && !autoSelect)  // Block in hand?
		return;

	// Adjustment by velocity
	float speed = Game.getLocalPlayer()->stateVector->velocity.magnitudexz();
	Vec3 vel = Game.getLocalPlayer()->stateVector->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1

	//if (bypass) Game.getLocalPlayer()->viewAnglesPtr->viewAngles.x = 50;

	if (this->down) {
		Vec3 blockBelow = player->getEyePos();  // Block 1 block below the player
		blockBelow.y -= player->aabbShape->height;
		blockBelow.y -= 1.5f;

		Vec3 blockBelowBelow = player->getEyePos();  // Block 2 blocks below the player
		blockBelowBelow.y -= player->aabbShape->height;
		blockBelowBelow.y -= 2.0f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				auto speedMod = moduleMgr->getModule<Speed>("Speed")->isEnabled();
				if (speedMod) {
					Game.getLocalPlayer()->stateVector->velocity.y = 0.01f;
					Game.getLocalPlayer()->stateVector->velocity.x *= 0.8f;
					Game.getLocalPlayer()->stateVector->velocity.z *= 0.8f;
				}
				blockBelow.z -= vel.z * 0.4f;
				blockBelowBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					blockBelowBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && player->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						blockBelowBelow.z += vel.z;
						blockBelowBelow.x += vel.x;
						tryScaffold(blockBelow);
						tryScaffold(blockBelowBelow);
					}
				}
			}
		}
	} else {
		Vec3 blockBelow = player->getEyePos();  // Block below the player
		blockBelow.y -= player->aabbShape->height;
		blockBelow.y -= 0.5f;

		auto& region = *Game.getRegion();
		Vec3i newBlockPos(blockBelow.floor());
		if (region.getBlock(newBlockPos)->blockLegacy->canBeBuiltOver(region, newBlockPos)) {
			auto speedMod = moduleMgr->getModule<Speed>("Speed")->isEnabled();
			if (speedMod) {
			    Game.getLocalPlayer()->stateVector->velocity.y = 0.01f;
				Game.getLocalPlayer()->stateVector->velocity.x *= 0.8f;
				Game.getLocalPlayer()->stateVector->velocity.z *= 0.8f;
			}
		tryClutchScaffold(blockBelow);
		}
		else if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
 				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && player->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						tryScaffold(blockBelow);
					}
				}
			}
		}
	}
}

void Scaffold::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) return;
	if (!this->bypass) return;
	if (packet->getId() == PacketID::MovePlayer) {
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		movePacket->pitch = 60;
	}
	if (packet->getId() == PacketID::PlayerAuthInput) {
		auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		authPacket->rot = Vec2(60.0f, Game.getLocalPlayer()->viewAnglesPtr->viewAngles.y);
	}
}