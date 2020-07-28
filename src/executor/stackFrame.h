
typedef struct Thread Thread;

typedef struct RuntimeStack RuntimeStack;

typedef struct StackFrame StackFrame;

struct StackFrame {

    LocalVariables *localVariables;

    OperandStack *operandStack;

    ByteReader *byteReader;

    Thread *thread;

    Method *method;
};

struct RuntimeStack {

    int count;

    StackFrame **frameStack;

    void (*pushStack)(RuntimeStack *this, StackFrame *stackFrame);

    StackFrame* (*popStack)(RuntimeStack *this);

    StackFrame* (*peekStack)(RuntimeStack *this);

    bool (*isStackEmpty)(RuntimeStack *this);
};

StackFrame *createStackFrame(Thread *thread, Method *method, ByteReader *byteReader);

RuntimeStack *createRuntimeStack();