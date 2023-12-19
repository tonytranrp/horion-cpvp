#pragma once
#include <memory>
#include <sstream>
#include <variant>

#include "MojangsonToken.h"


class PrintStream {
public:
	virtual ~PrintStream() = default;
	virtual void print(const std::string& text) {  }
};

class EndTag;
class ByteTag;
class ShortTag;
class IntTag;
class Int64Tag;
class FloatTag;
class DoubleTag;
class ByteArrayTag;
class StringTag;
class ListTag;
class CompoundTag;
class IntArrayTag;


class Tag {
public:
	enum class Type : char {
		End = 0x0,
		Byte = 0x1,
		Short = 0x2,
		Int = 0x3,
		Int64 = 0x4,
		Float = 0x5,
		Double = 0x6,
		ByteArray = 0x7,
		String = 0x8,
		List = 0x9,
		Compound = 0xA,
		IntArray = 0xB
	};
	template <typename T>
	struct TagTypeIdentifierValueResolver {
		constexpr static Tag::Type value = Tag::Type::End;
	};

	struct RequiredTagTypeMissmatchException : public std::exception {
		std::string errorMessage;
		RequiredTagTypeMissmatchException(Tag::Type obtained, Tag::Type expected) {
			errorMessage = "Tag " + std::to_string(*(int*)&obtained) + " is not type " + std::to_string(*(int*)&expected);
		}
		virtual ~RequiredTagTypeMissmatchException() = default;
		virtual const char* what() const override { return errorMessage.c_str(); }
	};

	//! this should never ever be called !
	virtual ~Tag() = default;                                                  // 0
	virtual void deleteChildren(void) {}                                       // 1
	virtual void write(class IDataOutput* output) {  }                                        // 2
	virtual void load(class IDataInput* input) { }                                          // 3
	virtual std::string toString(void) const { return std::string(); }         // 4
	virtual Type getType(void) const { return Tag::Type::End; }               // 5
	virtual bool equals(Tag const&) const { return false; }                    // 6
	virtual void print(PrintStream& PrintStream) const {}                      // 7
	virtual void print(const std::string&, PrintStream& PrintStream) const {}  // 8
	virtual std::unique_ptr<Tag> copy() const { return nullptr; };             //
	virtual size_t hash(void) const { return 69; }                              // 10

	static std::unique_ptr<Tag> newTag(Type type);



	template <typename T>
	T& as() { return *(T*)this; }

	template <typename T>
	const T& as() const { return *(T*)this; }

	template <typename T>
	bool is() const { return TagTypeIdentifierValueResolver<T>::value == this->getType(); }

	template <typename RequiredType>
	const RequiredType& require() const {
		if (!this->is<RequiredType>())
			throw Tag::RequiredTagTypeMissmatchException(this->getType(), TagTypeIdentifierValueResolver<RequiredType>::value);
		return this->as<RequiredType>();
	}

	template <typename RequiredType>
	RequiredType& require() {
		if (!this->is<RequiredType>())
			throw Tag::RequiredTagTypeMissmatchException(this->getType(), TagTypeIdentifierValueResolver<RequiredType>::value);
		return this->as<RequiredType>();
	}

	bool isEnd() const { return this->is<EndTag>(); }
	bool isByte() const { return this->is<ByteTag>(); }
	bool isShort() const { return this->is<ShortTag>(); }
	bool isInt() const { return this->is<IntTag>(); }
	bool isInt64() const { return this->is<Int64Tag>(); }
	bool isLong() const { return this->is<Int64Tag>(); }
	bool isFloat() const { return this->is<FloatTag>(); }
	bool isDouble() const { return this->is<DoubleTag>(); }
	bool isByteArray() const { return this->is<ByteArrayTag>(); }
	bool isString() const { return this->is<StringTag>(); }
	bool isList() const { return this->is<ListTag>(); }
	bool isCompound() const { return this->is<CompoundTag>(); }
	bool isIntArray() const { return this->is<IntArrayTag>(); }

	EndTag& asEnd() { return this->as<EndTag>(); }
	ByteTag& asByte() { return this->as<ByteTag>(); }
	ShortTag& asShort() { return this->as<ShortTag>(); }
	IntTag& asInt() { return this->as<IntTag>(); }
	Int64Tag& asInt64() { return this->as<Int64Tag>(); }
	Int64Tag& asLong() { return this->as<Int64Tag>(); }
	FloatTag& asFloat() { return this->as<FloatTag>(); }
	DoubleTag& asDouble() { return this->as<DoubleTag>(); }
	ByteArrayTag& asByteArray() { return this->as<ByteArrayTag>(); }
	StringTag& asString() { return this->as<StringTag>(); }
	ListTag& asList() { return this->as<ListTag>(); }
	CompoundTag& asCompound() { return this->as<CompoundTag>(); }
	IntArrayTag& asIntArray() { return this->as<IntArrayTag>(); }
	const EndTag& asEnd() const { return this->as<EndTag>(); }
	const ByteTag& asByte() const { return this->as<ByteTag>(); }
	const ShortTag& asShort() const { return this->as<ShortTag>(); }
	const IntTag& asInt() const { return this->as<IntTag>(); }
	const Int64Tag& asInt64() const { return this->as<Int64Tag>(); }
	const Int64Tag& asLong() const { return this->as<Int64Tag>(); }
	const FloatTag& asFloat() const { return this->as<FloatTag>(); }
	const DoubleTag& asDouble() const { return this->as<DoubleTag>(); }
	const ByteArrayTag& asByteArray() const { return this->as<ByteArrayTag>(); }
	const StringTag& asString() const { return this->as<StringTag>(); }
	const ListTag& asList() const { return this->as<ListTag>(); }
	const CompoundTag& asCompound() const { return this->as<CompoundTag>(); }
	const IntArrayTag& asIntArray() const { return this->as<IntArrayTag>(); }

