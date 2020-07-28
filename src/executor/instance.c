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
	instance->fieldDataList = (FieldData *)calloc(fieldCount, sizeof(FieldData));
    for (i=0,k=0;i<class->fieldCount;++i) {
        if (!isStatic(class->fieldList[i].accessFlags)) {
            instance->fieldDataList[k].field = &class->fieldList[i];
            k++;
        }
    }

	return instance;
}