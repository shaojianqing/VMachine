#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../resolver/class.h"
#include "../datatype/dataType.h"
#include "../hashmap/hashMap.h"
#include "../runtime/vmachine.h"

#include "dataStructure.h"
#include "localVariable.h"
#include "operandStack.h"
#include "byteReader.h"
#include "instruction.h"
#include "stackFrame.h"
#include "thread.h"

static void pushStackFrame(Thread* thread, StackFrame *stackFrame);

static StackFrame* popStackFrame(Thread* thread);

static StackFrame* peekStackFrame(Thread* thread);

static void startThread(Thread* thread, Method *method);

Thread* createThread(VMachine *vmachine) {
    if (vmachine!=NULL) {
        Thread *thread = (Thread *)calloc(1, sizeof(Thread));
        thread->vmachine = vmachine;
        thread->runtimeStack = createRuntimeStack();

        thread->pushStackFrame = pushStackFrame;
        thread->popStackFrame = popStackFrame;
        thread->peekStackFrame = peekStackFrame;
        thread->startThread = startThread;

        return thread;
    }
}

static void startThread(Thread* thread, Method *method) {
    StackFrame *stackFrame = createStackFrame(thread, method);
	ByteReader *byteReader = createByteReader(method->codeData, method->codeLength, 0);

    thread->pushStackFrame(thread, stackFrame);
	LocalVariables *localVariables = stackFrame->localVariables;

	while(byteReader->pc < byteReader->length) {
		u32 pc = byteReader->pc;
        
		u8 operCode = byteReader->readByte(byteReader);

		Instruction *instruction = getInstructionByCode(operCode);

		printf("PC:%02d %s\n", pc, instruction->name);
		instruction->fetcher(byteReader, stackFrame);
		instruction->processor(stackFrame);

		u32 i=0;
		for (i=0;i<localVariables->slotDataCount;++i) {
			int value = localVariables->getInt(localVariables, i);
			printf("	LocalVariables:%d\n", value);
		}
	}
    printf("byteReader->pc:%d\n", byteReader->pc);
}

static void pushStackFrame(Thread* thread, StackFrame *stackFrame) {
    RuntimeStack *runtimeStack = thread->runtimeStack;
    runtimeStack->pushStack(runtimeStack, stackFrame);
}

static StackFrame* popStackFrame(Thread* thread) {
    RuntimeStack *runtimeStack = thread->runtimeStack;
    return runtimeStack->popStack(runtimeStack);
}

static StackFrame* peekStackFrame(Thread* thread) {
    RuntimeStack *runtimeStack = thread->runtimeStack;
    return runtimeStack->peekStack(runtimeStack);
}