	EndTag& requireEnd() { return this->require<EndTag>(); }
	ByteTag& requireByte() { return this->require<ByteTag>(); }
	ShortTag& requireShort() { return this->require<ShortTag>(); }
	IntTag& requireInt() { return this->require<IntTag>(); }
	Int64Tag& requireInt64() { return this->require<Int64Tag>(); }
	Int64Tag& requireLong() { return this->require<Int64Tag>(); }
	FloatTag& requireFloat() { return this->require<FloatTag>(); }
	DoubleTag& requireDouble() { return this->require<DoubleTag>(); }
	ByteArrayTag& requireByteArray() { return this->require<ByteArrayTag>(); }
	StringTag& requireString() { return this->require<StringTag>(); }
	ListTag& requireList() { return this->require<ListTag>(); }
	CompoundTag& requireCompound() { return this->require<CompoundTag>(); }
	IntArrayTag& requireIntArray() { return this->require<IntArrayTag>(); }
	const EndTag& requireEnd() const { return this->require<EndTag>(); }
	const ByteTag& requireByte() const { return this->require<ByteTag>(); }
	const ShortTag& requireShort() const { return this->require<ShortTag>(); }
	const IntTag& requireInt() const { return this->require<IntTag>(); }
	const Int64Tag& requireInt64() const { return this->require<Int64Tag>(); }
	const Int64Tag& requireLong() const { return this->require<Int64Tag>(); }
	const FloatTag& requireFloat() const { return this->require<FloatTag>(); }
	const DoubleTag& requireDouble() const { return this->require<DoubleTag>(); }
	const ByteArrayTag& requireByteArray() const { return this->require<ByteArrayTag>(); }
	const StringTag& requireString() const { return this->require<StringTag>(); }
	const ListTag& requireList() const { return this->require<ListTag>(); }
	const CompoundTag& requireCompound() const { return this->require<CompoundTag>(); }
	const IntArrayTag& requireIntArray() const { return this->require<IntArrayTag>(); }


	protected:
	static inline void* vtableCache[16] = {nullptr};
	static void applyVtable(Tag::Type type, void* tag);
	inline void setVtable(Tag::Type type) { applyVtable(type, this); }
};


class EndTag : public Tag {
public:
	EndTag() {
		setVtable(Tag::Type::End);
	}
};

class ByteTag : public Tag {
public:
	uint8_t value;

	inline ByteTag() {
		setVtable(Tag::Type::Byte);
		value = 0;
	}
	inline ByteTag(const ByteTag& other) {
		setVtable(Tag::Type::Byte);
		this->value = other.value;
	}
	inline ByteTag(ByteTag&& other) {
		setVtable(Tag::Type::Byte);
		this->value = std::move(other.value);
	}
	explicit inline ByteTag(uint8_t value)  {
		setVtable(Tag::Type::Byte);
		this->value = value;
	}
	bool operator==(const ByteTag& other) const { return this->value == other.value; }
	bool operator!=(const ByteTag& other) const { return this->value != other.value; }
	ByteTag& operator=(uint8_t other) {
		setVtable(Tag::Type::Byte);
		this->value = other;
		return *this;
	}
	ByteTag& operator=(ByteTag other) {
		setVtable(Tag::Type::Byte);
		this->value = other.value;
		return *this;
	}

	operator uint8_t() const { return value; }
};
class ShortTag : public Tag {
public:
	int16_t value;

	inline ShortTag() {
		setVtable(Tag::Type::Short);
		value = 0;
	}
	inline ShortTag(const ShortTag& other) {
		setVtable(Tag::Type::Short);
		this->value = other.value;
	}
	inline ShortTag(ShortTag&& other) {
		setVtable(Tag::Type::Short);
		this->value = std::move(other.value);
	}
	explicit inline ShortTag(int16_t value) {
		setVtable(Tag::Type::Short);
		this->value = value; 
	}
	bool operator==(const ShortTag& other) const {  return this->value == other.value; }
	bool operator!=(const ShortTag& other) const {  return this->value != other.value; }
	ShortTag& operator=(int16_t other) {
		setVtable(Tag::Type::Short);
		this->value = other;
		return *this;
	}
	ShortTag& operator=(ShortTag other) {
		setVtable(Tag::Type::Short);
		this->value = other.value;
		return *this;
	}

	operator int16_t() const { return value; }
};
class IntTag : public Tag {
public:
	int32_t value;

