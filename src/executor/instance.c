#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "../common/constants.h"
#include "../common/commonType.h"
#include "../resolver/class.h"

#include "dataStructure.h"
#include "instance.h"

Instance *createInstance(Class *class) {
	Instance *instance = (Instance *)calloc(1, sizeof(Instance));
	instance->class = class;
    int i = 0, k = 0, fieldCount = 0;
    for (i=0;i<class->fieldCount;++i) {
        if (!isStatic(class->fieldList[i].accessFlags)) {
            fieldCount++;
        }
    }

	instance->fieldDataCount = fieldCount;
	instance->fieldDataList = (SlotData *)calloc(fieldCount, sizeof(SlotData));
	return instance;
}

int getInteger(Instance *instance, u32 slotId) {
    return instance->fieldDataList[slotId].intValue;
}

void setInteger(Instance *instance, u32 slotId, int intValue) {
    instance->fieldDataList[slotId].intValue = intValue;
}

long getLong(Instance *instance, u32 slotId) {
    return instance->fieldDataList[slotId].longValue;
}

void setLong(Instance *instance, u32 slotId, long longValue) {
    instance->fieldDataList[slotId].longValue = longValue;
}

float getFloat(Instance *instance, u32 slotId) {
    return instance->fieldDataList[slotId].floatValue;
}

void setFloat(Instance *instance, u32 slotId, float floatValue) {
    instance->fieldDataList[slotId].floatValue = floatValue;
}

double getDouble(Instance *instance, u32 slotId) {
    return instance->fieldDataList[slotId].doubleValue;
}

void setDouble(Instance *instance, u32 slotId, double doubleValue) {
    instance->fieldDataList[slotId].doubleValue = doubleValue;
}

void* getReference(Instance *instance, u32 slotId) {
    return instance->fieldDataList[slotId].reference;
}

void setReference(Instance *instance, u32 slotId, void* reference) {
    instance->fieldDataList[slotId].reference = reference;
}