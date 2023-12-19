#include "ChestESP.h"

#include "../../../DrawUtils.h"

ChestESP::ChestESP() : IModule(0, Category::VISUAL, "ESP & Tracers for chests") {
	Mode = SettingEnum(this)
			   .addEntry(EnumEntry("ESP", 0))
			   .addEntry(EnumEntry("Tracers", 1))
			   .addEntry(EnumEntry("Both", 2));
	registerEnumSetting("Mode", &Mode, 0);
	this->registerBoolSetting("Chests", &this->chests, this->chests);
	this->registerBoolSetting("Trap Chests", &this->trappedChests, this->trappedChests);
	this->registerBoolSetting("Ender Chests", &this->enderChests, this->enderChests);
	this->registerBoolSetting("Barrels", &this->barrels, this->barrels);
	this->registerBoolSetting("Shulkers", &this->shulkers, this->shulkers);
	this->registerBoolSetting("Outline", &this->doOutline, this->doOutline);
	this->registerBoolSetting("Colored Shulkers", &this->doShulkerColors, this->doShulkerColors);
}

ChestESP::~ChestESP() {
}

const char* ChestESP::getModuleName() {
	return ("ChestVisuals");
}

std::string ChestESP::getModeText() {
	return Mode.GetSelectedEntry().GetName();
}

void ChestESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (!Game.isInGame() || !GameData::canUseMoveKeys() || !Game.getLocalPlayer())
		return;

	static uintptr_t getMapColorF = FindSignature("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B E9 4D 8B E0 4C 8B F2 4C 8B F9");
	using get_map_color_t = MC_Color*(__fastcall*)(Block*, MC_Color*, BlockSource*, Vec3i);
	static get_map_color_t getMapColor = reinterpret_cast<get_map_color_t>(getMapColorF);

	if (Mode.selected == 1) return;
	auto ourListLock = std::scoped_lock(this->listLock);

	for (const auto& chest : bufferedChestList) {
		auto storageID = Game.getRegion()->getBlock(chest.lower)->blockLegacy->blockId;
		float math = (float)fmax(0.3f, (float)fmin(1.f, 15));

		Vec3 blockPos = chest.lower;
		if (storageID != 205 && storageID != 218 && storageID != 458) {
			if (blockPos.x < 0)
				blockPos.x -= 1;
			if (blockPos.z < 0)
				blockPos.z -= 1;
		}
		storageID = Game.getRegion()->getBlock(blockPos)->toLegacy()->blockId;

		auto mathVect = Vec3(chest.upper.floor().add(Vec3(1.f, 1.f, 1.f)).sub(chest.upper));
		mathVect.y = floor(mathVect.y);

		switch (storageID) {
		case 54:
			if (chests) {
				DrawUtils::SetColor(0.7f, 0.5f, 0.3f, math);  // Normal Chest
				DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(chest.lower))), this->doOutline);
			}
			break;
		case 146:
			if (trappedChests) {
				DrawUtils::SetColor(1.0f, 0.2f, 0.1f, math);  // Trapped Chest
				DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(chest.lower))), this->doOutline);
			}
			break;
		case 130:
			if (enderChests) {
				DrawUtils::SetColor(0.8f, 0.0f, 1.0f, math);  // Ender Chest
				DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(chest.lower))), this->doOutline);
			}
			break;
		case 205:
			if (shulkers) {
				DrawUtils::SetColor(0.35f, 0.24f, 0.4f, math);  // Undyed Shulker Boxes
				DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(chest.lower))), this->doOutline);
			}
			break;
		case 218:
			if (shulkers) {
				auto block = Game.getRegion()->getBlock(blockPos);
				MC_Color holder;
				getMapColor(block, &holder, Game.getRegion(), blockPos);
				if (doShulkerColors)
					DrawUtils::SetColor(holder.r, holder.g, holder.b, math);  // Shulker Boxes
				else
					DrawUtils::SetColor(0.35f, 0.24f, 0.4f, math);
				DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(chest.lower))), this->doOutline);
			}
			break;
		case 458:
			if (barrels) {
				DrawUtils::SetColor(0.9f, 0.8f, 0.3f, math);  // Barrel
				DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(chest.lower))), this->doOutline);
			}
			break;
		}
	}
}