	inline IntTag() {
		setVtable(Tag::Type::Int);
		value = 0;
	}
	inline IntTag(const IntTag& other) {
		setVtable(Tag::Type::Int);
		this->value = other.value;
	}
	inline IntTag(IntTag&& other) {
		setVtable(Tag::Type::Int);
		this->value = std::move(other.value);
	}
	explicit inline IntTag(int32_t value) {
		setVtable(Tag::Type::Int);
		this->value = value;
	}
	bool operator==(const IntTag& other) const { return this->value == other.value; }
	bool operator!=(const IntTag& other) const { return this->value != other.value; }
	IntTag& operator=(int32_t other) {
		setVtable(Tag::Type::Int);
		this->value = other;
		return *this;
	}
	IntTag& operator=(IntTag other) {
		setVtable(Tag::Type::Int);
		this->value = other.value;
		return *this;
	}

	operator int32_t() const { return value; }
};
class Int64Tag : public Tag {
public:
	int64_t value;

	inline Int64Tag() {
		setVtable(Tag::Type::Int64);
		value = 0;
	}
	inline Int64Tag(const Int64Tag& other) {
		setVtable(Tag::Type::Int64);
		this->value = other.value;
	}
	inline Int64Tag(Int64Tag&& other) {
		setVtable(Tag::Type::Int64);
		this->value = std::move(other.value);
	}
	explicit inline Int64Tag(int64_t value) {
		setVtable(Tag::Type::Int64);
		this->value = value; 
	}
	bool operator==(const Int64Tag& other) const { return this->value == other.value; }
	bool operator!=(const Int64Tag& other) const { return this->value != other.value; }
	Int64Tag& operator=(int64_t other) {
		setVtable(Tag::Type::Int64);
		this->value = other;
		return *this;
	}
	Int64Tag& operator=(Int64Tag other) {
		setVtable(Tag::Type::Int64);
		this->value = other.value;
		return *this;
	}

	operator int64_t() const { return value; }
};
class FloatTag : public Tag {
public:
	float value;

	inline FloatTag() {
		setVtable(Tag::Type::Float);
		value = 0;
	}
	inline FloatTag(const FloatTag& other) {
		setVtable(Tag::Type::Float);
		this->value = other.value;
	}
	inline FloatTag(FloatTag&& other) {
		setVtable(Tag::Type::Float);
		this->value = std::move(other.value);
	}
	explicit inline FloatTag(float value) {
		setVtable(Tag::Type::Float);
		this->value = value; 
	}
	bool operator==(const FloatTag& other) const { return this->value == other.value; }
	bool operator!=(const FloatTag& other) const { return this->value != other.value; }
	FloatTag& operator=(float other) {
		setVtable(Tag::Type::Float);
		this->value = other;
		return *this;
	}
	FloatTag& operator=(FloatTag other) {
		setVtable(Tag::Type::Float);
		this->value = other.value;
		return *this;
	}

	operator float() const { return value; }
};
class DoubleTag : public Tag {
public:
	double value;

	inline DoubleTag() {
		setVtable(Tag::Type::Double);
		value = 0;
	}
	inline DoubleTag(const DoubleTag& other) {
		setVtable(Tag::Type::Double);
		this->value = other.value;
	}
	inline DoubleTag(DoubleTag&& other) {
		setVtable(Tag::Type::Double);
		this->value = std::move(other.value);
	}
	explicit inline DoubleTag(double value) {
		setVtable(Tag::Type::Double);
		this->value = value;
	}
	bool operator==(const DoubleTag& other) const { return this->value == other.value; }
	bool operator!=(const DoubleTag& other) const { return this->value != other.value; }
	DoubleTag& operator=(double other) {
		setVtable(Tag::Type::Double);
		this->value = other;
		return *this;
	}
	DoubleTag& operator=(DoubleTag other) {
		setVtable(Tag::Type::Double);
		this->value = other.value;
		return *this;
	}

	operator double() const { return value; }
};


class TagMemoryChunk {
	size_t elementCount = 0;
	size_t elementSize = 0;
	std::unique_ptr<unsigned char[]> data;
public:
	TagMemoryChunk() = default;
	TagMemoryChunk(const TagMemoryChunk& other) { *this = other; }
	TagMemoryChunk(TagMemoryChunk&& other) { *this = std::move(other); }
	TagMemoryChunk(const std::vector<int>& other) { *this = other; }
	TagMemoryChunk(const std::vector<uint8_t>& other) { *this = other; }

	void operator=(const TagMemoryChunk& other) {
		this->elementCount = other.elementCount;
		this->elementSize = other.elementSize;
		data = std::make_unique<unsigned char[]>(elementCount * elementSize);
		memcpy(data.get(), other.data.get(), elementCount * elementSize);
	}
	void operator=(TagMemoryChunk&& other) {
		this->elementCount = other.elementCount;
		this->elementSize = other.elementSize;
		data = std::move(other.data);
		memset(&other, 0, sizeof(*this));
	}
	void operator=(const std::vector<int>& other) {
		this->elementCount = other.size();
		this->elementSize = 4;
		data = std::make_unique<unsigned char[]>(elementCount * elementSize);
		memcpy(data.get(), other.data(), elementCount * elementSize);
	}
	void operator=(const std::vector<uint8_t>& other) {
		this->elementCount = other.size();
		this->elementSize = 1;
		data = std::make_unique<unsigned char[]>(elementCount * elementSize);
		memcpy(data.get(), other.data(), elementCount * elementSize);
	}


