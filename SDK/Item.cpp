#include "Item.h"
#include "../Utils/Utils.h"
#include "Tag.h"
#include "../Memory/GameData.h"

ItemStack::ItemStack(const ItemStack &src) {
	memset(this, 0x0, sizeof(ItemStack));
	using ItemStackCopyConstructor_t = void(__cdecl *)(ItemStack *, ItemStack const &);
	static auto ItemStackCopyConstructor = reinterpret_cast<ItemStackCopyConstructor_t>(FindSignature("48 89 4C 24 ? 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B EA"));
	ItemStackCopyConstructor(this, src);
	this->setVtable();
}

ItemStack::ItemStack(const Tag &tag) {
	memset(this, 0x0, sizeof(ItemStack));
	this->setVtable();
	fromTag(tag);
}

void ItemStack::reinit(BlockLegacy &legacy, int count) {
	this->setVtable();
	Utils::CallVFunc<2, void>(this, &legacy, count);
}

void ItemStack::reinit(Item &item, int count, int itemData) {
	this->setVtable();
	Utils::CallVFunc<3, void>(this, &item, count, itemData);
}

void ItemStack::setVtable(void) {
	static uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 05 ? ? ? ? C7 ? ? ? ? ? ? ? ? ? C6 05 ? ? ? ? ? 48 8D 0D ? ? ? ? 48 83 C4");
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	this->vTable = reinterpret_cast<uintptr_t **>(sigOffset + offset + /*length of instruction*/ 7);
}

void ItemStack::fromTag(const Tag &tag) {
	using ItemStackBase__loadItemF = void(__fastcall *)(ItemStack *, Tag const &);
	static ItemStackBase__loadItemF fromTag = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B DA 48 8B F9 48 89 4C 24 ? 33 D2"));
	fromTag(this, tag);
}

void ItemStack::save(CompoundTag **tag) {
	using ItemStackBase__saveF = void(__fastcall *)(ItemStack *, CompoundTag **);
	ItemStackBase__saveF save = reinterpret_cast<ItemStackBase__saveF>(FindSignature("48 89 5C 24 ?? 55 56 57 41 56 41 57 48 8D 6C 24 C9 48 81 EC ?? ?? ?? ?? 48 8B FA 4C 8B F1 48 89 55 0F 45 33 ?? 44 89 7D B7 33 C0"));
	return save(this, tag);
}

void ItemStack::setUserData(std::unique_ptr<Tag> tag) {
	using setUserData_t = void(__fastcall *)(ItemStack *, std::unique_ptr<Tag>);
	setUserData_t setUserData = reinterpret_cast<setUserData_t>(FindSignature("48 89 5C 24 18 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B FA 48 8B F1 48 89 94 24 ? ? ? ? 48 8D 59 ? 48 3B DA"));
	setUserData(this, std::move(tag));
}

void ItemStack::setCustomName(std::string name) {
	using setCustomName_t = void(__fastcall *)(ItemStack *, std::string&);
	setCustomName_t setCustomName = reinterpret_cast<setCustomName_t>(FindSignature("48 89 5C 24 18 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B FA 45 33 E4 45 8B F4 44 89 65 ? 48 8B 41 08"));
	std::string finalName = name;
	setCustomName(this, finalName);
}

