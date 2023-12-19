#include "NbtCommand.h"

#include "../../../SDK/Mojangson.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

NbtCommand::NbtCommand() : IMCCommand("nbt", "read and write NBT tags to/from your clipboard (You have to point at an entity/block entity)", "<read/readf/save/savef/write/load>") {
	registerAlias("nbtraw");
}

NbtCommand::~NbtCommand() {
}

bool NbtCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);
	bool isRaw = args->at(0) == "nbtraw";
	if (isRaw) {
		assertTrue(args->at(1) == "write");
		assertTrue(args->size() > 2);
	}
	bool formatted = args->at(1) == "readf" || args->at(1) == "savef";

	Level* pointingStruct = Game.getLevel();
	BlockActor* blockActor = Game.getRegion()->getBlockEntity(pointingStruct->getHitResult().block);
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();
	ItemStack* item = Game.getLocalPlayer()->getSelectedItem();

	if (args->at(1) == "read" || args->at(1) == "save" || formatted) {
		std::unique_ptr<CompoundTag> tag = std::make_unique<CompoundTag>();
		std::string builtStr;

		if ((args->at(1) == "save" || args->at(1) == "savef") && item) {
			auto* boy = new CompoundTag();
			item->save(&boy);
			builtStr = Mojangson::FromTag(boy, formatted);
			delete boy;
		} else {
			if (pointingStruct->getLookingAtEntity()) {
				if (Game.getRakNetConnector()->serverIp.size() >= 1) {
					clientMessageF("%sNBT tags for mobs only works in local world!", RED);
					return true;
				}
				pointingStruct->getLookingAtEntity()->save(*tag.get());
				builtStr = Mojangson::FromTag(tag.get(), formatted);
			} else if (blockActor) {
				blockActor->save(tag.get());
				builtStr = Mojangson::FromTag(tag.get(), formatted);
			} else if (item && item->tag) {
				builtStr = Mojangson::FromTag(item->tag, formatted);
			} else {
				clientMessageF("%sCouldn't find NBT tags!", RED);
				return true;
			}
		}

		Utils::setClipboardText(builtStr);
		clientMessageF("%s%s", GREEN, "CompoundTag copied:");
		clientMessageF("%s%s", formatted ? "\n" : "", builtStr.c_str());
	} else if ((args->at(1) == "write" || args->at(1) == "load") && item) {
		std::string tag;
		if (isRaw) {
			std::ostringstream os;
			for (int i = 2; i < args->size(); i++) {
				if (i > 2)
					os << " ";
				os << args->at(i);
			}

			tag = os.str();
		} else {
			tag = Utils::getClipboardText();
		}

		{
			manager->addAction(InventoryAction(0, nullptr, item, ContainerID::Inventory, InventorySource::Type::NotImplemented));
			manager->addAction(InventoryAction(supplies->selectedHotbarSlot, item, nullptr));
		}

		try {
			if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START && tag.back() == MojangsonToken::COMPOUND_END) {
				if (args->at(1) == "write")
					item->setUserData(std::move(Mojangson::Parse(tag)));
				else if (args->at(1) == "load") {
					item->fromTag(*Mojangson::Parse(tag));
					item->count = 1;
				}
			} else {
				clientMessageF("%sInvalid NBT tag!", RED);
				return true;
			}
		} catch (Mojangson::Exception& e) {
			clientMessageF("%sInvalid NBT tag: %s", RED, e.what());
			return true;
		}

		{
			manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
			manager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
			manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
		}

		clientMessageF("%s%s", GREEN, "Successfully loaded mojangson!");
	} else {
		clientMessageF("%s%s", RED, "Couldn't execute command correctly");
	}

	return true;
}
const char* NbtCommand::getUsage(const char* alias) {
	if (strcmp(alias, "nbtraw") == 0) {
		return "write <nbt>";
	}

	return IMCCommand::getUsage(alias);
}