	bool empty() const { return !data || elementCount == 0 || elementSize == 0; }
	size_t size() const { return elementCount; }

	int at(size_t index) const {
	#ifdef HORION_DEBUG
		if (index > elementCount)
			throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
		if (this->empty())
			throw std::out_of_range("Trying to access empty TagMemoryChunk");
	#endif
		unsigned char* element = data.get() + index * elementSize;
		return elementSize == 1 ? *element : *(int*)element;
	}
	int operator[](size_t index) const { return at(index); }
	void set(size_t index, int value) {
#ifdef HORION_DEBUG
		if (index > elementCount)
			throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
		if (this->empty())
			throw std::out_of_range("Trying to access empty TagMemoryChunk");
#endif
		unsigned char* element = data.get() + index * elementSize;
		if (elementSize == 1)
			*element = (uint8_t)value;
		else
			*(int*)element = value;
	}

	std::vector<int>toIntVector() const {
#ifdef HORION_DEBUG
		if (this->elementSize != 4)
			throw std::bad_function_call(); //"This is not an int array"
#else
		if (this->elementSize != 4)
			return std::vector<int>();
#endif
		if (!this->data || this->elementCount <= 0)
			return std::vector<int>();
		std::vector<int> result(this->elementCount+2);
		for (int v : *this)
			result.push_back(v);
		return result;
	}
	std::vector<uint8_t> toByteVector() const {
#ifdef HORION_DEBUG
		if (this->elementSize != 1)
			throw std::bad_function_call(); //"This is not an byte array"
#else
		if (this->elementSize != 1)
			return std::vector<uint8_t>();
#endif
		if (!this->data || this->elementCount <= 0)
			return std::vector<uint8_t>();
		std::vector<uint8_t> result(this->elementCount + 2);
		for (int v : *this)
			result.push_back((uint8_t)v);
		return result;
	}

	class Iterator {
		size_t index;
		const TagMemoryChunk* parent;
		Iterator(size_t index, const TagMemoryChunk* parent)
			: index(index), parent(parent) {}
		public:

		inline int operator*() const { return parent->at(index); }
		inline operator int() const { this->operator*(); }

		inline void operator+=(size_t val) { index += val; }
		inline Iterator operator+(size_t val) const { return Iterator(index + val, parent); }
		inline void operator++() { this->operator+=(1); }
		inline void operator-=(size_t val) { index -= val; }
		inline Iterator operator-(size_t val) const { return Iterator(index - val, parent); }
		inline void operator--() { this->operator-=(1); }
		inline bool operator==(const Iterator& other) const { return this->parent == other.parent && this->index == other.index; }
		inline bool operator!=(const Iterator& other) const { return this->parent != other.parent || this->index != other.index; }
		inline bool operator>(const Iterator& other) const { return this->index > other.index; }
		inline bool operator<(const Iterator& other) const { return this->index < other.index; }

		friend class TagMemoryChunk;
	};
	Iterator begin() const { return Iterator(0, this); }
	Iterator end() const { return Iterator(elementCount, this); }
};


class ByteArrayTag : public Tag {
public:
	TagMemoryChunk value;

	inline ByteArrayTag() {
		setVtable(Tag::Type::ByteArray);
	}
	inline ByteArrayTag(const ByteArrayTag& other) {
		setVtable(Tag::Type::ByteArray);
		this->value = other.value;
	}
	inline ByteArrayTag(ByteArrayTag&& other) {
		setVtable(Tag::Type::ByteArray);
		this->value = std::move(other.value);
	}
	explicit inline ByteArrayTag(const std::vector<uint8_t>& value) {
		setVtable(Tag::Type::ByteArray);
		this->value = value;
	}
	bool operator==(const ByteArrayTag& other) const { return this->hash() == other.hash(); }
	bool operator!=(const ByteArrayTag& other) const { return this->hash() != other.hash(); }
	ByteArrayTag& operator=(std::vector<uint8_t> other) {
		setVtable(Tag::Type::ByteArray);
		this->value = other;
		return *this;
	}
	ByteArrayTag& operator=(ByteArrayTag other) {
		setVtable(Tag::Type::ByteArray);
		this->value = other.value;
		return *this;
	}

	
	std::vector<uint8_t> toVector() const { return this->value.toByteVector(); }
	inline TagMemoryChunk::Iterator begin() const { return value.begin(); }
	inline TagMemoryChunk::Iterator end() const { return value.end(); }
	inline bool empty() const { return value.empty(); }
	inline size_t size() const { return value.size(); }
	inline uint8_t at(size_t index) const { return (uint8_t)value.at(index); }
	inline uint8_t operator[](size_t index) const { return (uint8_t)value.at(index); }
	inline void set(size_t index, uint8_t val) { value.set(index, (int)val); }
};
class IntArrayTag : public Tag {
public:
	TagMemoryChunk value;

