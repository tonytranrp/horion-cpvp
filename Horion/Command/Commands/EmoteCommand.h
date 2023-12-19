#pragma once

#include "ICommand.h"
#include "../../../Utils/uuid_v4.h"

class EmoteCommand : public IMCCommand {
	std::optional<int32_t> tryGetIntegerFromString(const char* str) const;
	void playEmote(const UUIDv4::UUID& emoteUuid) const;
	void listAllFriendlyEmoteNames() const;
public:
	EmoteCommand();
	virtual bool execute(std::vector<std::string>* args) override;

	// UNUSED
	// call when SetLocalPlayerAsInitializedPacket is sent, & in cmd constructor
	static void sendFullEmoteListToServer();

	using EmoteMap = std::map<std::pair<std::string, int32_t>, UUIDv4::UUID>;
	static inline EmoteMap EMOTE_TO_UUID_MAP{};
	constexpr static inline int32_t MAX_EMOTE_LIST_UUIDS = 6;
};