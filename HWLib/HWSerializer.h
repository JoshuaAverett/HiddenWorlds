#pragma once

#include "Master.h"
#include <string>

#include <vector>

class HWSerializer
{
public:
	HWSerializer();
	~HWSerializer();

	void Clear();

	void Write(U8 value);
	void Write(U16 value);
	void Write(U32 value);
	void Write(U64 value);

	void Write(S8 value);
	void Write(S16 value);
	void Write(S32 value);
	void Write(S64 value);

	void WriteVarInt(S64 value);
	void WriteVarInt(U64 value);
	void Write(const std::string& string);
	void Write(const U8* data, S64 length);

	U8 ReadU8();
	U16 ReadU16();
	U32 ReadU32();
	U64 ReadU64();

	S8 ReadS8();
	S16 ReadS16();
	S32 ReadS32();
	S64 ReadS64();

	U64 ReadVarInt();
	std::string ReadString();
	void ReadBytes(U8* out, S64 length);

	S64 GetDataLength();
	S64 GetData(U8** data);

private:
	class HWSerializerSegment
	{
	public:
		HWSerializerSegment(S64 len);
		HWSerializerSegment(const HWSerializerSegment& copy);
		~HWSerializerSegment();

		U8* data;
		S64 length;
		S64 used;
	};

	std::vector<HWSerializerSegment*> segments;
	U8* serialized_data;
	S64 serialized_length;
	S64 expected_length;
	bool is_dirty;
	S64 read_pointer;

	static bool is_big_endian;
	static bool initialized;
};
