#pragma once

#include "../Utils/Utils.h"
#include <cstdint>

class ReadOnlyBinaryStream {
	uintptr_t **vtable;
public:
	size_t readPointer;
	bool hasOverflowed;
	std::string ownedBuffer;
	std::string* buffer;

	ReadOnlyBinaryStream() = default;
	ReadOnlyBinaryStream(std::string &&rhs) : readPointer(0), hasOverflowed(false), ownedBuffer(std::move(rhs)) {
		this->buffer = &this->ownedBuffer;
	}
	ReadOnlyBinaryStream(std::string const &buffer, bool copyBuffer);
};
static_assert(sizeof(ReadOnlyBinaryStream) == 0x40);

class BinaryStream : public ReadOnlyBinaryStream {
public:
	std::string ownedBufferWritable;
	std::string* bufferWritable;

	BinaryStream();

	inline void reset() {
		this->bufferWritable->clear();
		this->readPointer = 0;
	};
	inline void write(void const *data, size_t size) {
		this->bufferWritable->append(reinterpret_cast<const char *>(data), size);
	}
	template <typename T>
	inline void dwrite(T const &val) { write(&val, sizeof(val)); }

	inline void writeFloat(float f) { dwrite(f); }
	inline void writeDouble(double d) { dwrite(d); }
	inline void writeString(const char *data) {
		size_t strSize = strlen(data);
		this->writeUnsignedVarInt((uint32_t)strSize);
		if (strSize > 0) {
			this->write(data, strSize);
		}
	}
	inline void writeString(const std::string &str) {
		this->writeString(str.c_str());
	}
	inline void writeVarInt(int i) { writeUnsignedVarInt(i > 0 ? 2 * i : ~(2 * i)); }
	inline void writeVarInt64(long l) { writeUnsignedVarInt(l > 0 ? 2 * l : ~(2 * l)); }
	inline void writeSignedInt(int i) { dwrite(i); }
	inline void writeFixedFloat(float f, double d) { writeVarInt((int)(f * d)); }
	inline void writeNormalizedFloat(float f) { writeFixedFloat(f, 2147483647.); }
	inline void writeSignedInt64(long long ll) { dwrite(ll); }
	inline void writeSignedShort(short s) { dwrite(s); }
	inline void writeUnsignedInt(unsigned int ui) { dwrite(ui); }
	inline void writeUnsignedChar(unsigned char uc) { dwrite(uc); }
	inline void writeUnsignedInt64(unsigned long long ull) { dwrite(ull); }
	inline void writeUnsignedShort(unsigned short us) { dwrite(us); }
	// void writeSignedBigEndianInt(int); // TODO Sig
	inline void writeBool(bool b) { writeByte(b); }
	inline void writeByte(unsigned char uc) { dwrite(uc); }
	inline void writeUnsignedVarInt(uint32_t uvalue) {
		do {
			uint8_t nextByte = (uvalue & 0x7F);
			uvalue >>= 7;
			if (uvalue) {
				nextByte |= 0x80;
			}
			this->writeByte(nextByte);
		} while (uvalue > 0);
	}
	inline void writeUnsignedVarInt64(uint64_t uvalue) {
		do {
			uint8_t nextByte = (uvalue & 0x7F);
			uvalue >>= 7;
			if (uvalue) {
				nextByte |= 0x80;
			}
			this->writeByte(nextByte);
		} while (uvalue > 0);
	}
	// These aren't in the game obviously, they're just useful adaptations from PMMP
	inline void writeByteRotation(unsigned char rotation) {
		writeByte(static_cast<unsigned char>(rotation / (360.f / 256.f)));
	}
	inline void writeBlockPos(const Vec3i &pos) {
		writeVarInt(pos.x);
		writeUnsignedVarInt(pos.y);
		writeVarInt(pos.z);
	}
	inline void writeVec3(const Vec3 &vec3) {
		writeFloat(vec3.x);
		writeFloat(vec3.y);
		writeFloat(vec3.z);
	}

	void prependPacketHeaderMetadata(PacketID id);
};
static_assert(sizeof(BinaryStream) == 0x68);