void ChestESP::onLevelRender() {
	if (!Game.isInGame() || !GameData::canUseMoveKeys() || !Game.getLocalPlayer())
		return;

	static uintptr_t getMapColorF = FindSignature("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B E9 4D 8B E0 4C 8B F2 4C 8B F9");
	using get_map_color_t = MC_Color*(__fastcall*)(Block*, MC_Color*, BlockSource*, Vec3i);
	static get_map_color_t getMapColor = reinterpret_cast<get_map_color_t>(getMapColorF);

	if (Mode.selected == 0) return;
	LocalPlayer* player = Game.getLocalPlayer();
	if (!player) return;

	float calcYaw = (player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	float calcPitch = (player->viewAnglesPtr->viewAngles.x) * -(PI / 180);
	Vec3 moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.2f;
	moveVec.y = sin(calcPitch) * 0.2f;
	moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.2f;

	const Vec3 origin = Game.getLevelRenderer()->getOrigin().add(moveVec);
	auto ourListLock = std::scoped_lock(this->listLock);
	Vec2 windowSize = Game.getGuiData()->windowSize;

	for (const auto& chest : bufferedChestList) {
		auto storageID = Game.getRegion()->getBlock(chest.lower)->blockLegacy->blockId;
		float math = (float)fmax(0.3f, (float)fmin(1.f, 15));
		DrawUtils::SetColor(1.f, 1.f, 1.f, math);

		Vec3 blockPos = chest.lower;
		if (storageID != 205 && storageID != 218 && storageID != 458) {
			if (blockPos.x < 0)
				blockPos.x -= 1;
			if (blockPos.z < 0)
				blockPos.z -= 1;
		}
		Vec3 tracePos = chest.lower;
		tracePos.x += 0.45f;
		storageID = Game.getRegion()->getBlock(blockPos)->toLegacy()->blockId;
		switch (storageID) {
		case 54:
			if (chests) {
				DrawUtils::SetColor(0.7f, 0.5f, 0.3f, math);  // Normal Chest
				DrawUtils::drawLine3D(origin, tracePos, true);
			}
			break;
		case 146:
			if (trappedChests) {
				DrawUtils::SetColor(1.0f, 0.2f, 0.1f, math);  // Trapped Chest
				DrawUtils::drawLine3D(origin, tracePos, true);
			}
			break;
		case 130:
			if (enderChests) {
				DrawUtils::SetColor(0.8f, 0.0f, 1.0f, math);  // Ender Chest
				DrawUtils::drawLine3D(origin, tracePos, true);
			}
			break;
		case 205:
			if (shulkers) {
				DrawUtils::SetColor(0.35f, 0.24f, 0.4f, math);  // Undyed Shulker Boxes
				DrawUtils::drawLine3D(origin, tracePos, true);
			}
			break;
		case 218:
			if (shulkers) {
				auto block = Game.getRegion()->getBlock(blockPos);
				MC_Color holder;
				getMapColor(block, &holder, Game.getRegion(), blockPos);
				if (doShulkerColors)
					DrawUtils::SetColor(holder.r, holder.g, holder.b, math);  // Shulker Boxes
				else
					DrawUtils::SetColor(0.35f, 0.24f, 0.4f, math);
				DrawUtils::drawLine3D(origin, tracePos, true);
			}
			break;
		case 458:
			if (barrels) {
				DrawUtils::SetColor(0.9f, 0.8f, 0.3f, math);  // Barrel
				DrawUtils::drawLine3D(origin, tracePos, true);
			}
			break;
		}
	}
}

void ChestESP::onTick(GameMode* gm) {
	if (Mode.selected < 0) {
		Mode.selected = 0;
	}

	// Swap list
	auto listLock = Game.lockChestList();
	auto& chestList = Game.getChestList();
	auto ourListLock = std::scoped_lock(this->listLock);

	this->bufferedChestList.clear();
	this->bufferedChestList.insert(this->bufferedChestList.end(), chestList.begin(), chestList.end());
	chestList.clear();
}