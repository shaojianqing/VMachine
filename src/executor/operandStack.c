#include <stdlib.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../resolver/class.h"

#include "dataStructure.h"
#include "operandStack.h"

static void pushInt(OperandStack *this, int value);

static int popInt(OperandStack *this);

static void pushFloat(OperandStack *this, float value);

static float popFloat(OperandStack *this);

static void pushLong(OperandStack *this, long value);

static long popLong(OperandStack *this);

static void pushDouble(OperandStack *this, double value);

static double popDouble(OperandStack *this);

static void pushReference(OperandStack *this, void *reference);

static void* popReference(OperandStack *this);

static void pushSlotData(OperandStack *this, SlotData *slotData);

static SlotData* popSlotData(OperandStack *this);

OperandStack *createOperandStack(u32 maxStack) {
    if (maxStack>0) {
        OperandStack *operandStack = (OperandStack *)calloc(1, sizeof(OperandStack));
        operandStack->count = 0;
        operandStack->capacity = maxStack;
        operandStack->slotList = (SlotData *)calloc(maxStack, sizeof(SlotData));

        operandStack->pushInt = pushInt;
        operandStack->popInt = popInt;
        operandStack->pushFloat = pushFloat;
        operandStack->popFloat = popFloat;
        operandStack->pushLong = pushLong;
        operandStack->popLong = popLong;
        operandStack->pushDouble = pushDouble;
        operandStack->popDouble = popDouble;
        operandStack->pushReference = pushReference;
        operandStack->popReference = popReference;
        operandStack->pushSlotData = pushSlotData;
        operandStack->popSlotData = popSlotData;

        return operandStack;
    }
    return NULL;
}

static void pushInt(OperandStack *this, int value) {
    this->slotList[this->count].intValue = value;
    this->count++;
}

static int popInt(OperandStack *this) {
    this->count--;
    return this->slotList[this->count].intValue;
}

static void pushFloat(OperandStack *this, float value){
    this->slotList[this->count].floatValue = value;
    this->count++;
}

static float popFloat(OperandStack *this) {
    this->count--;
    return this->slotList[this->count].floatValue;
}

static void pushLong(OperandStack *this, long value) {
    this->slotList[this->count].longValue = value;
    this->count++;
}

static long popLong(OperandStack *this) {
    this->count--;
    return this->slotList[this->count].longValue;
}

static void pushDouble(OperandStack *this, double value) {
    this->slotList[this->count].doubleValue = value;
    this->count++;
}

static double popDouble(OperandStack *this) {
    this->count--;
    return this->slotList[this->count].doubleValue;
}

static void pushReference(OperandStack *this, void *reference) {
    this->slotList[this->count].reference = reference;
    this->count++;
}

static void* popReference(OperandStack *this) {
    this->count--;
    return this->slotList[this->count].reference;
}

static void pushSlotData(OperandStack *this, SlotData *slotData) {
    this->slotList[this->count] = *slotData;
    this->count++;
}

static SlotData* popSlotData(OperandStack *this) {
    this->count--;
    return &(this->slotList[this->count]);
}