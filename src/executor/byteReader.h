
typedef struct ByteReader ByteReader;

struct ByteReader {

	u32 pc;

	u32 length;

	byte* code;

	byte (*readByte)(ByteReader *this);

	short (*readShort)(ByteReader *this);

	int (*readInteger)(ByteReader *this);

	void (*setPC)(ByteReader *this, u32 pc);
};

ByteReader *createByteReader(byte* code, u32 length, u32 pc);
