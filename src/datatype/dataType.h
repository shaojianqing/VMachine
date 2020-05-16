typedef int (*HashCode)(Object *key);

typedef bool (*EqualFun)(Object *obj1, Object *obj2);

int StringHashCode(Object *string);

bool StringEqualFun(Object *obj1, Object *obj2);
