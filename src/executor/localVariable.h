typedef struct  LocalVariables LocalVariables;

struct LocalVariables {

    SlotData *slotList;

    void (*setInt)(LocalVariables *this, u32 index, int value);

    int (*getInt)(LocalVariables *this, u32 index);

    void (*setFloat)(LocalVariables *this, u32 index, float value);

    float (*getFloat)(LocalVariables *this, u32 index);

    void (*setLong)(LocalVariables *this, u32 index, long value);

    long (*getLong)(LocalVariables *this, u32 index);

    void (*setDouble)(LocalVariables *this, u32 index, double value);

    double (*getDouble)(LocalVariables *this, u32 index);

    void (*setReference)(LocalVariables *this, u32 index, void* reference);

    void* (*getReference)(LocalVariables *this, u32 index);
};

LocalVariables *createLocalVariables(u32 maxLocalVariables);