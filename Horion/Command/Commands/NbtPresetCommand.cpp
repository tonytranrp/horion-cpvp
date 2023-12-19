#include "NbtPresetCommand.h"
#include "../../../Utils/Logger.h"
#include "../../Config/ConfigManager.h"
#include "../../../SDK/Mojangson.h"

NbtPresetCommand::NbtPresetCommand() : IMCCommand("nbtpreset", "Save and load nbts from files", "<save/load/list/delete> <name>") {
}

NbtPresetCommand::~NbtPresetCommand() {
}

bool NbtPresetCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) != "list")
		assertTrue(args->size() >= 2);

	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();
	ItemStack* item = Game.getLocalPlayer()->getSelectedItem();

	std::string name;
	std::ostringstream os;
	for (int i = 2; i < args->size(); i++) {
		if (i > 2)
			os << " ";
		os << args->at(i);
	}
	name = os.str();

	if (os.str().empty() && args->at(1) != "list") {
		clientMessageF("%sInvalid name!", RED);
		return false;
	}

	auto wide = ConfigManager::GetRoamingFolderPath();
	std::string path(wide.length(), 0);
	std::transform(wide.begin(), wide.end(), path.begin(), [](wchar_t c) {
		1;
		return (char)c;
	});
	path += "\\Horion\\NbtPresets\\";
	std::filesystem::create_directory(path.c_str());
	std::string filePath = path + name + ".txt";

	int num = 1;
	if (args->at(1) == "list") {
		for (auto entry : std::filesystem::directory_iterator(path)) {
			auto name = entry.path().filename().string();
			name = name.substr(0, name.find(".txt"));
			clientMessageF("%i. %s", num, name.c_str());
			num++;
		}
		clientMessageF("%s%sUse these names to load your saved nbt's!", GREEN, ITALIC);
	}

	if (args->at(1) == "delete") {
		if (std::filesystem::exists(filePath)) {
			std::remove(filePath.c_str());
			clientMessageF("%sNBT Successfully Deleted!", GREEN);
		} else
			clientMessageF("%sError Deleting NBT!", RED);
		return true;
	}

	if (args->at(1) == "save") {
		PlayerInventoryProxy* supplies = Game.getSupplies();
		Inventory* inv = Game.getInventory();
		ItemStack* item = Game.getLocalPlayer()->getSelectedItem();
		if (item && item->item != NULL) {
			std::ofstream outfile;
			outfile.open(filePath, std::ofstream::out);
			auto* tag = new CompoundTag();
			item->save(&tag);
			std::string temp = Mojangson::FromTag(tag);
			outfile.write(temp.c_str(), temp.size());
			outfile.close();
			delete tag;
			clientMessageF("%s%s", GREEN, "Preset Saved!");
		} else {
			clientMessageF("%sNo Held Item!", RED);
			return true;
		}
	}

	if (args->at(1) == "load") {
		std::ifstream inFile;
		inFile.open(filePath);
		std::stringstream strStream;
		strStream << inFile.rdbuf();
		std::string tag = strStream.str();

		{
			manager->addAction(InventoryAction(0, nullptr, item, ContainerID::Inventory, InventorySource::Type::NotImplemented));
			manager->addAction(InventoryAction(supplies->selectedHotbarSlot, item, nullptr));
		}

		try {
			if (tag.size() > 1 && tag.front() == MojangsonToken::COMPOUND_START && tag.back() == MojangsonToken::COMPOUND_END) {
				if (args->at(1) == "load") {
					item->fromTag(*Mojangson::Parse(tag));
					item->count = 1;
				}
			} else {
				clientMessageF("%sInvalid NBT tag!", RED);
				return true;
			}
		} catch (Mojangson::Exception& e) {
			clientMessageF("%sInvalid NBT tag: %s", RED, e.what());
		}

		{
			manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
			manager->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
			manager->addAction(InventoryAction(supplies->selectedHotbarSlot, nullptr, item));
			clientMessageF("%s%s", GREEN, "Preset Loaded!");
		}
	}
	return true;
}