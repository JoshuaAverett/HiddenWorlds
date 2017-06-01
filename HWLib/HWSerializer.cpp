#include "HWSerializer.h"

bool HWSerializer::is_big_endian = false;
bool HWSerializer::initialized = false;

HWSerializer::HWSerializer()
{
	if(!initialized)
	{
		// Does anyone have a big endian machine now days?
		// It'd be faster to cut all the twiddling if we don't need it
		S32 testInt = 0x01020304;
		U8* testArray = reinterpret_cast<U8*>(&testInt);
		is_big_endian = testArray[0] == 0x01;
		initialized = true;
	}
	serialized_data = nullptr;
	expected_length = 0;
	read_pointer = 0;
	is_dirty = false;
}

HWSerializer::~HWSerializer()
{
	delete[] serialized_data;
}

void HWSerializer::Clear()
{
	segments.clear();
	if(serialized_data != nullptr)
	{
		delete[] serialized_data;
		serialized_data = nullptr;
	}
	serialized_length = 0;
	expected_length = 0;
	is_dirty = false;
	read_pointer = 0;
}

void HWSerializer::Write(U8 value)
{
	Write(&value, sizeof value);
}

void HWSerializer::Write(U16 value)
{
	if(is_big_endian)
	{
		U16 flip = (value << 8) | (value >> 8);
		Write(reinterpret_cast<U8*>(&flip), sizeof flip);
	}
	else
	{
		Write(reinterpret_cast<U8*>(&value), sizeof value);
	}
}

void HWSerializer::Write(U32 value)
{
	if(is_big_endian)
	{
		U32 flip = (value << 24) | ((value & 0xFF00) << 8) | ((value & 0xFF0000) >> 8) | (value >> 24);
		Write(reinterpret_cast<U8*>(&flip), sizeof flip);
	}
	else
	{
		Write(reinterpret_cast<U8*>(&value), sizeof value);
	}
}

void HWSerializer::Write(U64 value)
{
	if(is_big_endian)
	{
		U64 flip = (value << 56) |
					((value & 0xFF00) << 40) |
					((value & 0xFF0000) << 24) |
					((value & 0xFF000000) << 8) |
				    ((value & 0xFF00000000) >> 8) |
					((value & 0xFF0000000000) >> 24) |
					((value & 0xFF000000000000) >> 40) |
					(value >> 56);

		Write(reinterpret_cast<U8*>(&flip), sizeof flip);
	}
	else
	{
		Write(reinterpret_cast<U8*>(&value), sizeof value);
	}
}

void HWSerializer::Write(S8 value)
{
	Write(reinterpret_cast<U8*>(&value), 1);
}

void HWSerializer::Write(S16 value)
{
	Write(static_cast<U16>(value));
}

void HWSerializer::Write(S32 value)
{
	Write(static_cast<U32>(value));
}

void HWSerializer::Write(S64 value)
{
	Write(static_cast<U64>(value));
}

void HWSerializer::WriteVarInt(U64 value)
{
	while(value > 0x7F)
	{
		Write((U8)((value & 0x7F) | 0x80));
		value >>= 7;
	}
	Write((U8)value);
}

void HWSerializer::WriteVarInt(S64 value)
{
	U64 zig_zag = static_cast<U64>(value);
	zig_zag = (zig_zag << 1) | (zig_zag >> 31);
	WriteVarInt(zig_zag);
}

void HWSerializer::Write(const std::string& string)
{
	Write(reinterpret_cast<const U8*>(string.c_str()), string.length() + 1);
}

void HWSerializer::Write(const U8* data, S64 length)
{
	if(segments.empty() || segments.back()->used + length > segments.back()->length)
	{
		static const S64 default_allocation_size = 32;

		S64 size_to_allocate = default_allocation_size >= length ? default_allocation_size : length;

		segments.push_back(new HWSerializerSegment(size_to_allocate));
	}

	HWSerializerSegment* seg = segments.back();
	memcpy(seg->data + seg->used, data, length);
	seg->used += length;
		
	expected_length += length;
	is_dirty = true;
}

