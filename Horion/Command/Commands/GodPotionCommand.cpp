#include "GodPotionCommand.h"

#include "../../../SDK/Mojangson.h"
#include "../../Module/ModuleManager.h"

GodPotionCommand::GodPotionCommand() : IMCCommand("godpotion", "Spawn an area effect cloud that gives you potion effects", " <beehive/movingblock/strength>") {
	registerAlias("godpot");
}

GodPotionCommand::~GodPotionCommand() {
}

bool GodPotionCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);

	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	std::string cmd = os.str();

	std::string type = args->at(1);
	std::transform(type.begin(), type.end(), type.begin(), tolower);

	ItemStack* stack;
	
	try {
		if (type == "beehive" || type == "bh") {
			Item* tempItem = Game.getItemRegistry()->lookUpByName("beehive");
			stack = new ItemStack(*tempItem, 1, 0);
			std::string tag = R"({"Block":{"name":"minecraft:beehive","states":{"direction":0,"honey_level":0},"version":2561},"Count":1b,"Damage":0s,"Name":"minecraft:beehive","WasPickedUp":0b,"tag":{"Occupants":[{"ActorIdentifier":"minecraft:area_effect_cloud<>","SaveData":{"Duration":60,"ParticleId":61,"Pos":[],"Radius":2.000000f,"identifier":"minecraft:area_effect_cloud","mobEffects":[{"Amplifier":254b,"Duration":30000000,"Id":10b},{"Amplifier":254b,"Duration":30000000,"Id":11b},{"Amplifier":254b,"Duration":30000000,"Id":12b},{"Amplifier":254b,"Duration":30000000,"Id":13b},{"Ambient":0b,"Amplifier":4b,"Duration":30000000,"Id":21b},{"Amplifier":254b,"Duration":30000000,"Id":22b},{"Amplifier":254b,"Duration":30000000,"Id":29b}]},"TicksLeftToStay":0}],"display":{"Lore":["Â§bÂ§lMade With Horion Client"],"Name":"Â§gÂ§lBeehive God Pot"},"ench":[{"id":28s,"lvl":1s}]}})";
			stack->fromTag(*Mojangson::Parse(tag));

		}
		else if (type == "strength" || type == "st") {
			Item* tempItem = Game.getItemRegistry()->lookUpByName("beehive");
			stack = new ItemStack(*tempItem, 1, 0);
			std::string tag = R"({"Block":{"name":"minecraft:beehive","states":{"direction":0,"honey_level":0},"version":2561},"Count":1b,"Damage":0s,"Name":"minecraft:beehive","WasPickedUp":0b,"tag":{"Occupants":[{"ActorIdentifier":"minecraft:area_effect_cloud<>","SaveData":{"Duration":60,"ParticleId":61,"Pos":[],"Radius":2.000000f,"identifier":"minecraft:area_effect_cloud","mobEffects":[{"Amplifier":25b,"Duration":30000000,"Id":5b},{"Amplifier":254b,"Duration":30000000,"Id":10b},{"Amplifier":254b,"Duration":30000000,"Id":11b},{"Amplifier":254b,"Duration":30000000,"Id":12b},{"Amplifier":254b,"Duration":30000000,"Id":13b},{"Ambient":0b,"Amplifier":4b,"Duration":30000000,"Id":21b},{"Amplifier":254b,"Duration":30000000,"Id":22b},{"Amplifier":254b,"Duration":30000000,"Id":29b}]},"TicksLeftToStay":0}],"display":{"Lore":["Â§bÂ§lMade With Horion Client"],"Name":"Â§gÂ§lBeehive Strength Pot"},"ench":[{"id":28s,"lvl":1s}]}})";
			stack->fromTag(*Mojangson::Parse(tag));
		}
		else if (type == "movingblock" || type == "mb") {
			Item* tempItem = Game.getItemRegistry()->lookUpByName("movingblock");
			stack = new ItemStack(*tempItem, 1, 0);
			std::string tag = R"({"Block":{"name":"minecraft:moving_block","states":{},"version":2561},"Count":1b,"Damage":0s,"Name":"minecraft:moving_block","WasPickedUp":0b,"tag":{"display":{"Lore":["Â§bÂ§lMade With Horion Client"],"Name":"Â§gÂ§lMovingblock God Pot"},"ench":[{"id":28s,"lvl":1s}],"movingBlock":{"name":"minecraft:air"},"movingEntity":{"Occupants":[{"ActorIdentifier":"minecraft:area_effect_cloud<>","SaveData":{"Duration":60,"ParticleId":61,"Pos":[],"Radius":2.000000f,"identifier":"minecraft:area_effect_cloud","mobEffects":[{"Amplifier":254b,"Duration":30000000,"Id":10b},{"Amplifier":254b,"Duration":30000000,"Id":11b},{"Amplifier":254b,"Duration":30000000,"Id":12b},{"Amplifier":254b,"Duration":30000000,"Id":13b},{"Ambient":0b,"Amplifier":4b,"Duration":30000000,"Id":21b},{"Amplifier":254b,"Duration":30000000,"Id":22b},{"Amplifier":254b,"Duration":30000000,"Id":29b}]},"TicksLeftToStay":0}],"id":"Beehive"},"pistonPosX":0,"pistonPosY":0,"pistonPosZ":0}})";
			stack->fromTag(*Mojangson::Parse(tag));
		} else
			return false;

		Game.getTransactionManager()->addAction(InventoryAction(Game.getInventory()->getFirstEmptySlot(), nullptr, stack));
		Game.getTransactionManager()->addAction(InventoryAction(0, stack, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
		Game.getInventory()->addItemToFirstEmptySlot(stack);

	} catch (Mojangson::Exception& e) {
		clientMessageF("%sError creating the item: %s", RED, e.what());
	}

	clientMessageF("%sPlace the item down and stand in the aoe to get the effects!", GREEN);
	return true;
}