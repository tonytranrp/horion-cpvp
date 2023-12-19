#pragma once
#include "../Utils/Utils.h"

enum class PackManifestFormat;
enum class PackScope;
enum class ManifestOrigin;
enum class FormatVersion;
class BaseGameVersion;

enum class PackOrigin : int32_t {
	Unknown = 0x0,
	RealmsUnknown = 0x1,
	Package = 0x2,
	Treatment = 0x3,
	Dev = 0x4,
	World = 0x5,
	User = 0x6,
	TempCache = 0x7,
	PremiumCache = 0x8,
	PremiumTempCache = 0x9,
};

enum class PackType : char {
	Invalid = 0x0,
	Addon = 0x1,
	Cached = 0x2,
	CopyProtected = 0x3,
	Behavior = 0x4,
	PersonaPiece = 0x5,
	Resources = 0x6,
	Skins = 0x7,
	WorldTemplate = 0x8,
	Count = 0x9,
};

enum class PackCategory : int32_t {
	Unknown = 0x0,
	RealmsUnknown = 0x1,
	Standard = 0x2,
	Premium = 0x3,
	Custom = 0x4,
	Subpack = 0x5,
};

class PackAccessStrategy {
public:
	CLASS_FIELD(isTrusted, 0x1C, bool);
};

class DirectoryPackAccessStrategy : public PackAccessStrategy {
};

class ZipPackAccessStrategy : public PackAccessStrategy {
};

class PackManifest {
public:
	CLASS_FIELD(packSize, 0x76, uint64_t);
	CLASS_FIELD(name, 0x248, std::string);
	CLASS_FIELD(description, 0x268, std::string);
	CLASS_FIELD(packSettings, 0x328, MinecraftJson::Value*);
	CLASS_FIELD(type, 0x3A0, PackType);
	CLASS_FIELD(category, 0x3A4, PackCategory);
	CLASS_FIELD(origin, 0x3A8, PackOrigin);
	CLASS_FIELD(manifestOrigin, 0x3A9, ManifestOrigin);
	CLASS_FIELD(hidden, 0x3AB, bool);
	CLASS_FIELD(lastModifyDate, 0x3B8, std::string);
	CLASS_FIELD(invalidUUID, 0x3D8, bool);
	CLASS_FIELD(hasPlugins, 0x3D9, bool);
	CLASS_FIELD(hasClientScript, 0x3DA, bool);
	CLASS_FIELD(hasEduMetadata, 0x3DB, bool);
	CLASS_FIELD(platformLocked, 0x3DC, bool);
	CLASS_FIELD(capabilitiesTrusted, 0x3DE, bool);
	CLASS_FIELD(optionLockedState, 0x3DF, bool);
	CLASS_FIELD(scope, 0x3E0, PackScope);
	CLASS_FIELD(manifestFormat, 0x41C, PackManifestFormat);
	CLASS_FIELD(originalFormatVersion, 0x41D, FormatVersion);
};

class Pack {
public:
	CLASS_FIELD(manifest, 0x18, PackManifest*);
	CLASS_FIELD(accessStrategy, 0x20, PackAccessStrategy*);
};