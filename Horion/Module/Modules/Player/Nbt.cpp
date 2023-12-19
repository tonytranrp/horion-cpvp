#include "Nbt.h"

#include "../../../../SDK/Tag.h"
#include "../../../../Utils/Utils.h"

Nbt::Nbt() : IModule(0, Category::PLAYER, "Print NBT tags, right click on mobs (Only works on local worlds!)") {
	this->registerBoolSetting("Pretty Formatting", &prettyFormatting, prettyFormatting);
}

Nbt::~Nbt() {
}

const char* Nbt::getModuleName() {
	return ("Nbt");
}

void Nbt::onTick(GameMode* gm) {
	if (!GameData::canUseMoveKeys()) {
		return;
	}
	Level* pointingStruct = Game.getLevel();

	if (GameData::isRightClickDown()) {  // && Utils::getClipboardText() != this->lastCopy) {
		if (pointingStruct->getLookingAtEntity()) {
			if (!(Game.getRakNetConnector()->serverIp.size() < 1))
				return;
			std::unique_ptr<CompoundTag> tag = std::make_unique<CompoundTag>();
			pointingStruct->getLookingAtEntity()->save(*tag.get());
			std::string str = Mojangson::FromTag(tag.get(), prettyFormatting);
			if (this->lastCopy == str)
				return;
			this->lastCopy = str;
			Utils::setClipboardText(this->lastCopy);
			Game.getGuiData()->displayClientMessageF("%s%s", GREEN, "CompoundTag copied:");
			Game.getGuiData()->displayClientMessage(&str);
		}
	}
}