#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "common/constants.h"
#include "common/commonType.h"
#include "resolver/class.h"
#include "executor/dataStructure.h"
#include "executor/localVariable.h"
#include "executor/operandStack.h"
#include "executor/byteReader.h"
#include "executor/stackFrame.h"
#include "executor/instruction.h"
#include "executor/descriptor.h"
#include "datatype/dataType.h"
#include "hashmap/hashMap.h"
#include "runtime/vmachine.h"
#include "utils/utils.h"

VMachine *vmachine;

int main(int argc, char **argv) {
	
	if (argc<2) {
		printf("Parameter number not correct!!!\n");
		return 0;	
	}

	char *filename = argv[1];

    if (filename==NULL || strlen(filename)==0 || !isEndWith(filename, ".jar")) {
        printf("Parameter not correct, it should be jar file!!!\n");
		return 0;	
    }

	vmachine = buildVMachine();
	vmachine->initClassFromRuntimeJarFile(vmachine);
    vmachine->initClassFromUserDefineJarFile(vmachine, filename);
	vmachine->initMainEntryClass(vmachine, "com/sys/main/Main");
	vmachine->startVMachine(vmachine);

	/*Class *class = vmachine->mainEntryClass;
	Method *mainMethod = class->findMainMethod(class);

	MethodDescriptor *methodDescriptor = parseMethodDescriptor("([Lcom/sys/domain/UserInfo;Ljava/lang/String;IJDLjava/lang/String;)Lcom/sys/domain/UserInfo;");

	printf("MethodDescriptor paramTypeCount:		%d\n", methodDescriptor->paramTypeCount);
	printf("MethodDescriptor paramTypeList:			%s\n", methodDescriptor->paramTypeList[0]);
	printf("MethodDescriptor paramTypeList:			%s\n", methodDescriptor->paramTypeList[1]);
	printf("MethodDescriptor paramTypeList:			%s\n", methodDescriptor->paramTypeList[2]);
	printf("MethodDescriptor paramTypeList:			%s\n", methodDescriptor->paramTypeList[3]);
	printf("MethodDescriptor paramTypeList:			%s\n", methodDescriptor->paramTypeList[4]);
	printf("MethodDescriptor paramTypeList:			%s\n", methodDescriptor->paramTypeList[5]);
	printf("MethodDescriptor returnType:			%s\n", methodDescriptor->returnType);


	printf("MethodName:			%s\n", mainMethod->methodName);
	printf("MethodName:			%s\n", mainMethod->descriptor);
	printf("\n\n");

	printf("ClassName:			%s\n", class->className);
	printf("SuperName:			%s\n", class->superClassName);
	printf("InterfaceCount:			%d\n", class->interfaceCount);
	printf("FieldCount:			%d\n", class->fieldCount);
	if (class->fieldCount>0) {
		printf("FieldName:			%s\n", class->fieldList[0].fieldName);
		printf("FieldDescriptor:		%s\n", class->fieldList[0].descriptor);
	}
	if (class->methodCount>0) {
		printf("MethodName:			%s\n", class->methodList[0].methodName);
		printf("MethodDescriptor:		%s\n", class->methodList[0].descriptor);
	}

	printf("\n\n\n\n");

	printf("Class Magic:					%X\n", class->magic);
	printf("Class MinorVersion:				%d\n", class->minorVersion);
	printf("Class majorVersion:				%d\n", class->majorVersion);
	printf("Class constPoolCount:				%d\n", class->constPoolCount);
	printf("Class accessFlags:				%d\n", class->accessFlags);
	printf("Class thisClass:				%s\n", class->className);
	printf("Class superClass:				%s\n", class->superClassName);
	printf("Class interfaceCount:				%d\n", class->interfaceCount);
	printf("Class fieldCount:				%d\n", class->fieldCount);
	printf("Class methodCount:				%d\n", class->methodCount);
	printf("Class methodName:			%s\n", class->methodList[2].methodName);
	printf("Class methodDescriptor:		%s\n", class->methodList[2].descriptor);

	printf("Class attributeCount:				%d\n", class->attributeCount);
	if (class->attributeCount>0) {
		printf("Class attributeName:				%s\n", class->attributeList[0].attributeName);
		printf("Class attributeValue:				%s\n", class->attributeList[0].data);
	}

	printf("Start to print UTF-8 String Value!\n\n");*/
}
