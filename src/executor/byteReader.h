
typedef struct ByteReader ByteReader;

struct ByteReader {

	byte* code;

	u32 length;

	u32 pc;

	byte (*readByte)(ByteReader *this);

	short (*readShort)(ByteReader *this);

	int (*readInteger)(ByteReader *this);

};

ByteReader *createByteReader(byte* code, u32 length, u32 pc);
