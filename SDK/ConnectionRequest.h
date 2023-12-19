#pragma once

#include "MinecraftJson.h"

enum class CryptoHashType : int32_t {
	MD5 = 0,
	SHA1 = 1,
	SHA256 = 2,
	SHA384 = 3,
	SHA512 = 4,
};

class MinecraftCryptoAsymmetricInterface {
public:
	uintptr_t** vtable;
	inline std::string* signData(std::string* result, std::string* privateKey, std::string* data, CryptoHashType hashType) {
		return Utils::CallVFunc<4, std::string*>(this, result, privateKey, data, hashType);
	}
};

class MinecraftCryptoAsymmetric : public MinecraftCryptoAsymmetricInterface {
public:
	int32_t unk;
	std::unique_ptr<MinecraftCryptoAsymmetricInterface> openSSLInterface;
};
static_assert(sizeof(MinecraftCryptoAsymmetric) == 0x18);

class KeyManager {
public:
	virtual ~KeyManager();
	virtual bool isValid();

	std::string publicKey; // not base64 encoded
	std::unique_ptr<MinecraftCryptoAsymmetric> signer;

	std::string getEncodedPublicKey();
};
static_assert(offsetof(KeyManager, publicKey) == 0x8);
static_assert(offsetof(KeyManager, signer) == 0x28);
static_assert(sizeof(KeyManager) == 0x30);

class PrivateKeyManager : public KeyManager {
public:
	std::string privateKey; // not base64 encoded

	std::string getEncodedPrivateKey();

	// returns an intermediary webtoken signature, but the result still needs to be passed into WebToken::_DERToBinary
	inline std::string sign(std::string& data, CryptoHashType hashType = CryptoHashType::SHA384) {
		std::string result;
		this->signer->signData(&result, &this->privateKey, &data, hashType);
		return result;
	}
};
static_assert(offsetof(PrivateKeyManager, privateKey) == 0x30);
static_assert(sizeof(PrivateKeyManager) == 0x50);

class WebToken {
public:
	std::string header;
	MinecraftJson::Value headerInfo;
	std::string payload;
	MinecraftJson::Value dataInfo;
	std::string signature;

	// the result of this gets base64 url encoded then assigned to this->signature
	// hooking WebToken::_DERToBinary always shows the outputLength as 96
	static std::string _DERToBinary(std::string* derSignature, int32_t outputLength = 0x60);
};
static_assert(sizeof(WebToken) == 0x80);

class UnverifiedCertificate {
public:
	WebToken rawToken;
	std::unique_ptr<UnverifiedCertificate> parentUnverifiedCert;
};
static_assert(sizeof(UnverifiedCertificate) == 0x88);

class Certificate {
public:
	UnverifiedCertificate unverifiedCert;
	std::unique_ptr<Certificate> parentCert;
	bool isValid;
};
static_assert(sizeof(Certificate) == 0x98);

class ConnectionRequest {
public:
	std::unique_ptr<UnverifiedCertificate> certData;
	std::unique_ptr<Certificate> cert;
	std::unique_ptr<WebToken> rawToken; // where the actual stuff is i think (skin data, jwt claims, etc)
	uint8_t clientSubId;
};
static_assert(sizeof(ConnectionRequest) == 0x20);