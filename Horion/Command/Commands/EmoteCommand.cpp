#include "EmoteCommand.h"
#include "../../../PCH.h"

EmoteCommand::EmoteCommand() :
	IMCCommand("emote", "Plays any emote!", "\n1) <string: emoteName>\n2) <int: emoteId>\n3) <list>") {

	// https://github.com/JustTalDevelops/bedrock-emotes
	using uuidv4 = UUIDv4::UUID;
	EMOTE_TO_UUID_MAP[{"Abduction", 0}] = uuidv4::fromStrFactory(xorstr_("18891e6c-bb3d-47f6-bc15-265605d86525"));
	EMOTE_TO_UUID_MAP[{"ActingLikeADragon", 1}] = uuidv4::fromStrFactory(xorstr_("c2a47805-c792-4882-a56d-17c80b6c57a8"));
	EMOTE_TO_UUID_MAP[{"AhhChoo", 2}] = uuidv4::fromStrFactory(xorstr_("f9345ebb-4ba3-40e6-ad9b-6bfb10c92890"));
	EMOTE_TO_UUID_MAP[{"Airplane", 3}] = uuidv4::fromStrFactory(xorstr_("e7bb3d8c-811d-442a-bfc5-775556d05d7a"));
	EMOTE_TO_UUID_MAP[{"AngerManagement", 4}] = uuidv4::fromStrFactory(xorstr_("a0b1a9f2-0ea4-48d0-8641-ada998beee4d"));
	EMOTE_TO_UUID_MAP[{"BallerinaTwirl", 5}] = uuidv4::fromStrFactory(xorstr_("79452f7e-ffa0-470f-8283-f5063348471d"));
	EMOTE_TO_UUID_MAP[{"BigChuckles", 6}] = uuidv4::fromStrFactory(xorstr_("819f2f36-2a16-440c-8e46-94c6b003a2e0"));
	EMOTE_TO_UUID_MAP[{"Bored", 7}] = uuidv4::fromStrFactory(xorstr_("7a314ecf-f94c-42c0-945f-76903c923808"));
	EMOTE_TO_UUID_MAP[{"Bow", 8}] = uuidv4::fromStrFactory(xorstr_("ddfa6f0e-88ca-46de-b189-2bd5b18e96a0"));
	EMOTE_TO_UUID_MAP[{"Breakdance", 9}] = uuidv4::fromStrFactory(xorstr_("1dbaa006-0ec6-42c3-9440-a3bfa0c6fdbe"));
	EMOTE_TO_UUID_MAP[{"CallingADragon", 10}] = uuidv4::fromStrFactory(xorstr_("9f5d4732-0513-4a0a-8ea2-b6b8d7587e74"));
	EMOTE_TO_UUID_MAP[{"Cartwheel", 11}] = uuidv4::fromStrFactory(xorstr_("5cf9d5a3-6fa0-424e-8ae4-d1f877b836da"));
	EMOTE_TO_UUID_MAP[{"Chatting", 12}] = uuidv4::fromStrFactory(xorstr_("59d9e78c-f0bb-4f14-9e9b-7ab4f58ffbf5"));
	EMOTE_TO_UUID_MAP[{"CheerRoutine", 13}] = uuidv4::fromStrFactory(xorstr_("3d10a8c7-213c-4fbe-a208-a0f7990d5bbb"));
	EMOTE_TO_UUID_MAP[{"ChunLiSpinningKick", 14}] = uuidv4::fromStrFactory(xorstr_("af61c759-6252-431d-a7de-94d477cfb54c"));
	EMOTE_TO_UUID_MAP[{"ClickingHeels", 15}] = uuidv4::fromStrFactory(xorstr_("495d686a-4cb3-4f0b-beb5-bebdcb95eed9"));
	EMOTE_TO_UUID_MAP[{"CowpokeDancing", 16}] = uuidv4::fromStrFactory(xorstr_("f99ccd35-ebda-4122-b458-ff8c9f9a432f"));
	EMOTE_TO_UUID_MAP[{"DJing", 17}] = uuidv4::fromStrFactory(xorstr_("beb74219-e90c-46aa-8a4b-a1c175f6cab5"));
	EMOTE_TO_UUID_MAP[{"DancingLikeRapunzel", 18}] = uuidv4::fromStrFactory(xorstr_("f15ceb56-6651-41c0-8bd5-087951204d81"));
	EMOTE_TO_UUID_MAP[{"DancingLikeToothless", 19}] = uuidv4::fromStrFactory(xorstr_("a12252fa-4ec8-42e0-a7d0-d44fbc90d753"));
	EMOTE_TO_UUID_MAP[{"DiamondsToYou", 20}] = uuidv4::fromStrFactory(xorstr_("86b34976-8f41-475b-a386-385080dc6e83"));
	EMOTE_TO_UUID_MAP[{"Disappointed", 21}] = uuidv4::fromStrFactory(xorstr_("a98ea25e-4e6a-477f-8fc2-9e8a18ab7004"));
	EMOTE_TO_UUID_MAP[{"DoingAerobics", 22}] = uuidv4::fromStrFactory(xorstr_("2647f538-6fc6-4775-85ae-7f6f2a8065b2"));
	EMOTE_TO_UUID_MAP[{"DoingHakunaMatata", 23}] = uuidv4::fromStrFactory(xorstr_("71afa8ed-03b6-4808-b537-330daf9dbf50"));
	EMOTE_TO_UUID_MAP[{"DoingTheConga", 24}] = uuidv4::fromStrFactory(xorstr_("5e1ef7ed-efdf-44a9-8ace-6cca6275d80d"));
	EMOTE_TO_UUID_MAP[{"Enamored", 25}] = uuidv4::fromStrFactory(xorstr_("c4d87a21-d4c4-498b-8bc1-5dd78c895e9f"));
	EMOTE_TO_UUID_MAP[{"ExcitedlyDancing", 26}] = uuidv4::fromStrFactory(xorstr_("d9d1192d-1262-470d-bc0c-5ba46bedab72"));
	EMOTE_TO_UUID_MAP[{"Exhausted", 27}] = uuidv4::fromStrFactory(xorstr_("2391b018-3b8a-4bb0-9596-8edfc502d302"));
	EMOTE_TO_UUID_MAP[{"Facepalm", 28}] = uuidv4::fromStrFactory(xorstr_("402efb2d-6607-47f2-b8e5-bc422bcd8304"));
	EMOTE_TO_UUID_MAP[{"Faceplant", 29}] = uuidv4::fromStrFactory(xorstr_("6d9f24c0-6246-4c92-8169-4648d1981cbb"));
	EMOTE_TO_UUID_MAP[{"FakedDeath", 30}] = uuidv4::fromStrFactory(xorstr_("efc2f0f5-af00-4d9e-a4b1-78f18d63be79"));
	EMOTE_TO_UUID_MAP[{"FeelingSick", 31}] = uuidv4::fromStrFactory(xorstr_("bb6f1764-2b0b-4a3a-adfd-3334627cdee4"));
	EMOTE_TO_UUID_MAP[{"Flight", 32}] = uuidv4::fromStrFactory(xorstr_("b310ea06-0485-4ad1-8424-df271ca6ec37"));
	EMOTE_TO_UUID_MAP[{"Fluttering", 33}] = uuidv4::fromStrFactory(xorstr_("9c939344-f9d6-4a3a-95a5-f64dd3d94e45"));
	EMOTE_TO_UUID_MAP[{"FlyingLikePeterPan", 34}] = uuidv4::fromStrFactory(xorstr_("949c4f24-6aa6-4f2a-bad5-1b3de7e12749"));
	EMOTE_TO_UUID_MAP[{"FollowMe", 35}] = uuidv4::fromStrFactory(xorstr_("17428c4c-3813-4ea1-b3a9-d6a32f83afca"));  // undocumented
	EMOTE_TO_UUID_MAP[{"FootStomp", 36}] = uuidv4::fromStrFactory(xorstr_("13334afa-bd66-4285-b3d9-d974046db479"));
	EMOTE_TO_UUID_MAP[{"FoundInspiration", 37}] = uuidv4::fromStrFactory(xorstr_("b690c36b-bd02-45df-a60b-00c0dbbf265f"));
	EMOTE_TO_UUID_MAP[{"GhastDance", 38}] = uuidv4::fromStrFactory(xorstr_("5a5b2c0c-a924-4e13-a99b-4c12e3f02e1e"));
	EMOTE_TO_UUID_MAP[{"Giddy", 39}] = uuidv4::fromStrFactory(xorstr_("738497ce-539f-4e06-9a03-dc528506a468"));
	EMOTE_TO_UUID_MAP[{"GivingR2D2AMessage", 40}] = uuidv4::fromStrFactory(xorstr_("e1090020-cbe0-4b64-9c41-a3b9619da029"));
	EMOTE_TO_UUID_MAP[{"GoingHero", 41}] = uuidv4::fromStrFactory(xorstr_("f14d652f-18ed-42dc-831f-7f6a2eab1246"));
	EMOTE_TO_UUID_MAP[{"GolfClap", 42}] = uuidv4::fromStrFactory(xorstr_("434489fd-ed42-4814-961a-df14161d67e0"));
	EMOTE_TO_UUID_MAP[{"GotChills", 43}] = uuidv4::fromStrFactory(xorstr_("af6ad96c-5754-4cf9-a463-49a8190e6b0e"));
	EMOTE_TO_UUID_MAP[{"Grooving", 44}] = uuidv4::fromStrFactory(xorstr_("d863b9cc-9f8c-498b-a8a3-7ebd542cb08e"));
	EMOTE_TO_UUID_MAP[{"Hadoken", 45}] = uuidv4::fromStrFactory(xorstr_("6453480a-54ac-435d-899d-4e6f7fe57591"));
	EMOTE_TO_UUID_MAP[{"HandStand", 46}] = uuidv4::fromStrFactory(xorstr_("5dd129f9-cfc3-4fc1-b464-c66a03061545"));
	EMOTE_TO_UUID_MAP[{"HardLanding", 47}] = uuidv4::fromStrFactory(xorstr_("c2b9dd03-7258-4813-b6f5-37ef9b4a3326"));
	EMOTE_TO_UUID_MAP[{"HittingTheOmnitrix", 48}] = uuidv4::fromStrFactory(xorstr_("1e30e8f4-2991-4aac-a58a-a6bf90a080c5"));
	EMOTE_TO_UUID_MAP[{"Hooray", 49}] = uuidv4::fromStrFactory(xorstr_("c4b5b251-24d3-43eb-9c05-46be246aeefb"));
	EMOTE_TO_UUID_MAP[{"Hopping", 50}] = uuidv4::fromStrFactory(xorstr_("fb9b884a-b217-46a2-85fd-d3ab0f2f33e6"));
	EMOTE_TO_UUID_MAP[{"HuggingThemselves", 51}] = uuidv4::fromStrFactory(xorstr_("54e2209a-41ec-4502-8500-dd51e569ab69"));
	EMOTE_TO_UUID_MAP[{"Hungry", 52}] = uuidv4::fromStrFactory(xorstr_("d24f27c0-6bc8-4193-842c-8e33fdae5e8e"));
	EMOTE_TO_UUID_MAP[{"IntercomAndAttack", 53}] = uuidv4::fromStrFactory(xorstr_("87c039b7-7bdc-45d1-8684-e940a131ec0a"));
	EMOTE_TO_UUID_MAP[{"JumpingJacks", 54}] = uuidv4::fromStrFactory(xorstr_("b13966b3-1f7c-4f07-8ab8-9d85170203f5"));
	EMOTE_TO_UUID_MAP[{"Kneeling", 55}] = uuidv4::fromStrFactory(xorstr_("24444aea-cb6e-451f-90fc-b74e57cc7c5d"));
	EMOTE_TO_UUID_MAP[{"LaughingOutLoud", 56}] = uuidv4::fromStrFactory(xorstr_("310b41de-1c7a-4098-928d-addcd905bf65"));
	EMOTE_TO_UUID_MAP[{"MadFalling", 57}] = uuidv4::fromStrFactory(xorstr_("63cb3c45-b3b3-4c02-a4f3-fb4b9853dbe9"));
	EMOTE_TO_UUID_MAP[{"MeditatingLikeLuke", 58}] = uuidv4::fromStrFactory(xorstr_("85957448-e7bb-4bb4-9182-510b4428e52c"));
	EMOTE_TO_UUID_MAP[{"MinionCheering", 59}] = uuidv4::fromStrFactory(xorstr_("6174a111-19ac-41d1-a483-0e0b37f9d049"));
	EMOTE_TO_UUID_MAP[{"MinionPanicking", 60}] = uuidv4::fromStrFactory(xorstr_("79e93b59-38f0-4796-8033-f99d7aa9334e"));
	EMOTE_TO_UUID_MAP[{"MinionTaunting", 61}] = uuidv4::fromStrFactory(xorstr_("a51f8e7c-a4dd-4ce1-9fc9-2c3cad66cede"));
	EMOTE_TO_UUID_MAP[{"Offering", 62}] = uuidv4::fromStrFactory(xorstr_("21e0054a-5bf4-468d-bfc4-fc4b49bd44ac"));
	EMOTE_TO_UUID_MAP[{"OverHere", 63}] = uuidv4::fromStrFactory(xorstr_("71721c51-b7d1-46b1-b7ea-eb4c4126c3db"));
	EMOTE_TO_UUID_MAP[{"OverThere", 64}] = uuidv4::fromStrFactory(xorstr_("ce5c0300-7f03-455d-aaf1-352e4927b54d"));
	EMOTE_TO_UUID_MAP[{"Peekaboo", 65}] = uuidv4::fromStrFactory(xorstr_("2ecc0fd1-312b-4664-9bfa-a06429838706"));
	EMOTE_TO_UUID_MAP[{"PlayingDead", 66}] = uuidv4::fromStrFactory(xorstr_("9d4019d0-db3f-4859-a579-f6c37532d2a6"));
	EMOTE_TO_UUID_MAP[{"PlayingZombie", 67}] = uuidv4::fromStrFactory(xorstr_("5d644007-3cdf-4246-b4ca-cfd7a4318a1c"));
	EMOTE_TO_UUID_MAP[{"PosingLikeAHero", 68}] = uuidv4::fromStrFactory(xorstr_("7102c0f7-6113-44f5-b37c-4e94841b0e81"));
	EMOTE_TO_UUID_MAP[{"Pouting", 69}] = uuidv4::fromStrFactory(xorstr_("34259925-44dd-46f0-98e5-80ccebc086d0"));
	EMOTE_TO_UUID_MAP[{"ROFL", 70}] = uuidv4::fromStrFactory(xorstr_("6f82688e-e549-408c-946d-f8e99b91808d"));
	EMOTE_TO_UUID_MAP[{"Rebooting", 71}] = uuidv4::fromStrFactory(xorstr_("a602063f-1ded-4959-b978-b5ae7f353536"));
	EMOTE_TO_UUID_MAP[{"RyuGuarding", 72}] = uuidv4::fromStrFactory(xorstr_("0c141653-1166-448b-87a2-38d0679bb549"));
	EMOTE_TO_UUID_MAP[{"SadSigh", 73}] = uuidv4::fromStrFactory(xorstr_("98a68056-e025-4c0f-a959-d6e330ccb5f5"));
	EMOTE_TO_UUID_MAP[{"SalsaDancing", 74}] = uuidv4::fromStrFactory(xorstr_("6bcf44bd-ff8a-48a5-9254-3983a0b0f702"));
	EMOTE_TO_UUID_MAP[{"Saluting", 75}] = uuidv4::fromStrFactory(xorstr_("1ea5f771-827d-408d-ae8e-1af00d7aa416"));
	EMOTE_TO_UUID_MAP[{"Shrug", 76}] = uuidv4::fromStrFactory(xorstr_("4ff73ed2-3c2f-4d74-9055-5fa24e59dc7a"));
	EMOTE_TO_UUID_MAP[{"ShyGiggling", 77}] = uuidv4::fromStrFactory(xorstr_("f1e18201-729d-472d-9e4f-5cdd7f6bba0c"));
	EMOTE_TO_UUID_MAP[{"SidDance", 78}] = uuidv4::fromStrFactory(xorstr_("946f04ea-8ff0-416e-a66d-8b0d0751e8bf"));
	EMOTE_TO_UUID_MAP[{"SimpleClap", 79}] = uuidv4::fromStrFactory(xorstr_("9a469a61-c83b-4ba9-b507-bdbe64430582"));
	EMOTE_TO_UUID_MAP[{"Sliming", 80}] = uuidv4::fromStrFactory(xorstr_("70831936-d97d-48fe-a7e7-d8894e7c5507"));
	EMOTE_TO_UUID_MAP[{"SonicFingerWagging", 81}] = uuidv4::fromStrFactory(xorstr_("e8a91ad4-6f7a-479e-9556-e1a7c058351b"));
	EMOTE_TO_UUID_MAP[{"SonicRunning", 82}] = uuidv4::fromStrFactory(xorstr_("b8e54924-ec3f-409c-8914-bca61fbe9242"));
	EMOTE_TO_UUID_MAP[{"SonicVictorySpinning", 83}] = uuidv4::fromStrFactory(xorstr_("cd8c3bc6-f455-43d2-836e-62c1a19474c7"));
	EMOTE_TO_UUID_MAP[{"Surrendering", 84}] = uuidv4::fromStrFactory(xorstr_("daeaaa6f-db91-4461-8617-400c5d1b8646"));
	EMOTE_TO_UUID_MAP[{"SwingingEnergySword", 85}] = uuidv4::fromStrFactory(xorstr_("d7680c67-8e54-482a-903d-622ddb43485f"));
	EMOTE_TO_UUID_MAP[{"SwordFlourish", 86}] = uuidv4::fromStrFactory(xorstr_("a57277d5-0693-4c8a-9b5c-45c33fdf7c26"));
	EMOTE_TO_UUID_MAP[{"TheElytra", 87}] = uuidv4::fromStrFactory(xorstr_("7393aa53-9145-4e66-b23b-ec86def6c6f2"));
	EMOTE_TO_UUID_MAP[{"TheHammer", 88}] = uuidv4::fromStrFactory(xorstr_("7cec98d8-55cc-44fe-b0ae-2672b0b2bd37"));
	EMOTE_TO_UUID_MAP[{"TheMasterChief", 89}] = uuidv4::fromStrFactory(xorstr_("88dc7deb-21bb-48c3-8b5a-00757a06aef9"));
	EMOTE_TO_UUID_MAP[{"ThePickaxe", 90}] = uuidv4::fromStrFactory(xorstr_("d7519b5a-45ec-4d27-997c-89d402c6b57f"));
	EMOTE_TO_UUID_MAP[{"TheSplits", 91}] = uuidv4::fromStrFactory(xorstr_("51871829-e9ef-40aa-939c-b8952295d50e"));
	EMOTE_TO_UUID_MAP[{"TheWoodpunch", 92}] = uuidv4::fromStrFactory(xorstr_("42fde774-37d4-4422-b374-89ff13a6535a"));
	EMOTE_TO_UUID_MAP[{"Thinking", 93}] = uuidv4::fromStrFactory(xorstr_("20bcb500-af82-4c2f-9239-e78191c61375"));
	EMOTE_TO_UUID_MAP[{"ToInfinityAndBeyond", 94}] = uuidv4::fromStrFactory(xorstr_("79e8cb30-c1b4-4e41-8aa8-96492c6f390c"));
	EMOTE_TO_UUID_MAP[{"UnderwaterDancing", 95}] = uuidv4::fromStrFactory(xorstr_("05af18ca-920f-4232-83cb-133b2d913dd6"));
	EMOTE_TO_UUID_MAP[{"UsingJediMindTrick", 96}] = uuidv4::fromStrFactory(xorstr_("4b9b9f17-3722-4d38-a6a9-9ba0e8cf5044"));
	EMOTE_TO_UUID_MAP[{"Victorious", 97}] = uuidv4::fromStrFactory(xorstr_("77053f59-c79b-4d5c-8dc5-539ed28002e9"));
	EMOTE_TO_UUID_MAP[{"VictoryCheer", 98}] = uuidv4::fromStrFactory(xorstr_("d0c60245-538e-4ea2-bdd4-33477db5aa89"));
	EMOTE_TO_UUID_MAP[{"WarmUpDrills", 99}] = uuidv4::fromStrFactory(xorstr_("6b9c7731-7422-4333-950f-cf3d147ffa47"));
	EMOTE_TO_UUID_MAP[{"Wave", 100}] = uuidv4::fromStrFactory(xorstr_("4c8ae710-df2e-47cd-814d-cc7bf21a3d67"));
	EMOTE_TO_UUID_MAP[{"WavingLikeC3PO", 101}] = uuidv4::fromStrFactory(xorstr_("c2d6091d-9f91-4a9e-badd-ef8481353cb0"));
	EMOTE_TO_UUID_MAP[{"Yoga", 102}] = uuidv4::fromStrFactory(xorstr_("3f1bdf46-80b0-4a64-b631-4ac2f2491165"));
}

