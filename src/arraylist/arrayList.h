typedef struct ArrayList ArrayList;

struct ArrayList {
	
	Object **list;

	u32 capacity;

	u32 size;

	u32 (*getSize)(ArrayList *this);

	bool (*add)(ArrayList *this, Object *object);

	Object* (*get)(ArrayList *this, u32 index);

	bool (*remove)(ArrayList *this, Object *object);
	
	bool (*containsObject)(ArrayList *this, Object *object);

	EqualFun equalFun;

};

ArrayList *createArrayList(EqualFun equalFun, u32 capacity);

void destroyArrayList(ArrayList *this);
