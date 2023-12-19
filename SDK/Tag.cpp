#include "Tag.h"

#include "../Memory/GameData.h"
#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

std::unique_ptr<Tag> Tag::newTag(Type type) {
	using func_t = std::unique_ptr<Tag>(*)(Tag::Type type);
	func_t func = (func_t)FindSignature("40 53 48 83 EC 30 48 8B D9 48 89 4C 24 20 0F");
#ifdef HORION_DEBUG
	if (!func)
		throw std::out_of_range("Tag::newTag signature is dead.");
#endif
	return func(type);
}

void Tag::applyVtable(Tag::Type type, void* tag) {
	int typeint = *(char*)&type;
#ifdef HORION_DEBUG
	if (typeint > 0xB || typeint < 0)
		throw std::out_of_range("Tag type " + std::to_string(typeint) + " is not a valid type.");
#endif

	void* vtbl = vtableCache[typeint];
	if (!vtbl) {
		std::unique_ptr<Tag> suspiciousTag = Tag::newTag(type);
#ifdef HORION_DEBUG
		if (typeint > 0xB || typeint < 0)
			throw std::out_of_range("Tag type " + std::to_string(typeint) + " could not be created.");
#endif
		vtbl = *(void**)suspiciousTag.get();
		vtableCache[typeint] = vtbl;
	}

	*(void**)tag = vtbl;
}

void CompoundTag::sCopyVariant(Tag* v, CompoundTag::CompoundTagVariant& variant) {
	switch (v->getType()) {
	case Type::End:
		variant = *(EndTag*)v;
		break;
	case Type::Byte:
		variant = *(ByteTag*)v;
		break;
	case Type::Short:
		variant = *(ShortTag*)v;
		break;
	case Type::Int:
		variant = *(IntTag*)v;
		break;
	case Type::Int64:
		variant = *(Int64Tag*)v;
		break;
	case Type::Float:
		variant = *(FloatTag*)v;
		break;
	case Type::Double:
		variant = *(DoubleTag*)v;
		break;
	case Type::ByteArray:
		variant = *(ByteArrayTag*)v;
		break;
	case Type::String:
		variant = *(StringTag*)v;
		break;
	case Type::List:
		variant = *(ListTag*)v;
		break;
	case Type::Compound:
		variant = *(CompoundTag*)v;
		break;
	case Type::IntArray:
		variant = *(IntArrayTag*)v;
		break;
	default:
		break;
	}
}
void CompoundTag::sMoveVariant(Tag* v, CompoundTag::CompoundTagVariant& variant) {
	switch (v->getType()) {
	case Type::End:
		variant = std::move(*(EndTag*)v);
		break;
	case Type::Byte:
		variant = std::move(*(ByteTag*)v);
		break;
	case Type::Short:
		variant = std::move(*(ShortTag*)v);
		break;
	case Type::Int:
		variant = std::move(*(IntTag*)v);
		break;
	case Type::Int64:
		variant = std::move(*(Int64Tag*)v);
		break;
	case Type::Float:
		variant = std::move(*(FloatTag*)v);
		break;
	case Type::Double:
		variant = std::move(*(DoubleTag*)v);
		break;
	case Type::ByteArray:
		variant = std::move(*(ByteArrayTag*)v);
		break;
	case Type::String:
		variant = std::move(*(StringTag*)v);
		break;
	case Type::List:
		variant = std::move(*(ListTag*)v);
		break;
	case Type::Compound:
		variant = std::move(*(CompoundTag*)v);
		break;
	case Type::IntArray:
		variant = std::move(*(IntArrayTag*)v);
		break;
	default:
		break;
	}
}
CompoundTag::MapType CompoundTag::sCloneList(const CompoundTag::MapType& l) {
	MapType r;
	for (const auto& [k, v] : l) {
		sCopyVariant((Tag*)&v, r[k]);
	}
	return r;
}
