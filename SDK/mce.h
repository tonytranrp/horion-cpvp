#pragma once

#include <cstdint>

namespace mce {
	class UUID {
	public:
		uint64_t mostSig, leastSig;

		UUID() : mostSig(0), leastSig(0) {}
		UUID(uint64_t mostSig, uint64_t leastSig) : mostSig(mostSig), leastSig(leastSig) {}
		UUID(const UUID& rhs) : mostSig(rhs.mostSig), leastSig(rhs.leastSig) {}
		UUID(char* byteStringBuffer, bool swapBytes = false) {
			memcpy(this, byteStringBuffer, sizeof(mce::UUID));
			if (swapBytes) {
				this->mostSig = _byteswap_uint64(this->mostSig);  // todo: figure out how to do this byte swap bloat with uuidv4 lib
				this->leastSig = _byteswap_uint64(this->leastSig);
			}
		}

		inline bool operator==(const mce::UUID& rhs) const {
			return (this->mostSig == rhs.mostSig) && (this->leastSig == rhs.leastSig);
		}
		inline bool operator!=(const mce::UUID& rhs) const {
			return !(*this == rhs);
		}
	};
}  // namespace mce


// xref: mce::UUID::getHash
namespace std {
template <>
struct hash<mce::UUID> {
	 size_t operator()(const mce::UUID &k) const {
		return (522133279 * k.leastSig) ^ k.mostSig;
	}
};
}  // namespace std

static_assert(sizeof(mce::UUID) == 0x10);