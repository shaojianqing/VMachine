#include <stdlib.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"

#include "slotType.h"
#include "localVariable.h"

static void setInt(LocalVariables *this, u32 index, int value);

static int getInt(LocalVariables *this, u32 index);

static void setFloat(LocalVariables *this, u32 index, float value);

static float getFloat(LocalVariables *this, u32 index);

static void setLong(LocalVariables *this, u32 index, long value);

static long getLong(LocalVariables *this, u32 index);

static void setDouble(LocalVariables *this, u32 index, double value);

static double getDouble(LocalVariables *this, u32 index);

static void setReference(LocalVariables *this, u32 index, void* reference);

static void* getReference(LocalVariables *this, u32 index);

LocalVariables *createLocalVariables(u32 maxLocalVariables) {
    if (maxLocalVariables>0) {
        LocalVariables *localVariables = (LocalVariables *)malloc(sizeof(LocalVariables));
        localVariables->slotList = (SlotData *)calloc(maxLocalVariables, sizeof(SlotData));
        localVariables->setInt = setInt;
        localVariables->getInt = getInt;
        localVariables->setFloat = setFloat;
        localVariables->getFloat = getFloat;
        localVariables->setLong = setLong;
        localVariables->getLong = getLong;
        localVariables->setDouble = setDouble;
        localVariables->getDouble = getDouble;
        localVariables->setReference = setReference;
        localVariables->getReference = getReference;

        return localVariables;
    }
    return NULL;
}

static void setInt(LocalVariables *this, u32 index, int value) {
    this->slotList[index].intValue = value;
}

static int getInt(LocalVariables *this, u32 index) {
    return this->slotList[index].intValue;
}

static void setFloat(LocalVariables *this, u32 index, float value) {
    this->slotList[index].floatValue = value;
}

static float getFloat(LocalVariables *this, u32 index) {
    return this->slotList[index].floatValue;
}

static void setLong(LocalVariables *this, u32 index, long value) {
    this->slotList[index].longValue = value;
}

static long getLong(LocalVariables *this, u32 index) {
    return this->slotList[index].longValue;
}

static void setDouble(LocalVariables *this, u32 index, double value) {
    this->slotList[index].doubleValue = value;
}

static double getDouble(LocalVariables *this, u32 index) {
    return this->slotList[index].doubleValue;
}

static void setReference(LocalVariables *this, u32 index, void* reference) {
    this->slotList[index].reference = reference;
}

static void* getReference(LocalVariables *this, u32 index) {
    return this->slotList[index].reference;
}