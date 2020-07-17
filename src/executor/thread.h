typedef struct Thread Thread;

typedef struct VMachine VMachine;

typedef struct RuntimeStack RuntimeStack;

struct Thread {

    VMachine *vmachine;

    RuntimeStack *runtimeStack;

    void (*pushStackFrame)(Thread* thread, StackFrame *stackFrame);

    StackFrame* (*popStackFrame)(Thread* thread);

    StackFrame* (*peekStackFrame)(Thread* thread);

    void (*startThread)(Thread* thread, Method *method);
};

Thread* createThread(VMachine *vmachine);