U8 HWSerializer::ReadU8()
{
	if(is_dirty) GetData(nullptr);
	return serialized_data[read_pointer++];
}

U16 HWSerializer::ReadU16()
{
	if(is_dirty) GetData(nullptr);
	U16 ret = serialized_data[read_pointer] | (serialized_data[read_pointer + 1] << 8);
	read_pointer += 2;
	return ret;
}

U32 HWSerializer::ReadU32()
{
	if(is_dirty) GetData(nullptr);
	U32 ret = serialized_data[read_pointer] |
			    (serialized_data[read_pointer + 1] << 8) |
			    (serialized_data[read_pointer + 2] << 16) |
			    (serialized_data[read_pointer + 3] << 24);
	read_pointer += 4;
	return ret;
}

U64 HWSerializer::ReadU64()
{
	if(is_dirty) GetData(nullptr);
	U64 ret = (U64)serialized_data[read_pointer] |
			    ((U64)serialized_data[read_pointer + 1] << 8) |
			    ((U64)serialized_data[read_pointer + 2] << 16) |
			    ((U64)serialized_data[read_pointer + 3] << 24) |
			    ((U64)serialized_data[read_pointer + 4] << 32) |
			    ((U64)serialized_data[read_pointer + 5] << 40) |
			    ((U64)serialized_data[read_pointer + 6] << 48) |
			    ((U64)serialized_data[read_pointer + 7] << 56);
	read_pointer += 8;
	return ret;
}

S8 HWSerializer::ReadS8()
{
	return static_cast<S8>(ReadU8());
}

S16 HWSerializer::ReadS16()
{
	return static_cast<S16>(ReadU16());
}

S32 HWSerializer::ReadS32()
{
	return static_cast<S32>(ReadU32());
}

S64 HWSerializer::ReadS64()
{
	return static_cast<S64>(ReadU64());
}

U64 HWSerializer::ReadVarInt()
{
	if(is_dirty) GetData(nullptr);
	S64 out = 0;
	while((serialized_data[read_pointer] & 0x80) != 0)
	{
		out = (out << 7) | (serialized_data[read_pointer] & 0x7F);
		read_pointer++;
	}
	out = (out << 7) | (serialized_data[read_pointer] & 0x7F);
	++read_pointer;
	return out;
}

std::string HWSerializer::ReadString()
{
	if(is_dirty) GetData(nullptr);

	char* pointer = reinterpret_cast<char*>(serialized_data + read_pointer);
	S64 length = strlen(pointer);
	read_pointer += length;
	return std::string(pointer);
}

void HWSerializer::ReadBytes(U8* out, S64 length)
{
	memcpy(out, serialized_data + read_pointer, length);
	read_pointer += length;
}

S64 HWSerializer::GetDataLength()
{
	if(is_dirty) GetData(nullptr);
	return serialized_length;
}

S64 HWSerializer::GetData(U8** data)
{
	if(is_dirty)
	{
		U64 pointer = 0;
		if(serialized_data != nullptr)
		{
			U8* temp = serialized_data;
			serialized_data = new U8[expected_length];
			memcpy(serialized_data, temp, serialized_length);
			delete[] temp;
		}
		else
		{
			serialized_data = new U8[expected_length];
		}

		std::vector<HWSerializerSegment*>::iterator iter = segments.begin();
		for(; iter != segments.end(); ++iter)
		{
			HWSerializerSegment* seg = *iter;
			memcpy(serialized_data + pointer, seg->data, seg->used);
			pointer += seg->used;
		}

		segments.clear();
		serialized_length = expected_length;
		is_dirty = false;
	}
	if(data != nullptr && serialized_data != nullptr)
	{
		memcpy(*data, serialized_data, serialized_length);
	}
	return serialized_length;
}

HWSerializer::HWSerializerSegment::HWSerializerSegment(S64 len)
{
	data = new U8[len];
	length = len;
	used = 0;
}

HWSerializer::HWSerializerSegment::~HWSerializerSegment()
{
	delete[] data;
}