	inline IntArrayTag() {
		setVtable(Tag::Type::IntArray);
	}
	inline IntArrayTag(const IntArrayTag& other) {
		setVtable(Tag::Type::IntArray);
		this->value = other.value;
	}
	inline IntArrayTag(IntArrayTag&& other) {
		setVtable(Tag::Type::IntArray);
		this->value = std::move(other.value);
	}
	explicit inline IntArrayTag(const std::vector<int>& value) {
		setVtable(Tag::Type::IntArray);
		this->value = value;
	}
	bool operator==(const IntArrayTag& other) const { return this->hash() == other.hash(); }
	bool operator!=(const IntArrayTag& other) const { return this->hash() != other.hash(); }
	IntArrayTag& operator=(std::vector<int> other) {
		setVtable(Tag::Type::IntArray);
		this->value = other;
		return *this;
	}
	IntArrayTag& operator=(IntArrayTag other) {
		setVtable(Tag::Type::IntArray);
		this->value = other.value;
		return *this;
	}

	std::vector<int> toVector() const { return this->value.toIntVector(); }
	inline TagMemoryChunk::Iterator begin() const { return value.begin(); }
	inline TagMemoryChunk::Iterator end() const { return value.end(); }
	inline bool empty() const { return value.empty(); }
	inline size_t size() const { return value.size(); }
	inline int at(size_t index) const { return value.at(index); }
	inline int operator[](size_t index) const { return value.at(index); }
	inline void set(size_t index, int val) { value.set(index, val); }
};


class StringTag : public Tag {
public:
	std::string value;
	using ListType = std::string;
	using ValueType = std::string::value_type;

	StringTag() {
		setVtable(Tag::Type::String);
	}
	StringTag(const StringTag& other) {
		setVtable(Tag::Type::String);
		*this = other;
	}
	StringTag(const std::string& other) {
		setVtable(Tag::Type::String);
		*this = other;
	}
	StringTag(StringTag&& other) {
		setVtable(Tag::Type::String);
		*this = std::move(other);
	}
	StringTag(const std::string&& other) {
		setVtable(Tag::Type::String);
		*this = std::move(other);
	}

	StringTag& operator=(const std::string& other) {
		setVtable(Tag::Type::String);
		this->value = other;
		return *this;
	}
	StringTag& operator=(const StringTag& other) {
		setVtable(Tag::Type::String);
		this->value = other.value;
		return *this;
	}
	StringTag& operator=(std::string&& other) {
		setVtable(Tag::Type::String);
		this->value = std::move(other);
		return *this;
	}
	StringTag& operator=(StringTag&& other) {
		setVtable(Tag::Type::String);
		this->value = std::move(other.value);
		return *this;
	}
	StringTag& operator+=(const std::string& other) {
		this->value += other;
		return *this;
	}
	StringTag& operator+=(const char* other) {
		this->value += other;
		return *this;
	}
	std::string operator+(const std::string& other) { return this->value + other; }
	std::string operator+(const char* other) { return this->value + other; }
	bool operator==(const std::string& other) const { return this->value == other; }
	bool operator!=(const std::string& other) const { return this->value != other; }
	bool operator>=(const std::string& other) const { return this->value >= other; }
	bool operator<=(const std::string& other) const { return this->value <= other; }
	bool operator>(const std::string& other) const { return this->value > other; }
	bool operator<(const std::string& other) const { return this->value < other; }
	bool operator==(const StringTag& other) const { return this->value == other.value; }
	bool operator!=(const StringTag& other) const { return this->value != other.value; }
	bool operator>=(const StringTag& other) const { return this->value >= other.value; }
	bool operator<=(const StringTag& other) const { return this->value <= other.value; }
	bool operator>(const StringTag& other) const { return this->value > other.value; }
	bool operator<(const StringTag& other) const { return this->value < other.value; }

	inline ListType::iterator begin() { return value.begin(); }
	inline ListType::iterator end() { return value.end(); }
	inline ListType::const_iterator begin() const { return value.begin(); }
	inline ListType::const_iterator end() const { return value.end(); }
	inline ValueType& front() { return value.front(); }
	inline const ValueType& front() const { return value.front(); }
	inline ValueType& back() { return value.back(); }
	inline const ValueType& back() const { return value.back(); }
	inline void push_back(ValueType Value) { value.push_back(Value); }
	inline void emplace_back(ValueType&& Value) { value.push_back(Value); }
	inline void pop() { value.pop_back(); }
	inline void clear() { value.clear(); }
	inline bool empty(size_t key) const { return value.empty(); }
	inline ValueType& at(size_t key) { return value.at(key); }
	inline const ValueType& at(size_t key) const { return value.at(key); }
	inline size_t find(const std::string& str) const { return value.find(str); }
	inline std::string substr(size_t offset) const { return value.substr(offset); }
	inline std::string substr(size_t offset, size_t size) const { return value.substr(offset, size); }
	inline ValueType& operator[](size_t key) { return value.operator[](key); }
	inline size_t size() const { return value.size(); }
	inline void insert(size_t index, ValueType value) { this->value.insert(this->value.begin() + index, value); }
	inline void insert(size_t index, const char* value) { this->value.insert(index, value); }
	inline void insert(size_t index, const std::string& value) { this->value.insert(index, value.c_str()); }
	inline void reserve(size_t size) { this->value.reserve(size); }
	inline void insert(ListType::iterator index, ValueType value) { this->value.insert(index, value); }
	inline void insert(ListType::const_iterator index, ValueType value) { this->value.insert(index, value); }
	inline void erase(ListType::iterator it) { value.erase(it); }
	inline void erase(ListType::const_iterator it) { value.erase(it); }
	inline void erase(ListType::iterator begin, ListType::iterator end) { value.erase(begin, end); }
	inline void erase(ListType::const_iterator begin, ListType::const_iterator end) { value.erase(begin, end); }

