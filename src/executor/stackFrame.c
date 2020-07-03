#include <stdlib.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../resolver/class.h"

#include "slotType.h"
#include "localVariable.h"
#include "operandStack.h"
#include "byteReader.h"
#include "instruction.h"
#include "stackFrame.h"

StackFrame *createStackFrame(Method *method) {
    if (method!=NULL) {
        StackFrame *stackFrame = (StackFrame *)malloc(sizeof(StackFrame));

        stackFrame->method = method;
        stackFrame->localVariables = createLocalVariables(method->maxLocalSize);
        stackFrame->operandStack = createOperandStack(method->maxStackSize);

        return stackFrame;
    }
    return NULL;
}