#include "ComplexInventoryTransaction.h"
#include "../Utils/Utils.h"

//void ComplexInventoryTransaction::read(ReadOnlyBinaryStream& bs) {
//	Utils::CallVFunc<1, void>(this, bs);
//}
//
//void ComplexInventoryTransaction::write(BinaryStream* bs) {
//	Utils::CallVFunc<2, void>(this, bs);
//}

ItemUseInventoryTransaction::ItemUseInventoryTransaction() {
	using ItemUseInventoryTransaction_t = void*(__cdecl*)(ItemUseInventoryTransaction*);
	static auto ItemUseInventoryTransaction = reinterpret_cast<ItemUseInventoryTransaction_t>(
		FindSignature("48 89 5C 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8D 59 ? ? ? ? 02"));

	ItemUseInventoryTransaction(this);
}

ItemUseOnActorInventoryTransaction::ItemUseOnActorInventoryTransaction() {
	using ItemUseOnActorInventoryTransaction_t = void*(__cdecl*)(ItemUseOnActorInventoryTransaction*);
	static auto ItemUseOnActorInventoryTransaction = reinterpret_cast<ItemUseOnActorInventoryTransaction_t>(
		FindSignature("48 89 5C 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8D 59 ? ? ? ? 03"));

	ItemUseOnActorInventoryTransaction(this);
}

ItemReleaseInventoryTransaction::ItemReleaseInventoryTransaction() {
	using ItemReleaseInventoryTransaction_t = void*(__cdecl*)(ItemReleaseInventoryTransaction*);
	static auto ItemReleaseInventoryTransaction = reinterpret_cast<ItemReleaseInventoryTransaction_t>(
		FindSignature("48 89 5C 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8D 59 ? ? ? ? 04"));

	ItemReleaseInventoryTransaction(this);
}