void ItemStack::setCustomLore(std::string customLore) {
	using setCustomLore_t = void(__fastcall *)(ItemStack *, std::string **);
	setCustomLore_t setCustomLore = reinterpret_cast<setCustomLore_t>(FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B F2"));
	std::string lore = customLore;
	std::string *pText = &lore;
	std::string *vec[] = {pText, &pText[1], &pText[1]};
	setCustomLore(this, vec);
}

float ItemStack::getDestroySpeed(Block *block) {
	using getDestroySpeed_t = float(__fastcall *)(ItemStack *, Block *);
	static getDestroySpeed_t getDestroySpeed = reinterpret_cast<getDestroySpeed_t>(FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? ? 29 70 D8 4C 8B FA 4C 8B F1 33 ? 89 7D 67 48 8B"));
	return getDestroySpeed(this, block);
}

int32_t ItemStack::getEnchantValue(EnchantType enchantType) {
	using getEnchantsLevel_t = int32_t(__cdecl *)(EnchantType, ItemStack *);
	static auto getEnchantsLevel = reinterpret_cast<getEnchantsLevel_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 0F B6 D9 33 FF 48 8B CA E8 ? ? ? ?"));
	return getEnchantsLevel(enchantType, this);
}

// 48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? ? 0F ? ? 48 8B FA 48 8B F1   EnchantUtils::applyEnchant, contains all funcs needed.
void ItemStack::applyEnchant(EnchantType enchantType, int enchantLevel) {
	auto enchants = this->constructEnchantsFromUserData();
	if (enchants.addEnchant(enchantType, enchantLevel))
		Game.getGuiData()->displayClientMessageF("%sEnchant successful!", GREEN);
	else
		Game.getGuiData()->displayClientMessageF("%sError applying enchant!", RED);
	this->saveEnchantsToUserData(&enchants);
}

ItemEnchants ItemStack::constructEnchantsFromUserData() {
	using constructEnchantsFromUserData_t = ItemEnchants*(__cdecl *)(ItemStack*, ItemEnchants*);
	static auto constructEnchantsFromUserDataFunc = reinterpret_cast<constructEnchantsFromUserData_t>(
		FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B F2 48 8B D9 48 89 54 24 ? ? ? ? ? 24 ? E8 ? ? ? ? ? ? 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9"));
	ItemEnchants enchants;
	constructEnchantsFromUserDataFunc(this, &enchants);
	return enchants;
}

void ItemStack::saveEnchantsToUserData(ItemEnchants* enchants) {
	using saveEnchantsToUserData_t = void(__cdecl *)(ItemStack*, ItemEnchants*);
	static auto saveEnchantsToUserDataFunc = reinterpret_cast<saveEnchantsToUserData_t>(
		FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 56 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B FA 4C 8B C1"));
	saveEnchantsToUserDataFunc(this, enchants);
}

const ItemStack& ItemStack::getEmptyItemStack() {
	static uintptr_t sigOffset = FindSignature("48 8D 3D ? ? ? ? ? 80 B8 ? ? ? ? ? 75 1D 49 8B 88 ? ? ? ? 48 8B");
	int32_t offset = *reinterpret_cast<int32_t *>(sigOffset + 3);
	const auto& emptyItemStack = *reinterpret_cast<ItemStack *>(sigOffset + offset + /*length of instruction*/ 7);
	return emptyItemStack;
}

bool ItemEnchants::addEnchant(EnchantType enchantType, int level, bool allowNonVanilla) {
	using addEnchant_t = bool(__cdecl *)(ItemEnchants *, EnchantmentInstance, bool);
	static addEnchant_t addEnchant = reinterpret_cast<addEnchant_t>(FindSignature("48 89 5C 24 ? 48 89 54 24 ? ? 48 83 EC ? ? ? ? ? 48 8B DA 4C 8B C2 48 8B F9 48 8D 54 24 ? E8 ? ? ? ? 0F B6 CB E8 ? ? ? ? 44 0F BE 44 24 ? 45 85 C0"));
	return addEnchant(this, EnchantmentInstance(enchantType, level), allowNonVanilla);
}

ItemEnchants::ItemEnchants() {
	memset(this, 0x0, sizeof(*this));
}

ItemRegistry* ItemRegistryManager::getItemRegistry() {
	static auto daRegistry = ([] { // static shared ptr instance of registry, safe to cache
		std::weak_ptr<ItemRegistry> out;
		using getItemRegistry_t = std::weak_ptr<ItemRegistry>*(__cdecl *)(std::weak_ptr<ItemRegistry>*);
		auto getItemRegistry = reinterpret_cast<getItemRegistry_t>(FindSignature("40 53 48 83 EC ? 48 83 3D ? ? ? ? ? 48 8B D9 74"));
		getItemRegistry(&out);
		return out.lock().get();
	})();
	return daRegistry;
}

Item *ItemRegistry::getItemFromId(int32_t itemId) {
	auto it = this->idToItemMap.find(itemId);
	if (it != this->idToItemMap.end()) {
		return it->second.get();
	}
	return nullptr;
}

Item *ItemRegistry::lookUpByName(std::string_view name) {
	std::string mut{name};
	constexpr std::string_view mcNamespace{"minecraft:"};
	if (!Utils::stringStartsWith(mut, mcNamespace)) {
		mut.insert(0, mcNamespace);
	}

	HashedString hs(mut);
	auto it = this->nameToItemMap.find(hs);
	if (it != this->nameToItemMap.end()) {
		return it->second.get();
	}
	return nullptr;
}

void ItemDescriptor::fromStack(ItemStack* item) {
	using ItemDescriptor_ctorT = void(*)(ItemDescriptor*, ItemStack*);
	static ItemDescriptor_ctorT ItemDescriptor_ctor = (ItemDescriptor_ctorT)FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B FA 48 8B F1 48 89 4C 24 ? 0F B6 ? ? 48 8D 54 24 ? 48 8B CF");
	ItemDescriptor_ctor(this, item);
}

int32_t EnchantUtils::determineActivation(EnchantType enchantType) {
	switch (enchantType) {
	case EnchantType::Protection:
	case EnchantType::FireProtection:
	case EnchantType::FeatherFalling:
	case EnchantType::BlastProtection:
	case EnchantType::ProjectileProtection:
	case EnchantType::Thorns:
	case EnchantType::Respiration:
	case EnchantType::DepthStrider:
	case EnchantType::AquaAffinity:
	case EnchantType::FrostWalker:
	case EnchantType::SoulSpeed:
	case EnchantType::SwiftSneak:
		return 0;
	case EnchantType::Sharpness:
	case EnchantType::Smite:
	case EnchantType::BaneOfArthropods:
	case EnchantType::FireAspect:
	case EnchantType::Looting:
	case EnchantType::SilkTouch:
	case EnchantType::Unbreaking:
	case EnchantType::Fortune:
	case EnchantType::Flame:
	case EnchantType::LuckOfTheSea:
	case EnchantType::Impaling:
		return 1;
	case EnchantType::Knockback:
	case EnchantType::Efficiency:
	case EnchantType::Power:
	case EnchantType::Punch:
	case EnchantType::Infinity:
	case EnchantType::Lure:
	case EnchantType::Mending:
	case EnchantType::CurseOfBinding:
	case EnchantType::CurseOfVanishing:
	case EnchantType::Riptide:
	case EnchantType::Loyalty:
	case EnchantType::Channeling:
	case EnchantType::Multishot:
	case EnchantType::Piercing:
	case EnchantType::QuickCharge:
		return 2;
	default:
		return 4;
	}
}

bool EnchantUtils::applyUnfilteredEnchant(ItemStack &out, EnchantmentInstance const &newEnchant, bool overwriteDuplicates) {

	auto resultEnchants = out.constructEnchantsFromUserData();
	int32_t activationIndex = determineActivation(newEnchant.enchantType);

	if ((activationIndex >= 0) && (activationIndex <= 2)) {
		
		auto &instanceVectorToWriteTo = resultEnchants.enchants[activationIndex];
		if (overwriteDuplicates) {

			for (int32_t i = 0; i < instanceVectorToWriteTo.size(); i++) {
				if (instanceVectorToWriteTo[i].enchantType == newEnchant.enchantType) {
					instanceVectorToWriteTo[i] = newEnchant;
					out.saveEnchantsToUserData(&resultEnchants);
					return true;
				}
			}
		}
		
		instanceVectorToWriteTo.push_back(newEnchant);
		out.saveEnchantsToUserData(&resultEnchants);
		return true;
	}
	return false;
}

bool EnchantUtils::removeEnchant(ItemStack &out, EnchantType enchantTypeToRemove) {

	auto resultEnchants = out.constructEnchantsFromUserData();
	int32_t activationIndex = determineActivation(enchantTypeToRemove);

	if ((activationIndex >= 0) && (activationIndex <= 2)) {

		auto &instanceVectorToWriteTo = resultEnchants.enchants[activationIndex];
		bool atLeastOneRemoved = false;
		auto it = instanceVectorToWriteTo.begin();

		while (it != instanceVectorToWriteTo.end()) {
			if (it->enchantType == enchantTypeToRemove) {
				it = instanceVectorToWriteTo.erase(it);
				atLeastOneRemoved = true;
			}
			else ++it;
		}

		if (atLeastOneRemoved) {
			out.saveEnchantsToUserData(&resultEnchants);
		}
		return atLeastOneRemoved;
	}
	return false;
}

void EnchantUtils::removeAllEnchants(ItemStack &out) {

	auto resultEnchants = out.constructEnchantsFromUserData();

	const int32_t ENCHANTS_ARRAY_SIZE = 3;
	for (int32_t i = 0; i < ENCHANTS_ARRAY_SIZE; i++) {
		resultEnchants.enchants[i].clear();
	}

	out.saveEnchantsToUserData(&resultEnchants);
}