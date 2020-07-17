#define LIMIT_FACTOR 0.75

#define STATUS_SUCCESS	1
#define STATUS_FAILURE  0

typedef struct Entry Entry;

typedef struct HashMap HashMap;

struct Entry {
	
	Object *key;
	
	Object *value;

	Entry *next;	
};

struct HashMap {
	
	Entry **table;

	u32 count;

	u32 limit;

	u32 capacity;

	u32 (*put)(HashMap *this, Object *key, Object *value);

	Object* (*get)(HashMap *this, Object *key);

	u32 (*getCount)(HashMap *this);

	u32 (*remove)(HashMap *this, Object *key);

	bool (*containsKey)(HashMap *this, Object *key);

	void (*printHashMap)(HashMap *this);

	HashCode hashCode;

	EqualFun equalFun;
};

HashMap* createHashMap(HashCode hashCode, EqualFun equalFun, u32 capacity);

void destroyHashMap(HashMap* this);