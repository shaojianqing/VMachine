
typedef struct Class Class;

typedef struct Field Field;

typedef struct Method Method;

struct Field {

	Class *class;

	u16 accessFlags;

	String methodName;

	String descriptor;

	u32 constValueIdx;

	u32 slotId;
};

struct Method {

	Class *class;

	u16 accessFlags;

	String methodName;

	String descriptor;
	
	u32 maxStackSize;

	u32 maxLocalSize;

	byte *code;
};

struct Class {

	u16 accessFlags;

	String className;

	String superClassName;

	String *interfaceNames;

	ConstPool *constantPool;

	Field *fieldList;

	Method *methodList;

	Class *superClass;

	Class *interfaces;

	bool (*isPublic)(Class *this);

	bool (*isFinal)(Class *this);

	bool (*isSuper)(Class *this);

	bool (*isInterface)(Class *this);

	bool (*isAbstract)(Class *this);

	bool (*isSynthetic)(Class *this);

	bool (*isAnnotation)(Class *this);

	bool (*isEnum)(Class *this);

	bool (*isInterface)(Class *this);
};

Class* initializeClass(ClassType *classType);
