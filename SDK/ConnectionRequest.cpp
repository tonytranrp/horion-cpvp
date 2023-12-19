#include "ConnectionRequest.h"

std::string WebToken::_DERToBinary(std::string* derSignature, int32_t inputLength) {
	using tokenSigToBinary_t = std::string*(__cdecl*)(std::string*, std::string*, int32_t);
	static auto _DERToBinaryFunc = reinterpret_cast<tokenSigToBinary_t>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 45 8B F8 48 8B F2"));

	std::string result;

	return *_DERToBinaryFunc(&result, derSignature, inputLength);
}

std::string KeyManager::getEncodedPublicKey() {
	return Utils::base64url_encode(this->publicKey);
}

std::string PrivateKeyManager::getEncodedPrivateKey() {
	return Utils::base64url_encode(this->privateKey);
}
