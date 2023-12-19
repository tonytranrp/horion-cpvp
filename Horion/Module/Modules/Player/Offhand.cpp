#include "Offhand.h"

Offhand::Offhand() : IModule(0, Category::PLAYER, "Automatically puts totems or a shield into your offhand") {
	Item.addEntry(EnumEntry("Totem", 0));
	Item.addEntry(EnumEntry("Shield", 1));
	registerEnumSetting("Item", &Item, 0);

	Mode.addEntry(EnumEntry("Vanilla", 0));
	Mode.addEntry(EnumEntry("Inventory", 1));
	registerEnumSetting("Mode", &Mode, 0);
}

Offhand::~Offhand() {
}

const char* Offhand::getModuleName() {
	return ("Offhand");
}

std::string Offhand::getModeText() {
	return Item.GetSelectedEntry().GetName();
}

void Offhand::onTick(GameMode* gm) {
	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	PlayerInventoryProxy* supplies = Game.getSupplies();
	Inventory* inv = Game.getInventory();
	InventoryTransactionManager* manager = Game.getTransactionManager();

	ItemStack* current = Utils::CallVFunc<5, ItemStack*>((void*)(*((uintptr_t**)Game.getLocalPlayer() + 0x7C)), 1);

	if (current->item != NULL) return;

	bool ItemNotInOffhand = false;

	if (current->item == NULL || (*current->item)->itemId != (this->Item.selected == 0 ? 568 : 355))
		ItemNotInOffhand = true;

	if (ItemNotInOffhand) {
		for (int i = 0; i < 36; i++) {
			ItemStack* totem = inv->getItemStack(i);
			if (totem->item != NULL && (*totem->item)->itemId == (this->Item.selected == 0 ? 568 : 355)) {
				if (Mode.selected == 0) {
					InventoryAction first(0, nullptr, totem, ContainerID::Offhand);
					manager->addAction(first);
					inv->removeItem(i, i);
				} else if (Mode.selected == 1) {
					bool hastotem = false;
					for (int i = 0; i < 4 * 9; i++) {
						ItemStack* stack = Game.getInventory()->getItemStack(i);
						if (stack && stack->item != NULL)
							if ((*stack->item)->itemId == (this->Item.selected == 0 ? 568 : 355))
								hastotem = true;
					}
					if (hastotem == false) return;
					InteractPacket openInv(InteractPacket::Action::OpenInventory, lp->getRuntimeId(), *lp->getPos());
					Game.getLoopbackPacketSender()->sendToServer(&openInv);
					setOffhand = true;
				}
			}
		}
	}
}

void Offhand::ScreenController_tick(ScreenController* c) {
	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	ItemStack* current = Utils::CallVFunc<5, ItemStack*>((void*)(*((uintptr_t**)Game.getLocalPlayer() + 0x7C)), 1);

	bool ItemNotInOffhand = false;

	if (current->item == NULL || (*current->item)->itemId != (this->Item.selected == 0 ? 568 : 355))
		ItemNotInOffhand = true;
	
	if (Mode.selected == 1 && setOffhand == true && c->isInventory()) {
		bool hasTotem = !(this->Item.selected == 0 ? 568 : 355);
		std::vector<int> invItems = {};
		std::vector<int> hotbarItems = {};
		for (int i = 9; i < 37; i++) {
			ItemStack* stack = c->_getItemStack(std::string("inventory_items"), i);
			if (stack && stack->item != NULL)
				if ((*stack->item)->itemId == (this->Item.selected == 0 ? 568 : 355))
					if (ItemNotInOffhand) {
						c->handleAutoPlace(0x7FFFFFFF, "inventory_items", i);
						break;
					}
		}
		for (int i = 0; i < 9; i++) {
			ItemStack* stack = c->_getItemStack(std::string("hotbar_items"), i);
			if (stack && stack->item != NULL)
				if ((*stack->item)->itemId == (this->Item.selected == 0 ? 568 : 355))
					if (ItemNotInOffhand) {
						c->handleAutoPlace(0x7FFFFFFF, "hotbar_items", i);
						break;
					}
		}
		c->tryExit();
		setOffhand = false;
	}
}