	operator const std::string&() const { return value; }
	operator std::string&() { return value; }
};



class ListTag : public Tag {
public:
	using KeyType = size_t;
	using ValueType = std::unique_ptr<Tag>;
	using ListType = std::vector<ValueType>;
	ListType value;
	Tag::Type type = Tag::Type::End;

	ListTag() {
		setVtable(Tag::Type::List);
	}
	ListTag(const ListTag& other) {
		setVtable(Tag::Type::List);
		*this = other;
	}
	ListTag(const ListType& other) {
		setVtable(Tag::Type::List);
		*this = other;
	}
	ListTag(ListTag&& other) {
		setVtable(Tag::Type::List);
		*this = std::move(other);
	}

	ListTag& operator=(const ListType& other) {
		setVtable(Tag::Type::List);
		this->value = sCloneList(other);
		if (this->value.size() > 0)
			this->type = this->value.front()->getType();
		return *this;
	}
	ListTag& operator=(const ListTag& other) {
		setVtable(Tag::Type::List);
		this->value = sCloneList(other.value);
		this->type = other.type;
		return *this;
	}
	ListTag& operator=(ListType&& other) {
		setVtable(Tag::Type::List);
		this->value = std::move(other);
		if (this->value.size() > 0)
			this->type = this->value.front()->getType();
		return *this;
	}
	ListTag& operator=(ListTag&& other) {
		setVtable(Tag::Type::List);
		this->value = std::move(other.value);
		this->type = other.type;
		return *this;
	}

	template <typename T>
	class Iterator {
		std::vector<std::unique_ptr<T>>* owner;

	public:
		Iterator(std::vector<std::unique_ptr<Tag>>* owner)
			: owner((std::vector<std::unique_ptr<T>>*)owner) {}

		auto begin() { return owner->begin(); }
		auto end() { return owner->end(); }
	};
	template <typename T>
	class ConstIterator {
		const std::vector<std::unique_ptr<T>>* owner;

	public:
		ConstIterator(std::vector<std::unique_ptr<Tag>>* owner)
			: owner((std::vector<std::unique_ptr<T>>*)owner) {}

		auto begin() const { return owner->cbegin(); }
		auto end() const { return owner->cend(); }
	};

	inline ListType::iterator begin() { return value.begin(); }
	inline ListType::iterator end() { return value.end(); }
	inline ListType::const_iterator begin() const { return value.begin(); }
	inline ListType::const_iterator end() const { return value.end(); }
	inline ValueType& front() { return value.front(); }
	inline const ValueType& front() const { return value.front(); }
	inline ValueType& back() { return value.back(); }
	inline const ValueType& back() const { return value.back(); }
	inline void push_back(ValueType&& Value) {
		type = Value->getType();
		value.push_back(std::move(Value));
	}
	inline void emplace_back(ValueType&& Value) {
		type = Value->getType();
		value.emplace_back(std::move(Value));
	}
	template <typename T, typename... Args>
	inline void emplace_back(const Args&... Value) {
		type = Value->getType();
		value.emplace_back(std::move(std::unique_ptr<Tag>((Tag*)(new T(Value...)))));
	}
	inline void pop() { value.pop_back(); }
	inline void clear() { value.clear(); }
	inline bool empty(const KeyType& key) const { return value.empty(); }
	inline ValueType& at(const KeyType& key) { return value.at(key); }
	inline const ValueType& at(const KeyType& key) const { return value.at(key); }
	inline ValueType& operator[](const KeyType& key) { return value.operator[](key); }
	inline size_t size() const { return value.size(); }
	inline void insert(size_t index, ValueType&& Value) {
		type = Value->getType();
		this->value.insert(this->value.begin() + index, std::move(Value));
	}
	inline void insert(ListType::iterator index, ValueType&& Value) {
		type = Value->getType();
		this->value.insert(index, std::move(Value));
	}
	inline void insert(ListType::const_iterator index, ValueType&& Value) {
		type = Value->getType();
		this->value.insert(index, std::move(Value));
	}
	inline void reserve(size_t size) { this->value.reserve(size); }
	inline void erase(ListType::iterator it) { value.erase(it); }
	inline void erase(ListType::const_iterator it) { value.erase(it); }
	inline void erase(ListType::iterator begin, ListType::iterator end) { value.erase(begin, end); }
	inline void erase(ListType::const_iterator begin, ListType::const_iterator end) { value.erase(begin, end); }

	template <typename T>
	Iterator<T> iterate() { return Iterator<T>(&this->value); }
	template <typename T>
	ConstIterator<T> iterate() const { return ConstIterator<T>(&this->value); }

	operator const ListType&() const { return value; }
	operator ListType&() { return value; }

private:
	inline static ListType sCloneList(const ListType& l) {
		ListType r;
		for (auto& v : l)
			r.emplace_back(v->copy());
		return r;
	}
};


