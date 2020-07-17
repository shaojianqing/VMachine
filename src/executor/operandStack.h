typedef struct  OperandStack OperandStack;

struct OperandStack {

	u32 count;

    u32 capacity;

    SlotData *slotList;

    void (*pushInt)(OperandStack *this, int value);

    int (*popInt)(OperandStack *this);

    void (*pushFloat)(OperandStack *this, float value);

    float (*popFloat)(OperandStack *this);

    void (*pushLong)(OperandStack *this, long value);

    long (*popLong)(OperandStack *this);

    void (*pushDouble)(OperandStack *this, double value);

    double (*popDouble)(OperandStack *this);

    void (*pushReference)(OperandStack *this, void *reference);

    void* (*popReference)(OperandStack *this);

    void (*pushSlotData)(OperandStack *this, SlotData *slotData);

    SlotData* (*popSlotData)(OperandStack *this);
};

OperandStack *createOperandStack(u32 maxStack);