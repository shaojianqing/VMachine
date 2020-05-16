typedef struct ConstUtf8Info {

    u8 tag;

    u16 length;

    byte* bytes;

} ConstUtf8Info;


typedef struct ConstIntegerInfo {

    u8 tag;

    u32 bytes;

} ConstIntegerInfo;

typedef struct ConstFloatInfo {

    u8 tag;

    u32 bytes;

} ConstFloatInfo;

typedef struct ConstLongInfo {

    u8 tag;

    u32 highBytes;

    u32 lowBytes;

} ConstLongInfo;


typedef struct ConstDoubleInfo {

    u8 tag;

    u32 highBytes;

    u32 lowBytes;

} ConstDoubleInfo;


typedef struct ConstClassInfo {

    u8 tag;

    u16 nameIndex;

} ConstClassInfo;

typedef struct ConstStringInfo {

    u8 tag;

    u16 stringIndex;

} ConstStringInfo;

typedef struct ConstFieldRefInfo {

    u8 tag;

    u16 classIndex;

    u16 nameAndTypeIndex;

} ConstFieldRefInfo;

typedef struct ConstMethodRefInfo {

    u8 tag;

    u16 classIndex;

    u16 nameAndTypeIndex;

} ConstMethodRefInfo;

typedef struct ConstInterfaceMethodRefInfo {

    u8 tag;

    u16 classIndex;

    u16 nameAndTypeIndex;

} ConstInterfaceMethodRefInfo;

typedef struct ConstNameAndTypeInfo {

    u8 tag;

    u16 nameIndex;

    u16 descriptorIndex;

} ConstNameAndTypeInfo;

typedef struct ConstMethodHandleInfo {

    u8 tag;

    u8 referenceKind;

    u16 referenceIndex;

} ConstMethodHandleInfo;

typedef struct ConstMethodTypeInfo {

    u8 tag;

    u16 descriptorIndex;

} ConstMethodTypeInfo;

typedef struct ConstInvokeDynamicInfo {

    u8 tag;

    u16 bootstrapMethodAttrIndex;

    u16 nameAndTypeIndex;

} ConstInvokeDynamicInfo;


typedef struct ConstPool {

	u8 type;

	void *value;

} ConstPool;

typedef struct AttributeInfo {
	
	u16 nameIndex;

	u32	attrLength;

	byte *data;

} AttributeInfo;

typedef struct MethodInfo {

	u16 accessFlags;

	u16	nameIndex;

	u16	descriptorIndex;

	u16 attributeCount;

	AttributeInfo *attributeList;

} MethodInfo;

typedef struct FieldInfo {

	u16 accessFlags;

	u16	nameIndex;

	u16	descriptorIndex;

	u16 attributeCount;

	AttributeInfo *attributeList;
	
} FieldInfo;

typedef struct ClassType {

	u32 magic;

	u16 minorVersion;

	u16 majorVersion;

	u16 constPoolCount;

	ConstPool *constPool;

	u16 accessFlags;
	
	u16 thisClass;

	u16 superClass;

	u16 interfaceCount;

	u16* interfaceList;

	u16 fieldCount;

	FieldInfo *fieldList;

	u16 methodCount;

	MethodInfo *methodList;

	u16 attributeCount;

	AttributeInfo *attributeList;

} ClassType;

ClassType* loadClassData(char *filename);

void resolveClassData(ClassType *classType, byte *data);