class CompoundTag : public Tag {
public:
	using KeyType = std::string;
	using CompoundTagVariant = std::variant<EndTag,ByteTag,ShortTag,IntTag,Int64Tag,FloatTag,DoubleTag,ByteArrayTag,StringTag,ListTag,CompoundTag,IntArrayTag>;
	using MapType = std::map<KeyType, CompoundTagVariant>;
	MapType value;

	CompoundTag() {
		setVtable(Tag::Type::Compound);
	}
	CompoundTag(const CompoundTag& other) {
		setVtable(Tag::Type::Compound);
		*this = other;
	}
	CompoundTag(const MapType& other) {
		setVtable(Tag::Type::Compound);
		*this = other;
	}
	CompoundTag(CompoundTag&& other) {
		setVtable(Tag::Type::Compound);
		*this = std::move(other);
	}

	CompoundTag& operator=(const MapType& other) {
		setVtable(Tag::Type::Compound);
		this->value = sCloneList(other);
		return *this;
	}
	CompoundTag& operator=(const CompoundTag& other) {
		setVtable(Tag::Type::Compound);
		this->value = sCloneList(other.value);
		return *this;
	}
	CompoundTag& operator=(MapType&& other) {
		setVtable(Tag::Type::Compound);
		this->value = std::move(other);
		return *this;
	}
	CompoundTag& operator=(CompoundTag&& other) {
		setVtable(Tag::Type::Compound);
		this->value = std::move(other.value);
		return *this;
	}

	inline MapType::iterator begin() { return value.begin(); }
	inline MapType::iterator end() { return value.end(); }
	inline MapType::const_iterator begin() const { return value.begin(); }
	inline MapType::const_iterator end() const { return value.end(); }
	inline MapType::iterator find(const KeyType& key) { return value.find(key); }
	inline MapType::const_iterator find(const KeyType& key) const { return value.find(key); }
	inline bool contains(const KeyType& key) const { return value.find(key) != value.end(); }
	inline void clear() { value.clear(); }
	inline bool empty(const KeyType& key) const { return value.empty(); }
	inline Tag* at(const KeyType& key) { return (Tag*)&value.at(key); }
	inline const Tag* at(const KeyType& key) const { return (const Tag*)&value.at(key); }
	inline CompoundTag& operator[](const KeyType& key) { return getOrSet<CompoundTag>(key); }
	inline size_t size() const { return value.size(); }
	inline void erase(MapType::iterator it) { value.erase(it); }
	inline void erase(MapType::const_iterator it) { value.erase(it); }
	inline void erase(MapType::iterator begin, MapType::iterator end) { value.erase(begin, end); }
	inline void erase(MapType::const_iterator begin, MapType::const_iterator end) { value.erase(begin, end); }
	inline void erase(const KeyType& key) {
		auto I = value.find(key);
		if (I != value.end()) value.erase(I);
	}


	template <typename T, typename... Args>
	inline T& getOrSet(const std::string& key, const Args&... args) {
		auto& variant = this->value.operator[](key); 
		if (variant.valueless_by_exception())
			variant = T(args...);
		return *(T*)&variant;
	}
	template <typename T, typename... Args>
	inline T& set(const std::string& key, const Args&... args) {
		auto& variant = this->value.operator[](key);
		variant = T(args...);
		return *(T*)&variant;
	}

	template <typename T>
	inline T* get(const std::string& key) {
		auto I = find(key);
		if (I == end() || !((Tag*)&I->second)->is<T>()) return nullptr;
		return (T*)&I->second;
	}
	template <typename T>
	inline const T* get(const std::string& key) const {
		auto I = find(key);
		if (I == end() || !((Tag*)&I->second)->is<T>()) return nullptr;
		return (T*)&I->second;
	}

	template <typename T>
	inline T& as(const std::string& key) { return *(T*)&this->at(key)->as<T>(); }
	template <typename T>
	inline const T& as(const std::string& key) const { return this->at(key)->as<T>(); }

	template <typename T>
	inline T& require(const std::string& key) { return this->at(key)->require<T>(); }
	template <typename T>
	inline const T& require(const std::string& key) const { return this->at(key)->require<T>(); }

	// mojang functions
	inline void putByte(const std::string& Key, uint8_t Value) { set<ByteTag>(Key, Value); }
	inline void putShort(const std::string& Key, int16_t Value) { set<ShortTag>(Key, Value); }
	inline void puInt(const std::string& Key, int32_t Value) { set<IntTag>(Key, Value); }
	inline void putInt64(const std::string& Key, int64_t Value) { set<Int64Tag>(Key, Value); }
	inline void putFloat(const std::string& Key, float Value) { set<FloatTag>(Key, Value); }
	inline void putDouble(const std::string& Key, double Value) { set<DoubleTag>(Key, Value); }
	inline void putString(const std::string& Key, const std::string& Value) { set<StringTag>(Key, Value); }
	inline void putList(const std::string& Key, const ListTag& Value) { set<ListTag>(Key, Value); }
	inline void putList(const std::string& Key, const ListTag::ListType& Value) { set<ListTag>(Key, Value); }
	inline void putCompound(const std::string& Key, const CompoundTag& Value) { set<CompoundTag>(Key, Value); }
	inline void putCompound(const std::string& Key, const CompoundTag::MapType& Value) { set<CompoundTag>(Key, Value); }
	inline void putByteArray(const std::string& Key, const ByteArrayTag& Value) { set<ByteArrayTag>(Key, Value); }
	inline void putByteArray(const std::string& Key, const std::vector<uint8_t>& Value) { set<ByteArrayTag>(Key, Value); }
	inline void putIntArray(const std::string& Key, const IntArrayTag& Value) { set<IntArrayTag>(Key, Value); }
	inline void putIntArray(const std::string& Key, const std::vector<int>& Value) { set<IntArrayTag>(Key, Value); }

