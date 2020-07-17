#include <stdlib.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../resolver/class.h"

#include "dataStructure.h"
#include "localVariable.h"
#include "operandStack.h"
#include "byteReader.h"
#include "instruction.h"
#include "thread.h"
#include "stackFrame.h"

static void pushStack(RuntimeStack *this, StackFrame *stackFrame);

static StackFrame* popStack(RuntimeStack *this);

static StackFrame* peekStack(RuntimeStack *this);

static bool isStackEmpty(RuntimeStack *this);

StackFrame *createStackFrame(Thread *thread, Method *method) {
    if (method!=NULL) {
        StackFrame *stackFrame = (StackFrame *)calloc(1, sizeof(StackFrame));

        stackFrame->thread = thread;
        stackFrame->method = method;
        stackFrame->localVariables = createLocalVariables(method->maxLocalSize);
        stackFrame->operandStack = createOperandStack(method->maxStackSize);

        return stackFrame;
    }
    return NULL;
}

RuntimeStack *createRuntimeStack() {
    RuntimeStack *runtimeStack = (RuntimeStack *)calloc(1, sizeof(RuntimeStack));

    runtimeStack->pushStack = pushStack;
    runtimeStack->popStack = popStack;
    runtimeStack->peekStack = peekStack;
    runtimeStack->isStackEmpty = isStackEmpty;

    runtimeStack->count = 0;
    runtimeStack->frameStack = (StackFrame **)calloc(MAX_STACK_FRAME, sizeof(StackFrame *));
    return runtimeStack;
}

static void pushStack(RuntimeStack *this, StackFrame *stackFrame) {
    this->frameStack[this->count] = stackFrame;
    this->count++;
}

static StackFrame* popStack(RuntimeStack *this) {
     StackFrame *stackFrame = this->frameStack[this->count-1];
     this->count--;
     return stackFrame;
}

static StackFrame* peekStack(RuntimeStack *this) {
    return this->frameStack[this->count-1];
}

static bool isStackEmpty(RuntimeStack *this) {
    return this->count>0;
}
