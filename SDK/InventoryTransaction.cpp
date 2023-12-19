#include "InventoryTransaction.h"
#include "../Utils/Utils.h"
void InventoryTransactionManager::addAction(InventoryAction const & action, bool balanced) {
	using InventoryTransactionManager__addAction_t = void(__cdecl*)(InventoryTransactionManager*, InventoryAction const&,bool);
	static auto InventoryTransactionManager__addAction = reinterpret_cast<InventoryTransactionManager__addAction_t>(FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? ? ? ? ? 4C 8B F2 48 8B F9"));
	if (InventoryTransactionManager__addAction )
		InventoryTransactionManager__addAction(this, action, balanced);
}

void InventoryTransaction::addAction(InventoryAction const & action) {
	using InventoryTransaction__addAction_t = void(__cdecl*)(InventoryTransaction*, InventoryAction const&);
	static auto InventoryTransaction__addAction = reinterpret_cast<InventoryTransaction__addAction_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B F2"));
	if (InventoryTransaction__addAction)
		InventoryTransaction__addAction(this, action);
}