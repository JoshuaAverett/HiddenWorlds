#pragma once

#include "Master.h"
#include "HWSocket.h"
#include "HWSerializer.h"

enum class HWPacketType : U32
{
	Test,
	CreateCircuit,
	PACKET_COUNT
};

enum class HWPacketFlag : U8
{
	Reliable		= 1 << 0,
	Encrypted		= 1 << 1,
	HasAcks			= 1 << 2,
	Resent			= 1 << 3,
};

class HWPacket
{
public:
	virtual void Serialize(HWSerializer& stream) const = 0;
	virtual void Deserialize(HWSerializer& stream) = 0;

	bool HasFlag(HWPacketFlag flag) const
	{
		return (flags & (U8)flag) != 0;
	}

	void SetFlag(HWPacketFlag flag)
	{
		flags |= (U8)flag;
	}

	HWPacketType GetPacketType() const
	{
		return packet_type;
	}

protected:
	HWPacketType packet_type;
	U8 flags;

	void SerializeHeader(HWSerializer& stream) const
	{
		stream.WriteVarInt((U64)packet_type);
		stream.Write(flags);
	}

	void DeserializeHeader(HWSerializer& stream)
	{
		// packet type already known at deserialization time
		flags = stream.ReadU8();
	}
};

class HWUDPPacket
{
public:
	HWUDPPacket(U32 _id, U32 _seq_num, U32 _seq_size, HWSerializer& stream) :
		packet_id(_id),
		sequence_number(_seq_num),
		sequence_size(_seq_size)
	{
		body = new U8[stream.GetDataLength()];
		length = stream.GetData(&body);
		ack_list_length = 0;
		ack_list = nullptr;
	}

	HWUDPPacket(HWSerializer& stream)
	{
		Deserialize(stream);
	}

	~HWUDPPacket()
	{
		delete[] body;
	}

	void Serialize(HWSerializer& stream) const
	{
		stream.WriteVarInt(packet_id);

		stream.WriteVarInt(sequence_number);
		if(sequence_number == 0)
		{
			stream.WriteVarInt(sequence_size);
		}

		stream.WriteVarInt(ack_list_length);
		if(ack_list != nullptr && ack_list_length != 0)
		{
			for(int i = 0; i < ack_list_length; ++i)
			{
				stream.WriteVarInt(ack_list[i]);
			}
		}
		stream.WriteVarInt((U64)length);
		stream.Write(body, length);
	}

	void Deserialize(HWSerializer& stream)
	{
		packet_id = stream.ReadVarInt();
		sequence_number = stream.ReadVarInt();
		if(sequence_number == 0)
		{
			sequence_size = stream.ReadVarInt();
		}
		else
		{
			sequence_size = 0;
		}

		ack_list_length = stream.ReadVarInt();
		if(ack_list_length)
		{
			ack_list = new U64[ack_list_length];
			for(S32 i = 0; i < ack_list_length; ++i)
			{
				ack_list[i] = stream.ReadVarInt();
			}
		}
		else
		{
			ack_list = nullptr;
		}

		length = stream.ReadVarInt();
		body = new U8[length];
		stream.ReadBytes(body, length);
	}

	U64 GetPacketID() const
	{
		return packet_id;
	}

	U64 GetSequenceNumber() const
	{
		return sequence_number;
	}

	U64 GetSequenceSize() const
	{
		return sequence_size;
	}

	U8* GetBody() const
	{
		return body;
	}

	S64 GetBodyLength() const
	{
		return length;
	}

private:
	// The unique packet ID of the full packet
	U64 packet_id;

	// The index into the full packet that this is a sub-section of
	U64 sequence_number;

	//The length of the full packet in sub-sections
	U64 sequence_size;

	// The list of all UDP packets that have been recieved since the last sending
	U64* ack_list;

	// The length of the ack list
	S64 ack_list_length;

	// The body of the packet's data
	U8* body;

	// The length of the body data
	S64 length;
};
