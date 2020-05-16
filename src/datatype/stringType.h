#define DEFALUT_STRING_SPACE		16

typedef struct String String;

struct String {
	
	int length;

	int free;

	char *value;

	u32 (*getLength)(String *this);

	char* (*getValue)(String *this);

	String* (*subString)(String *this, int start, int end);

	String* (*catString)(String *this, String *string);

};

String* createString(char *value);

String* createStringWithRange(char *value, int range);
