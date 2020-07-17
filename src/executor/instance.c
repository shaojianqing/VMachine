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
	instance->slotDataCount = class->fieldCount;
	instance->slotDataList = (SlotData *)calloc(class->fieldCount, sizeof(SlotData));

	return instance;
}