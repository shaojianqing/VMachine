typedef struct MethodDescriptor {

    u32 paramTypeCount;

    char *paramTypeList[64];

    char *returnType;

} MethodDescriptor;

MethodDescriptor *parseMethodDescriptor(char *descriptor);