	inline ByteTag* getByte(const std::string& Key) { get<ByteTag>(Key); }
	inline ShortTag* getShort(const std::string& Key) { get<ShortTag>(Key); }
	inline IntTag* getInt(const std::string& Key) { get<IntTag>(Key); }
	inline Int64Tag* getInt64(const std::string& Key) { get<Int64Tag>(Key); }
	inline FloatTag* getFloat(const std::string& Key) { get<FloatTag>(Key); }
	inline DoubleTag* getDouble(const std::string& Key) { get<DoubleTag>(Key); }
	inline StringTag* getString(const std::string& Key) { get<StringTag>(Key); }
	inline ListTag* getList(const std::string& Key) { get<ListTag>(Key); }
	inline CompoundTag* getCompound(const std::string& Key) { get<CompoundTag>(Key); }
	inline ByteArrayTag* getByteArray(const std::string& Key) { get<ByteArrayTag>(Key); }
	inline IntArrayTag* getIntArray(const std::string& Key) { get<IntArrayTag>(Key); }
	inline const ByteTag* getByte(const std::string& Key) const { get<ByteTag>(Key); }
	inline const ShortTag* getShort(const std::string& Key) const { get<ShortTag>(Key); }
	inline const IntTag* getInt(const std::string& Key) const { get<IntTag>(Key); }
	inline const Int64Tag* getInt64(const std::string& Key) const { get<Int64Tag>(Key); }
	inline const FloatTag* getFloat(const std::string& Key) const { get<FloatTag>(Key); }
	inline const DoubleTag* getDouble(const std::string& Key) const { get<DoubleTag>(Key); }
	inline const StringTag* getString(const std::string& Key) const { get<StringTag>(Key); }
	inline const ListTag* getList(const std::string& Key) const { get<ListTag>(Key); }
	inline const CompoundTag* getCompound(const std::string& Key) const { get<CompoundTag>(Key); }
	inline const ByteArrayTag* getByteArray(const std::string& Key) const { get<ByteArrayTag>(Key); }
	inline const IntArrayTag* getIntArray(const std::string& Key) const { get<IntArrayTag>(Key); }

	inline void putCopy(const std::string& key, Tag* tag) { sCopyVariant(tag, value[key]); }
	inline void putMove(const std::string& key, Tag* tag) { sMoveVariant(tag, value[key]); }

	operator const MapType&() const { return value; }
	operator MapType&() { return value; }

private:
	void sCopyVariant(Tag* tag, CompoundTagVariant& variant);
	void sMoveVariant(Tag* tag, CompoundTagVariant& variant);
	MapType sCloneList(const MapType& Map);
};



template <> struct Tag::TagTypeIdentifierValueResolver<EndTag> { constexpr inline static Tag::Type value = Tag::Type::End; };
template <> struct Tag::TagTypeIdentifierValueResolver<ByteTag> { constexpr inline static Tag::Type value = Tag::Type::Byte; };
template <> struct Tag::TagTypeIdentifierValueResolver<ShortTag> { constexpr inline static Tag::Type value = Tag::Type::Short; };
template <> struct Tag::TagTypeIdentifierValueResolver<IntTag> { constexpr inline static Tag::Type value = Tag::Type::Int; };
template <> struct Tag::TagTypeIdentifierValueResolver<Int64Tag> { constexpr inline static Tag::Type value = Tag::Type::Int64; };
template <> struct Tag::TagTypeIdentifierValueResolver<FloatTag> { constexpr inline static Tag::Type value = Tag::Type::Float; };
template <> struct Tag::TagTypeIdentifierValueResolver<DoubleTag> { constexpr inline static Tag::Type value = Tag::Type::Double; };
template <> struct Tag::TagTypeIdentifierValueResolver<ByteArrayTag> { constexpr inline static Tag::Type value = Tag::Type::ByteArray; };
template <> struct Tag::TagTypeIdentifierValueResolver<StringTag> { constexpr inline static Tag::Type value = Tag::Type::String; };
template <> struct Tag::TagTypeIdentifierValueResolver<ListTag> { constexpr inline static Tag::Type value = Tag::Type::List; };
template <> struct Tag::TagTypeIdentifierValueResolver<CompoundTag> { constexpr inline static Tag::Type value = Tag::Type::Compound; };
template <> struct Tag::TagTypeIdentifierValueResolver<IntArrayTag> { constexpr inline static Tag::Type value = Tag::Type::IntArray; };

