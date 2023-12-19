#include "KitCommand.h"

#include "../../../SDK/Mojangson.h"
#include "../../Module/ModuleManager.h"

KitCommand::KitCommand() : IMCCommand("kitcommand", "obtain pre made kits with goodies", " <unfair/fair/legit/cunfair/cfair/clegit>") {
	registerAlias("kit");
}

KitCommand::~KitCommand() {
}

bool KitCommand::execute(std::vector<std::string>* args) {
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

	ItemStack* item;
	try {
		if (type == "unfair" || type == "uf") {
			auto regItem = Game.getItemRegistry()->lookUpByName("shulker_box");
			item = new ItemStack(*regItem, 1, 0);
			std::string tag = R"({Count:1b,Damage:0s,Name:"minecraft:shulker_box",tag:{minecraft:keep_on_death:1b,Items:[{Count:1b,Damage:0s,Name:"minecraft:netherite_helmet",Slot:0b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Helmet"},ench:[{id:0s,lvl:32767s},{id:28s,lvl:1s},{id:1s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:8s,lvl:32767s},{id:5s,lvl:32767s},{id:6s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_chestplate",Slot:1b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Chestplate"},ench:[{id:5s,lvl:32767s},{id:3s,lvl:32767s},{id:0s,lvl:32767s},{id:1s,lvl:32767s},{id:4s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_leggings",Slot:2b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Leggings"},ench:[{id:37s,lvl:32767s},{id:5s,lvl:32767s},{id:1s,lvl:32767s},{id:0s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_boots",Slot:3b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Boots"},ench:[{id:5s,lvl:32767s},{id:2s,lvl:32767s},{id:7s,lvl:32767s},{id:4s,lvl:32767s},{id:0s,lvl:32767s},{id:3s,lvl:32767s},{id:1s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_sword",Slot:4b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Sword"},ench:[{id:9s,lvl:32767s},{id:13s,lvl:32767s},{id:14s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_axe",Slot:5b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Axe"},ench:[{id:9s,lvl:32767s},{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_pickaxe",Slot:6b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Pickaxe"},ench:[{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_shovel",Slot:7b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Shovel"},ench:[{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_hoe",Slot:8b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Hoe"},ench:[{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:elytra",Slot:9b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lElytra"},ench:[{id:0s,lvl:32767s},{id:1s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:5s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:64b,Damage:0s,Name:"minecraft:fireworks",Slot:10b,tag:{minecraft:keep_on_death:1b,Fireworks:{Explosions:[],Flight:-1b}}},{Count:64b,Damage:4s,Name:"minecraft:respawn_anchor",Slot:11b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:cooked_beef",Slot:12b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:appleenchanted",Slot:13b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:obsidian",Slot:14b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:end_crystal",Slot:15b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:2s,Name:"minecraft:emptymap",Slot:16b,tag:{minecraft:keep_on_death:1b,map_scale:3}},{Count:1b,Damage:0s,Name:"minecraft:flint_and_steel",Slot:17b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lFlint and Steel"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shears",Slot:18b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lShears"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:totem",Slot:19b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:experience_bottle",Slot:20b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:25s,Name:"minecraft:arrow",Slot:21b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:0s,Name:"minecraft:turtle_helmet",Slot:22b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lTurtle Shell"},ench:[{id:0s,lvl:32767s},{id:1s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:5s,lvl:32767s},{id:8s,lvl:32767s},{id:6s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shield",Slot:23b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lShield"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:bow",Slot:24b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lBow"},ench:[{id:21s,lvl:32767s},{id:22s,lvl:32767s},{id:19s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:25b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lTrident"},ench:[{id:29s,lvl:32767s},{id:9s,lvl:32767s},{id:14s,lvl:32767s},{id:13s,lvl:32767s},{id:31s,lvl:3s},{id:28s,lvl:1s},{id:32s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:26b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lTrident"},ench:[{id:13s,lvl:32767s},{id:14s,lvl:32767s},{id:9s,lvl:32767s},{id:29s,lvl:32767s},{id:28s,lvl:1s},{id:30s,lvl:32767s}]}}],display:{Lore:["§1§lMade With Horion Client"],Name:"§1§l32K Horion Kit"},ench:[{id:28s,lvl:1s}]}})";
			item->fromTag(*Mojangson::Parse(tag));
		} else if (type == "fair" || type == "fr") {
			auto regItem = Game.getItemRegistry()->lookUpByName("shulker_box");
			item = new ItemStack(*regItem, 1, 0);
			std::string tag = R"({Count:1b,Damage:1s,Name:"minecraft:shulker_box",tag:{minecraft:keep_on_death:1b,Items:[{Count:1b,Damage:0s,Name:"minecraft:netherite_helmet",Slot:0b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Helmet"},ench:[{id:0s,lvl:4s},{id:28s,lvl:1s},{id:1s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s},{id:8s,lvl:1s},{id:5s,lvl:3s},{id:6s,lvl:3s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_chestplate",Slot:1b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Chestplate"},ench:[{id:5s,lvl:3s},{id:28s,lvl:1s},{id:3s,lvl:4s},{id:0s,lvl:4s},{id:1s,lvl:4s},{id:4s,lvl:4s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_leggings",Slot:2b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Leggings"},ench:[{id:37s,lvl:3s},{id:5s,lvl:3s},{id:28s,lvl:1s},{id:1s,lvl:4s},{id:0s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_boots",Slot:3b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Boots"},ench:[{id:5s,lvl:3s},{id:2s,lvl:4s},{id:7s,lvl:3s},{id:4s,lvl:4s},{id:0s,lvl:4s},{id:3s,lvl:4s},{id:1s,lvl:4s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_sword",Slot:4b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Sword"},ench:[{id:9s,lvl:5s},{id:11s,lvl:5s},{id:10s,lvl:5s},{id:13s,lvl:2s},{id:14s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_axe",Slot:5b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Axe"},ench:[{id:9s,lvl:5s},{id:11s,lvl:5s},{id:10s,lvl:5s},{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_pickaxe",Slot:6b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Pickaxe"},ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_shovel",Slot:7b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Shovel"},ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_hoe",Slot:8b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Hoe"},ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:elytra",Slot:9b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lElytra"},ench:[{id:0s,lvl:4s},{id:1s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:64b,Damage:0s,Name:"minecraft:fireworks",Slot:10b,tag:{minecraft:keep_on_death:1b,Fireworks:{Explosions:[],Flight:3b}}},{Count:64b,Damage:4s,Name:"minecraft:respawn_anchor",Slot:11b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:cooked_beef",Slot:12b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:appleenchanted",Slot:13b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:obsidian",Slot:14b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:end_crystal",Slot:15b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:2s,Name:"minecraft:emptymap",Slot:16b,tag:{minecraft:keep_on_death:1b,map_scale:3}},{Count:1b,Damage:0s,Name:"minecraft:flint_and_steel",Slot:17b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lFlint and Steel"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shears",Slot:18b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lShears"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:totem",Slot:19b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:experience_bottle",Slot:20b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:25s,Name:"minecraft:arrow",Slot:21b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:0s,Name:"minecraft:turtle_helmet",Slot:22b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lTurtle Shell"},ench:[{id:0s,lvl:4s},{id:1s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s},{id:5s,lvl:3s},{id:8s,lvl:1s},{id:6s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shield",Slot:23b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lShield"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:bow",Slot:24b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lBow"},ench:[{id:21s,lvl:1s},{id:22s,lvl:1s},{id:19s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:25b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lTrident"},ench:[{id:29s,lvl:5s},{id:11s,lvl:5s},{id:10s,lvl:5s},{id:9s,lvl:5s},{id:14s,lvl:3s},{id:13s,lvl:2s},{id:28s,lvl:1s},{id:31s,lvl:3s},{id:32s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:26b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lTrident"},ench:[{id:11s,lvl:5s},{id:10s,lvl:5s},{id:13s,lvl:2s},{id:14s,lvl:3s},{id:9s,lvl:5s},{id:29s,lvl:5s},{id:28s,lvl:1s},{id:30s,lvl:3s}]}}],display:{Lore:["§2§lMade With Horion Client"],Name:"§2§lOP Legit Horion Kit"},ench:[{id:28s,lvl:1s}]}})";
			item->fromTag(*Mojangson::Parse(tag));
		} else if (type == "legit" || type == "lt") {
			auto regItem = Game.getItemRegistry()->lookUpByName("shulker_box");
			item = new ItemStack(*regItem, 1, 0);
			std::string tag = R"({Count:1b,Damage:2s,Name:"minecraft:shulker_box",tag:{Items:[{Count:1b,Damage:0s,Name:"minecraft:netherite_helmet",Slot:0b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:5s,lvl:3s},{id:8s,lvl:1s},{id:6s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_chestplate",Slot:1b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_leggings",Slot:2b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:37s,lvl:3s},{id:0s,lvl:4s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_boots",Slot:3b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:5s,lvl:3s},{id:2s,lvl:4s},{id:7s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_sword",Slot:4b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:9s,lvl:5s},{id:14s,lvl:3s},{id:13s,lvl:2s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_axe",Slot:5b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:9s,lvl:5s},{id:18s,lvl:3s},{id:15s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_pickaxe",Slot:6b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_shovel",Slot:7b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:18s,lvl:3s},{id:15s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_hoe",Slot:8b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:18s,lvl:3s},{id:15s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:elytra",Slot:9b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:64b,Damage:0s,Name:"minecraft:fireworks",Slot:10b,WasPickedUp:0b,tag:{Fireworks:{Explosions:[],Flight:3b}}},{Count:64b,Damage:4s,Name:"minecraft:respawn_anchor",Slot:11b},{Count:64b,Damage:0s,Name:"minecraft:cooked_beef",Slot:12b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:appleenchanted",Slot:13b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:obsidian",Slot:14b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:end_crystal",Slot:15b,WasPickedUp:0b},{Count:1b,Damage:2s,Name:"minecraft:emptymap",Slot:16b,WasPickedUp:0b,tag:{map_scale:3}},{Count:1b,Damage:0s,Name:"minecraft:flint_and_steel",Slot:17b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shears",Slot:18b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:totem",Slot:19b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:experience_bottle",Slot:20b,WasPickedUp:0b},{Count:64b,Damage:25s,Name:"minecraft:arrow",Slot:21b,WasPickedUp:0b},{Count:1b,Damage:0s,Name:"minecraft:turtle_helmet",Slot:22b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:8s,lvl:1s},{id:6s,lvl:3s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shield",Slot:23b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:bow",Slot:24b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:21s,lvl:1s},{id:19s,lvl:5s},{id:28s,lvl:1s},{id:22s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:25b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:29s,lvl:5s},{id:28s,lvl:1s},{id:31s,lvl:3s},{id:32s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:26b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:29s,lvl:5s},{id:30s,lvl:3s},{id:28s,lvl:1s}]}}]}})";
			item->fromTag(*Mojangson::Parse(tag));
		}

		else if (type == "cunfair" || type == "cuf") {
			auto regItem = Game.getItemRegistry()->lookUpByName("chest");
			item = new ItemStack(*regItem, 1, 0);
			std::string tag = R"({Count:1b,Damage:0s,Name:"minecraft:chest",tag:{minecraft:keep_on_death:1b,Items:[{Count:1b,Damage:0s,Name:"minecraft:netherite_helmet",Slot:0b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Helmet"},ench:[{id:0s,lvl:32767s},{id:28s,lvl:1s},{id:1s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:8s,lvl:32767s},{id:5s,lvl:32767s},{id:6s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_chestplate",Slot:1b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Chestplate"},ench:[{id:5s,lvl:32767s},{id:3s,lvl:32767s},{id:0s,lvl:32767s},{id:1s,lvl:32767s},{id:4s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_leggings",Slot:2b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Leggings"},ench:[{id:37s,lvl:32767s},{id:5s,lvl:32767s},{id:1s,lvl:32767s},{id:0s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_boots",Slot:3b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Boots"},ench:[{id:5s,lvl:32767s},{id:2s,lvl:32767s},{id:7s,lvl:32767s},{id:4s,lvl:32767s},{id:0s,lvl:32767s},{id:3s,lvl:32767s},{id:1s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_sword",Slot:4b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Sword"},ench:[{id:9s,lvl:32767s},{id:13s,lvl:32767s},{id:14s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_axe",Slot:5b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Axe"},ench:[{id:9s,lvl:32767s},{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_pickaxe",Slot:6b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Pickaxe"},ench:[{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_shovel",Slot:7b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Shovel"},ench:[{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_hoe",Slot:8b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lNetherite Hoe"},ench:[{id:16s,lvl:32767s},{id:28s,lvl:1s},{id:15s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:elytra",Slot:9b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lElytra"},ench:[{id:0s,lvl:32767s},{id:1s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:5s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:64b,Damage:0s,Name:"minecraft:fireworks",Slot:10b,tag:{minecraft:keep_on_death:1b,Fireworks:{Explosions:[],Flight:-1b}}},{Count:64b,Damage:4s,Name:"minecraft:respawn_anchor",Slot:11b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:cooked_beef",Slot:12b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:appleenchanted",Slot:13b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:obsidian",Slot:14b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:end_crystal",Slot:15b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:2s,Name:"minecraft:emptymap",Slot:16b,tag:{minecraft:keep_on_death:1b,map_scale:3}},{Count:1b,Damage:0s,Name:"minecraft:flint_and_steel",Slot:17b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lFlint and Steel"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shears",Slot:18b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lShears"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:totem",Slot:19b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:experience_bottle",Slot:20b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:25s,Name:"minecraft:arrow",Slot:21b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:0s,Name:"minecraft:turtle_helmet",Slot:22b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lTurtle Shell"},ench:[{id:0s,lvl:32767s},{id:1s,lvl:32767s},{id:3s,lvl:32767s},{id:4s,lvl:32767s},{id:5s,lvl:32767s},{id:8s,lvl:32767s},{id:6s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shield",Slot:23b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lShield"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:bow",Slot:24b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lBow"},ench:[{id:21s,lvl:32767s},{id:22s,lvl:32767s},{id:19s,lvl:32767s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:25b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lTrident"},ench:[{id:29s,lvl:32767s},{id:9s,lvl:32767s},{id:14s,lvl:32767s},{id:13s,lvl:32767s},{id:31s,lvl:3s},{id:28s,lvl:1s},{id:32s,lvl:32767s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:26b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§3§lMade With Horion Client"],Name:"§3§lTrident"},ench:[{id:13s,lvl:32767s},{id:14s,lvl:32767s},{id:9s,lvl:32767s},{id:29s,lvl:32767s},{id:28s,lvl:1s},{id:30s,lvl:32767s}]}}],display:{Lore:["§1§lMade With Horion Client"],Name:"§1§l32K Horion Kit"},ench:[{id:28s,lvl:1s}]}})";
			item->fromTag(*Mojangson::Parse(tag));
		} else if (type == "cfair" || type == "cfr") {
			auto regItem = Game.getItemRegistry()->lookUpByName("chest");
			item = new ItemStack(*regItem, 1, 0);
			std::string tag = R"({Count:1b,Damage:1s,Name:"minecraft:chest",tag:{minecraft:keep_on_death:1b,Items:[{Count:1b,Damage:0s,Name:"minecraft:netherite_helmet",Slot:0b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Helmet"},ench:[{id:0s,lvl:4s},{id:28s,lvl:1s},{id:1s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s},{id:8s,lvl:1s},{id:5s,lvl:3s},{id:6s,lvl:3s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_chestplate",Slot:1b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Chestplate"},ench:[{id:5s,lvl:3s},{id:28s,lvl:1s},{id:3s,lvl:4s},{id:0s,lvl:4s},{id:1s,lvl:4s},{id:4s,lvl:4s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_leggings",Slot:2b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Leggings"},ench:[{id:37s,lvl:3s},{id:5s,lvl:3s},{id:28s,lvl:1s},{id:1s,lvl:4s},{id:0s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_boots",Slot:3b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Boots"},ench:[{id:5s,lvl:3s},{id:2s,lvl:4s},{id:7s,lvl:3s},{id:4s,lvl:4s},{id:0s,lvl:4s},{id:3s,lvl:4s},{id:1s,lvl:4s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_sword",Slot:4b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Sword"},ench:[{id:9s,lvl:5s},{id:11s,lvl:5s},{id:10s,lvl:5s},{id:13s,lvl:2s},{id:14s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_axe",Slot:5b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Axe"},ench:[{id:9s,lvl:5s},{id:11s,lvl:5s},{id:10s,lvl:5s},{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_pickaxe",Slot:6b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Pickaxe"},ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_shovel",Slot:7b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Shovel"},ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_hoe",Slot:8b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lNetherite Hoe"},ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:elytra",Slot:9b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lElytra"},ench:[{id:0s,lvl:4s},{id:1s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:64b,Damage:0s,Name:"minecraft:fireworks",Slot:10b,tag:{minecraft:keep_on_death:1b,Fireworks:{Explosions:[],Flight:3b}}},{Count:64b,Damage:4s,Name:"minecraft:respawn_anchor",Slot:11b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:cooked_beef",Slot:12b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:appleenchanted",Slot:13b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:obsidian",Slot:14b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:end_crystal",Slot:15b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:2s,Name:"minecraft:emptymap",Slot:16b,tag:{minecraft:keep_on_death:1b,map_scale:3}},{Count:1b,Damage:0s,Name:"minecraft:flint_and_steel",Slot:17b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lFlint and Steel"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shears",Slot:18b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lShears"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:totem",Slot:19b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:0s,Name:"minecraft:experience_bottle",Slot:20b,tag:{minecraft:keep_on_death:1b}},{Count:64b,Damage:25s,Name:"minecraft:arrow",Slot:21b,tag:{minecraft:keep_on_death:1b}},{Count:1b,Damage:0s,Name:"minecraft:turtle_helmet",Slot:22b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lTurtle Shell"},ench:[{id:0s,lvl:4s},{id:1s,lvl:4s},{id:3s,lvl:4s},{id:4s,lvl:4s},{id:5s,lvl:3s},{id:8s,lvl:1s},{id:6s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shield",Slot:23b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lShield"},ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:bow",Slot:24b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lBow"},ench:[{id:21s,lvl:1s},{id:22s,lvl:1s},{id:19s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:25b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lTrident"},ench:[{id:29s,lvl:5s},{id:11s,lvl:5s},{id:10s,lvl:5s},{id:9s,lvl:5s},{id:14s,lvl:3s},{id:13s,lvl:2s},{id:28s,lvl:1s},{id:31s,lvl:3s},{id:32s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:26b,tag:{minecraft:keep_on_death:1b,Damage:0,Unbreakable:1b,display:{Lore:["§a§lMade With Horion Client"],Name:"§a§lTrident"},ench:[{id:11s,lvl:5s},{id:10s,lvl:5s},{id:13s,lvl:2s},{id:14s,lvl:3s},{id:9s,lvl:5s},{id:29s,lvl:5s},{id:28s,lvl:1s},{id:30s,lvl:3s}]}}],display:{Lore:["§2§lMade With Horion Client"],Name:"§2§lOP Legit Horion Kit"},ench:[{id:28s,lvl:1s}]}})";
			item->fromTag(*Mojangson::Parse(tag));
		} else if (type == "clegit" || type == "clt") {
			auto regItem = Game.getItemRegistry()->lookUpByName("chest");
			item = new ItemStack(*regItem, 1, 0);
			std::string tag = R"({Count:1b,Damage:0s,Name:"minecraft:chest",tag:{Items:[{Count:1b,Damage:0s,Name:"minecraft:netherite_helmet",Slot:0b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:5s,lvl:3s},{id:8s,lvl:1s},{id:6s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_chestplate",Slot:1b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_leggings",Slot:2b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:37s,lvl:3s},{id:0s,lvl:4s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_boots",Slot:3b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:5s,lvl:3s},{id:2s,lvl:4s},{id:7s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_sword",Slot:4b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:9s,lvl:5s},{id:14s,lvl:3s},{id:13s,lvl:2s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_axe",Slot:5b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:9s,lvl:5s},{id:18s,lvl:3s},{id:15s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_pickaxe",Slot:6b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:18s,lvl:3s},{id:28s,lvl:1s},{id:15s,lvl:5s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_shovel",Slot:7b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:18s,lvl:3s},{id:15s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:netherite_hoe",Slot:8b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:18s,lvl:3s},{id:15s,lvl:5s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:elytra",Slot:9b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:64b,Damage:0s,Name:"minecraft:fireworks",Slot:10b,WasPickedUp:0b,tag:{Fireworks:{Explosions:[],Flight:3b}}},{Count:64b,Damage:4s,Name:"minecraft:respawn_anchor",Slot:11b},{Count:64b,Damage:0s,Name:"minecraft:cooked_beef",Slot:12b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:appleenchanted",Slot:13b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:obsidian",Slot:14b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:end_crystal",Slot:15b,WasPickedUp:0b},{Count:1b,Damage:2s,Name:"minecraft:emptymap",Slot:16b,WasPickedUp:0b,tag:{map_scale:3}},{Count:1b,Damage:0s,Name:"minecraft:flint_and_steel",Slot:17b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shears",Slot:18b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:totem",Slot:19b,WasPickedUp:0b},{Count:64b,Damage:0s,Name:"minecraft:experience_bottle",Slot:20b,WasPickedUp:0b},{Count:64b,Damage:25s,Name:"minecraft:arrow",Slot:21b,WasPickedUp:0b},{Count:1b,Damage:0s,Name:"minecraft:turtle_helmet",Slot:22b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:0s,lvl:4s},{id:8s,lvl:1s},{id:6s,lvl:3s},{id:5s,lvl:3s},{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:shield",Slot:23b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:28s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:bow",Slot:24b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:21s,lvl:1s},{id:19s,lvl:5s},{id:28s,lvl:1s},{id:22s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:25b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:29s,lvl:5s},{id:28s,lvl:1s},{id:31s,lvl:3s},{id:32s,lvl:1s}]}},{Count:1b,Damage:0s,Name:"minecraft:trident",Slot:26b,WasPickedUp:0b,tag:{Unbreakable:1b,ench:[{id:29s,lvl:5s},{id:30s,lvl:3s},{id:28s,lvl:1s}]}}]}})";
			item->fromTag(*Mojangson::Parse(tag));
		}
		else return false;

		{
			Game.getTransactionManager()->addAction(InventoryAction(Game.getInventory()->getFirstEmptySlot(), nullptr, item));
			Game.getTransactionManager()->addAction(InventoryAction(0, item, nullptr, ContainerID::Inventory, InventorySource::Type::NotImplemented));
			Game.getInventory()->addItemToFirstEmptySlot(item);
		}

	} catch (Mojangson::Exception& e) {
		clientMessageF("%sError creating the item: %s", RED, e.what());
	}

	clientMessageF("%sPlace the item down to retrieve the kit!", GREEN);
	return true;
}