std::optional<int32_t> EmoteCommand::tryGetIntegerFromString(const char* str) const {
	int32_t result{};
	try {
		result = std::stoi(str);
	} catch (...) {
		return {};
	}
	return std::make_optional<int32_t>(result);
}

void EmoteCommand::playEmote(const UUIDv4::UUID& emoteUuid) const {
	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	static char uuidBuf[16];
	emoteUuid.bytes(uuidBuf);

	EmoteListPacket emoteListPkt(lp->getRuntimeId(), {mce::UUID(uuidBuf, true)});
	Game.getLoopbackPacketSender()->sendToServer(&emoteListPkt);
	// lp->playEmote(emoteUuid.str());  // sends EmotePacket and plays it clientside, but only if it's unlocked :/

	auto SharedEmotePkt = std::make_shared<EmotePacket>(EmotePacket(lp->getRuntimeId(), emoteUuid.str()));
	std::thread([SharedEmotePkt]() -> void {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		Game.getLoopbackPacketSender()->sendToServer(SharedEmotePkt.get());
	}).detach();
}

void EmoteCommand::listAllFriendlyEmoteNames() const {
	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	std::string buf{};
	for (const auto& [nameIdPair, emoteUuid] : EMOTE_TO_UUID_MAP) {
		buf += nameIdPair.first + " (\u00c2\u00a7b" + std::to_string(nameIdPair.second) + "\u00c2\u00a7r)\n";
	}
	buf.pop_back();

	Game.getGuiData()->displayClientMessage(&buf);
}

