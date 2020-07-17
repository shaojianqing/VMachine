
typedef struct Thread Thread;

typedef struct Instruction Instruction;

typedef struct StackFrame StackFrame;

typedef union OperandStore OperandStore;

typedef struct BranchOperand BranchOperand;

typedef struct Index8Operand Index8Operand;

typedef struct Index16Operand Index16Operand;

typedef struct BipushOperand BipushOperand;

typedef struct SipushOperand SipushOperand;

typedef struct WideOperand WideOperand;

typedef struct IIncOperand IIncOperand;

typedef struct GotoWOperand GotoWOperand;

typedef struct RuntimeStack RuntimeStack;

typedef struct NewArrayOperand NewArrayOperand;

typedef struct TableSwitchOperand TableSwitchOperand;

typedef struct LookupSwitchOperand LookupSwitchOperand;

typedef struct MultiANewArrayOperand MultiANewArrayOperand;

typedef struct InvokeInterfaceOperand InvokeInterfaceOperand;

struct BranchOperand {
    int offset;
};

struct Index8Operand {
    u32 index;
};

struct Index16Operand {
    u32 index;
};

struct BipushOperand {
    byte value;
};

struct SipushOperand {
    short value;
};

struct WideOperand {
    Instruction *instruction;
};

struct IIncOperand {
    u32 index;

    int constValue;
};

struct GotoWOperand {
    int offset;
};

struct NewArrayOperand {
    u8 atype;
};

struct LookupSwitchOperand {
    
    int defaultOffset;

    int npairs;

    int *matchOffsets;
};

struct TableSwitchOperand {
    
    int defaultOffset;

    int low;

    int high;

    int *jumpOffsets;
};

struct MultiANewArrayOperand {

    u16 index;

    u8 dimensions;
};

struct InvokeInterfaceOperand {
    u32 index;
};

union OperandStore {

    BranchOperand branchOperand;

    Index8Operand index8Operand;

    Index16Operand index16Operand;

    BipushOperand bipushOperand;

    SipushOperand sipushOperand;

    IIncOperand iincOperand;

    WideOperand wideOperand;

    GotoWOperand gotoWOperand;

    NewArrayOperand newArrayOperand;

    TableSwitchOperand tableSwitchOperand;

    LookupSwitchOperand lookupSwitchOperand;

    InvokeInterfaceOperand invokeInterfaceOperand;
};

struct StackFrame {

    OperandStore operandStore;

    LocalVariables *localVariables;

    OperandStack *operandStack;

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

StackFrame *createStackFrame(Thread *thread, Method *method);

RuntimeStack *createRuntimeStack();