#include "SignEdit.h"
#include "../../../../SDK/Mojangson.h"
SignEdit::SignEdit() : IModule(0, Category::WORLD, "Edits the text on pre-placed signs") {
	registerIntSetting("Range", &this->range, this->range, 1, 15);
}

SignEdit::~SignEdit() {
}

const char* SignEdit::getModuleName() {
	return "SignEdit";
}

void SignEdit::onTick(GameMode* gm) {
	auto plr = Game.getLocalPlayer();
	if (!plr) return;

	auto SignIds = {
		63, 68,    // oak
		436, 437,  // spruce
		441, 442,  // birch
		443, 444,  // jungle
		445, 446,  // acacia
		447, 448,  // dark oak
		505, 507,  // crimson
		506, 508   // warped
	};
	Vec3* pos = Game.getLocalPlayer()->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				Vec3i blockPos = Vec3(x, y, z);
				int blockId = Game.getRegion()->getBlock(blockPos)->toLegacy()->blockId;
				if (blockId == 0) continue;
				bool MoveOn = true;

				for (auto id : SignIds)
					if (id == blockId)
						MoveOn = false;

				if (MoveOn) continue;
				std::stringstream NBTForSign;
				NBTForSign << "{IgnoreLighting:0b,PersistFormatting:1b,SignTextColor:-16777216,";
				NBTForSign << "Text:\"";
				NBTForSign << SignText;
				NBTForSign << "\",TextIgnoreLegacyBugResolved:0b,TextOwner:\"2535436476719391\",id:\"Sign\",isMovable:1b,x:";  // TextOwner is for the minecraft account :flushed:
				NBTForSign << std::to_string(blockPos.x);
				NBTForSign << ",y:";
				NBTForSign << std::to_string(blockPos.y);
				NBTForSign << ",z:";
				NBTForSign << std::to_string(blockPos.z);
				NBTForSign << "}";
				std::unique_ptr<Tag> tag = Mojangson::Parse(NBTForSign.str());
				std::unique_ptr<CompoundTag> CompoundTagSign = std::unique_ptr<CompoundTag>(static_cast<CompoundTag*>(tag.get()));

				BlockActorDataPacket p(blockPos, *CompoundTagSign.get());
				Game.getClientInstance()->getLoopbackPacketSender()->send(&p);
				CompoundTagSign.reset();
				tag.reset();
			}
		}
	}
	this->setEnabled(false);  // spamming this could lead to lag
}