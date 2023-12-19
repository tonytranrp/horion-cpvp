#include "AutoArmor.h"

#include "../../../../Utils/Logger.h"
#include "../../../../Utils/Utils.h"

class ArmorStruct {
public:
	ArmorStruct(ItemStack* item, ArmorItem* yot, int slot, std::string container) {
		armor = yot;
		m_slot = slot;
		m_item = item;
		m_container = container;
	}
	bool isEqual(ArmorStruct& src) {
		if (this->m_item->getArmorValueWithEnchants() == src.m_item->getArmorValueWithEnchants())
			return true;
		else
			return false;
	}

	bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
		return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
	}
	ArmorItem* armor = nullptr;
	ItemStack* m_item = nullptr;
	int m_slot = 0;
	std::string m_container;
};

AutoArmor::AutoArmor() : IModule(0, Category::PLAYER, "Automatically equips the best armor") {
	Mode.addEntry(EnumEntry("Vanilla", 0));
	Mode.addEntry(EnumEntry("Open Inv", 1));
	registerEnumSetting("Mode", &Mode, 0);
}

AutoArmor::~AutoArmor() {
}

const char* AutoArmor::getModuleName() {
	return ("AutoArmor");
}

void AutoArmor::onTick(GameMode* gm) {
	if (Mode.selected != 0 && Mode.selected != 1)
		Mode.selected = 0;
	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();

	InventoryAction* first = nullptr;
	InventoryAction* second = nullptr;
	static ItemStack* emptyItemStack = nullptr;

	if (emptyItemStack == 0x0) {
		uintptr_t sigOffset = FindSignature("48 8D 3D ? ? ? ? ? 80 B8 ? ? ? ? ? 75 ? 49 8B 88 ? ? ? ? 48 8B 11 48 8B 42 28 41 8B 50 10");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		emptyItemStack = reinterpret_cast<ItemStack*>(sigOffset + offset + /*length of instruction*/ 7);
	}

	std::vector<ArmorStruct> armorList;

	struct CompareArmorStruct {
		bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
			return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
		}
	};

	for (int i = 0; i < 4; i++) {
		for (int n = 0; n < 36; n++) {
			ItemStack* stack = inv->getItemStack(n);
			if (stack->item != NULL && (*stack->item)->isArmor() && reinterpret_cast<ArmorItem*>(*stack->item)->ArmorSlot == i) {
				armorList.push_back(ArmorStruct(stack, reinterpret_cast<ArmorItem*>(*stack->item), n, ""));
			}
		}

		if (gm->player->getArmor(i)->item)
			armorList.push_back(ArmorStruct(gm->player->getArmor(i), reinterpret_cast<ArmorItem*>(*gm->player->getArmor(i)->item), i, ""));

		if (armorList.size() > 0) {
			std::sort(armorList.begin(), armorList.end(), CompareArmorStruct());
			ItemStack* armorItem = gm->player->getArmor(i);

			if (armorItem->item && (ArmorStruct(armorItem, reinterpret_cast<ArmorItem*>(*armorItem->item), 0, "").isEqual(armorList[0])) == false) {
				if (Mode.selected == 1) {
					InteractPacket openInv(InteractPacket::Action::OpenInventory, Game.getLocalPlayer()->getRuntimeId(), *Game.getLocalPlayer()->getPos());
					Game.getLoopbackPacketSender()->sendToServer(&openInv);
					setArmor = true;
				} else {
					int slot = inv->getFirstEmptySlot();
					first = new InventoryAction(i, armorItem, nullptr, ContainerID::Armor);
					second = new InventoryAction(slot, nullptr, armorItem);

					*Game.getLocalPlayer()->getArmor(i) = *emptyItemStack;
					*inv->getItemStack(slot) = *armorItem;

					manager->addAction(*first);
					manager->addAction(*second);

					delete first;
					delete second;
					inv->removeItem(slot, slot);  // removes ghost item in inv

					first = new InventoryAction(armorList[0].m_slot, armorList[0].m_item, nullptr);
					second = new InventoryAction(i, nullptr, armorList[0].m_item, ContainerID::Armor);

					manager->addAction(*first);
					manager->addAction(*second);

					delete first;
					delete second;
					inv->removeItem(armorList[0].m_slot, armorList[0].m_slot);  // removes ghost item in inv
				}
			}
			if (!armorItem->item) {
				if (Mode.selected == 1) {
					InteractPacket openInv(InteractPacket::Action::OpenInventory, Game.getLocalPlayer()->getRuntimeId(), *Game.getLocalPlayer()->getPos());
					Game.getLoopbackPacketSender()->sendToServer(&openInv);
					setArmor = true;
				} else {
					first = new InventoryAction(armorList[0].m_slot, armorList[0].m_item, nullptr);
					second = new InventoryAction(i, nullptr, armorList[0].m_item, ContainerID::Armor);

					manager->addAction(*first);
					manager->addAction(*second);

					delete first;
					delete second;
					inv->removeItem(armorList[0].m_slot, armorList[0].m_slot);  // removes ghost item in inv
				}
			}
			armorList.clear();
		}
	}
	armorList.clear();
}

void AutoArmor::ScreenController_tick(ScreenController* c) {
	if (Mode.selected == 1 && setArmor && c->isInventory()) {
		PlayerInventoryProxy* supplies = Game.getSupplies();
		Inventory* inv = Game.getInventory();
		std::vector<ArmorStruct> armorList;

		struct CompareArmorStruct {
			bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
				return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
			}
		};

		for (int i = 0; i < 4; i++) {
			for (int in = 0; in < 28; in++) {
				ItemStack* stack = c->_getItemStack(std::string("inventory_items"), in);
				if (stack->item != NULL && (*stack->item)->isArmor() && reinterpret_cast<ArmorItem*>(*stack->item)->ArmorSlot == i) {
					armorList.push_back(ArmorStruct(stack, reinterpret_cast<ArmorItem*>(*stack->item), in, "inventory_items"));
				}
			}
			for (int hi = 0; hi < 9; hi++) {
				ItemStack* stack = c->_getItemStack(std::string("hotbar_items"), hi);
				if (stack->item != NULL && (*stack->item)->isArmor() && reinterpret_cast<ArmorItem*>(*stack->item)->ArmorSlot == i) {
					armorList.push_back(ArmorStruct(stack, reinterpret_cast<ArmorItem*>(*stack->item), hi, "hotbar_items"));
				}
			}

			if (Game.getLocalPlayer()->getArmor(i)->item)
				armorList.push_back(ArmorStruct(Game.getLocalPlayer()->getArmor(i), reinterpret_cast<ArmorItem*>(*Game.getLocalPlayer()->getArmor(i)->item), i, "armor_items"));

			if (armorList.size() > 0) {
				std::sort(armorList.begin(), armorList.end(), CompareArmorStruct());
				ItemStack* armorItem = Game.getLocalPlayer()->getArmor(i);

				if (armorItem->item && (ArmorStruct(armorItem, reinterpret_cast<ArmorItem*>(*armorItem->item), 0, "armor_items").isEqual(armorList[0])) == false) {
					c->handleSwap("armor_items", i, armorList[0].m_container, armorList[0].m_slot);
				}
				if (!armorItem->item) {
					c->handleAutoPlace(0x7FFFFFFF, armorList[0].m_container, armorList[0].m_slot);
				}
			}
			armorList.clear();
		}
		armorList.clear();
		setArmor = false;
		c->tryExit();
	}
}