bool EmoteCommand::execute(std::vector<std::string>* args) {
	size_t argSize = args->size();
	this->assertTrue(argSize >= 2);

	const std::string& action = Utils::toLowercase((*args)[1]);
	
	if (action == "list") {
		this->listAllFriendlyEmoteNames();
		return true;
	}

	// id lookup
	auto maybeIdArg = this->tryGetIntegerFromString(action.c_str());
	if (maybeIdArg.has_value()) {
		int32_t idArg = maybeIdArg.value();
		for (const auto& [nameIdPair, emoteUuid] : EMOTE_TO_UUID_MAP) {
			if (nameIdPair.second == idArg) {
				this->playEmote(emoteUuid);
				this->clientMessageF("%sSuccessfully sent emote to server!", GREEN);
				return true;
			}
		}
		this->clientMessageF("%sInvalid emote ID! See '.emote list' for a list of emote IDs", RED);
		return false;
	}

	// name lookup
	for (const auto& [nameIdPair, emoteUuid] : EMOTE_TO_UUID_MAP) {
		if (Utils::caseInsensitiveEquals(nameIdPair.first, action)) {
			this->playEmote(emoteUuid);
			this->clientMessageF("%sSuccessfully sent emote to server!", GREEN);
			return true;
		}
	}
	this->clientMessageF("%sInvalid emote name! See '.emote list' for a list of emote names", RED);
	return false;
}

void EmoteCommand::sendFullEmoteListToServer() {
	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	EmoteListPacket pkt{};
	pkt.runtimeId = lp->getRuntimeId();
	static char uuidBuf[16];
	auto& pktSender = *Game.getLoopbackPacketSender();

	for (const auto& [nameIdPair, emoteUuid] : EMOTE_TO_UUID_MAP) {
		emoteUuid.bytes(uuidBuf);
		pkt.emoteUuids.emplace_back(mce::UUID(uuidBuf, true));
		if (pkt.emoteUuids.size() == MAX_EMOTE_LIST_UUIDS) {
			pktSender.sendToServer(&pkt);
			pkt.emoteUuids.clear();
		}
	}
	pktSender.sendToServer(&pkt); // send remainder
}