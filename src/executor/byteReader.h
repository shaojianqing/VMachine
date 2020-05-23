
typedef struct ByteReader ByteReader;

struct ByteReader {

	byte* code;

	u32 length;

	u32 pc;

	u8 (*readByte)(ByteReader *this);

	u16 (*readShort)(ByteReader *this);

	u32 (*readInteger)(ByteReader *this);

};

ByteReader *createByteReader(byte* code, u32 length, u32 pc);
