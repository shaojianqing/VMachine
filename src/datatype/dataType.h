typedef int (*HashCode)(char *key);

typedef bool (*EqualFun)(char *obj1, char *obj2);

int StringHashCode(char *string);

bool StringEqualFun(char *obj1, char *obj2);