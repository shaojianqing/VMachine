#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "../common/constants.h"
#include "../common/commonType.h"
#include "../utils/utils.h"

#include "descriptor.h"

static u32 offset = 0;

static void validateParamStart(char *descriptor);

static void parseParameters(MethodDescriptor *methodDescriptor, char *parameter);

static char* parseObjectType(char *descriptor);

static char* parseArrayType(char *descriptor);

static void validateParamEnd(char *descriptor);

static void parseReturnType(MethodDescriptor *methodDescriptor, char *descriptor);

static void addMethodParameter(MethodDescriptor *methodDescriptor, char *parameter);

MethodDescriptor *parseMethodDescriptor(char *descriptor) {
    if (descriptor!=NULL) {
        offset = 0;
        MethodDescriptor *methodDescriptor = (MethodDescriptor *)calloc(1, sizeof(MethodDescriptor));
        methodDescriptor->paramTypeCount = 0;
        
        validateParamStart(descriptor);
        parseParameters(methodDescriptor, descriptor);
        validateParamEnd(descriptor);
        parseReturnType(methodDescriptor, descriptor);

        return methodDescriptor;
    }
    return NULL;
}

static void validateParamStart(char *descriptor) {
    if (descriptor[offset] == '(') {
        offset++;
    }
}

static void parseParameters(MethodDescriptor *methodDescriptor, char *descriptor) {

    while(descriptor[offset]!= ')' && offset<strlen(descriptor)) {
        if (descriptor[offset] == 'B') {
            addMethodParameter(methodDescriptor, "B");
        } else if (descriptor[offset] == 'C') {
            addMethodParameter(methodDescriptor, "C");
        } else if (descriptor[offset] == 'D') {
            addMethodParameter(methodDescriptor, "D");
        } else if (descriptor[offset] == 'F') {
            addMethodParameter(methodDescriptor, "F");
        } else if (descriptor[offset] == 'I') {
            addMethodParameter(methodDescriptor, "I");
        } else if (descriptor[offset] == 'J') {
            addMethodParameter(methodDescriptor, "J");
        } else if (descriptor[offset] == 'S') {
            addMethodParameter(methodDescriptor, "S");
        } else if (descriptor[offset] == 'Z') {
            addMethodParameter(methodDescriptor, "Z");
        } else if (descriptor[offset] == 'L') {
            char *paramType = parseObjectType(descriptor);
            addMethodParameter(methodDescriptor, paramType);
        } else if (descriptor[offset] == '[') {
            char *paramType = parseArrayType(descriptor);
            addMethodParameter(methodDescriptor, paramType);
        }
        offset++;
    }
}

static char* parseObjectType(char *descriptor){
    int index = indexOfString(descriptor, offset, ';');
    if (index>=0) {
        u32 i = 0;
        char *paramType = (char*)calloc(index-offset+1, sizeof(char));
        for (i=0;i<index-offset;++i) {
            paramType[i] = descriptor[i+offset];
        }
        offset = index;
        return paramType;
    }
    return NULL;
}

static char* parseArrayType(char *descriptor) {
    int index = indexOfString(descriptor, offset, ';');
    if (index>=0) {
        u32 i = 0;
        char *paramType = (char*)calloc(index-offset+1, sizeof(char));
        for (i=0;i<index-offset;++i) {
            paramType[i] = descriptor[i+offset];
        }
        offset = index;
        return paramType;
    }
    return NULL;
}

static void validateParamEnd(char *descriptor) {
    if (descriptor[offset] == ')') {
        offset++;
    }
}

static void parseReturnType(MethodDescriptor *methodDescriptor, char *descriptor) {
    if (descriptor[offset]=='V') {
        methodDescriptor->returnType = "V";
    } else if (descriptor[offset] == 'C') {
        methodDescriptor->returnType = "C";
    } else if (descriptor[offset] == 'D') {
        methodDescriptor->returnType = "D";
    } else if (descriptor[offset] == 'F') {
        methodDescriptor->returnType = "F";
    } else if (descriptor[offset] == 'I') {
        methodDescriptor->returnType = "I";
    } else if (descriptor[offset] == 'J') {
        methodDescriptor->returnType = "J";
    } else if (descriptor[offset] == 'S') {
        methodDescriptor->returnType = "S";
    } else if (descriptor[offset] == 'Z') {
        methodDescriptor->returnType = "Z";
    } else if (descriptor[offset] == 'L') {
        char *returnType = parseObjectType(descriptor);
        methodDescriptor->returnType = returnType;
    } else if (descriptor[offset] == '[') {
        char *returnType = parseArrayType(descriptor);
        methodDescriptor->returnType = returnType;
    }
}

static void addMethodParameter(MethodDescriptor *methodDescriptor, char *parameter) {
    if (methodDescriptor!=NULL && parameter!=NULL) {
        methodDescriptor->paramTypeList[methodDescriptor->paramTypeCount] = parameter;
        methodDescriptor->paramTypeCount++;
    }
}