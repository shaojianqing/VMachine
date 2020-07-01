#include <stdlib.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../endian/endianSwap.h"

#include "resolver.h"
#include "class.h"

static bool isPublic(Class *this);

static bool isFinal(Class *this);

static bool isSuper(Class *this);

static bool isInterface(Class *this);

static bool isAbstract(Class *this);

static bool isSynthetic(Class *this);

static bool isAnnotation(Class *this);

static bool isEnum(Class *this);

static bool isInterface(Class *this);

static void initFuncations(Class *class);

static void initClassData(Class *class, ClassType *classType);

static ConstPool* getConstant(Class *this, u32 index);

static Field* findField(Class *this, char *fieldName);

static Method* findMethod(Class *this, char *methodName, char *descriptor);

static Method* findMainMethod(Class *this);

Class* initializeClass(ClassType *classType) {
	if (classType!=NULL) {
		Class *class = (Class *)calloc(1, sizeof(Class));
		if (class!=NULL) {
			initFuncations(class);			
			initClassData(class, classType);
			return class;
		}
	}
	return NULL;
}

static void initFuncations(Class *class) {
	class->isPublic = isPublic;
	class->isFinal = isFinal;
	class->isSuper = isSuper;
	class->isInterface = isInterface;
	class->isAbstract = isAbstract;
	class->isSynthetic = isSynthetic;
	class->isAnnotation = isAnnotation;
	class->isEnum = isEnum;

	class->getConstant = getConstant;
	class->findField = findField;
	class->findMethod = findMethod;
	class->findMainMethod = findMainMethod;
}

static void initClassData(Class *class, ClassType *classType) {

	int i = 0, j = 0, k = 0;
	ConstPool constant;
	ConstUtf8Info *constUtf8Info;
	ConstFieldRefInfo *constFieldRefInfo;
	ConstNameAndTypeInfo *constNameAndTypeInfo;

	class->accessFlags = classType->accessFlags;
	class->constPool = classType->constPool;

	constant = classType->constPool[classType->thisClass];
	constNameAndTypeInfo = (ConstNameAndTypeInfo *)constant.value;
	constant = classType->constPool[constNameAndTypeInfo->nameIndex];
	constUtf8Info = (ConstUtf8Info *)constant.value;
	class->className = constUtf8Info->bytes;

	constant = classType->constPool[classType->superClass];
	constNameAndTypeInfo = (ConstNameAndTypeInfo *)constant.value;
	constant = classType->constPool[constNameAndTypeInfo->nameIndex];
	constUtf8Info = (ConstUtf8Info *)constant.value;
	class->superClassName = constUtf8Info->bytes;

	if (classType->interfaceCount>0) {
		class->interfaceCount = classType->interfaceCount;
		class->interfaceNames = (char **)calloc(classType->interfaceCount, sizeof(char *));
		for (i=0;i<classType->interfaceCount;++i) {
			u16 index = classType->interfaceList[i];
			constant = classType->constPool[index];
			constNameAndTypeInfo = (ConstNameAndTypeInfo *)constant.value;
			constant = classType->constPool[constNameAndTypeInfo->nameIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->interfaceNames[i] = constUtf8Info->bytes;
		}
	} else {
		class->interfaceCount = 0;
		class->interfaceNames = NULL;
	}

	if (classType->fieldCount>0) {
		class->fieldCount = classType->fieldCount;
		class->fieldList = (Field *)calloc(classType->fieldCount, sizeof(Field));
		for (i=0;i<classType->fieldCount;++i) {
			FieldInfo fieldInfo = classType->fieldList[i];
			class->fieldList[i].class = class;
			class->fieldList[i].accessFlags = fieldInfo.accessFlags;

			constant = classType->constPool[fieldInfo.nameIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->fieldList[i].fieldName = constUtf8Info->bytes;

			constant = classType->constPool[fieldInfo.descriptorIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->fieldList[i].descriptor = constUtf8Info->bytes;

			if (fieldInfo.attributeCount>0) {
				class->fieldList[i].attributeCount = fieldInfo.attributeCount;
				class->fieldList[i].attributeList = (Attribute *)calloc(fieldInfo.attributeCount, sizeof(Attribute));
				for (j=0;j<fieldInfo.attributeCount;++j) {
					AttributeInfo attributeInfo = fieldInfo.attributeList[j];
					class->fieldList[i].attributeList[j].attributeLength = attributeInfo.attrLength;

					constant = classType->constPool[attributeInfo.nameIndex];
					constUtf8Info = (ConstUtf8Info *)constant.value;
					class->fieldList[i].attributeList[j].attributeName = constUtf8Info->bytes;
					class->fieldList[i].attributeList[j].data = attributeInfo.data;
				}
			}
		}
	} else {
		class->fieldCount = 0;
		class->fieldList = NULL;
	}

	if (classType->methodCount>0) {
		class->methodCount = classType->methodCount;
		class->methodList = (Method *)calloc(classType->methodCount, sizeof(Method));
		for (i=0;i<classType->methodCount;++i) {
			MethodInfo methodInfo = classType->methodList[i];
			class->methodList[i].class = class;
			class->methodList[i].accessFlags = methodInfo.accessFlags;

			constant = classType->constPool[methodInfo.nameIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->methodList[i].methodName = constUtf8Info->bytes;

			constant = classType->constPool[methodInfo.descriptorIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->methodList[i].descriptor = constUtf8Info->bytes;

			if (methodInfo.attributeCount>0) {
				class->methodList[i].attributeCount = methodInfo.attributeCount;
				class->methodList[i].attributeList = (Attribute *)calloc(methodInfo.attributeCount, sizeof(Attribute));
				for (j=0;j<methodInfo.attributeCount;++j) {
					AttributeInfo attributeInfo = methodInfo.attributeList[j];
					class->methodList[i].attributeList[j].attributeLength = attributeInfo.attrLength;

					constant = classType->constPool[attributeInfo.nameIndex];
					constUtf8Info = (ConstUtf8Info *)constant.value;
					class->methodList[i].attributeList[j].attributeName = constUtf8Info->bytes;
					class->methodList[i].attributeList[j].data = attributeInfo.data;

					if (strcmp(class->methodList[i].attributeList[j].attributeName, "Code")==0) {
						byte *codeBase = class->methodList[i].attributeList[j].data;

						u16 maxStackSize = *(u16 *)codeBase;
						maxStackSize = swapU16(maxStackSize);
						class->methodList[i].maxStackSize = maxStackSize;
						codeBase += sizeof(u16);

						u16 maxLocalSize = *(u16 *)codeBase;
						maxLocalSize = swapU16(maxLocalSize);
						class->methodList[i].maxLocalSize = maxLocalSize;
						codeBase += sizeof(u16);

						u32 codeLength = *(u32 *)codeBase;
						codeLength = swapU32(codeLength);
						class->methodList[i].codeLength = codeLength;
						codeBase += sizeof(u32);

						class->methodList[i].codeData = (byte *)calloc(1, codeLength);
						memcpy(class->methodList[i].codeData, codeBase, codeLength);
						codeBase += codeLength;

						u16 exceptionTableLength = *(u16 *)codeBase;
						exceptionTableLength = swapU16(exceptionTableLength);
						class->methodList[i].exceptionTableLength = exceptionTableLength;
						codeBase += sizeof(u16);
						if (exceptionTableLength>0) {
							class->methodList[i].exceptionList = calloc(exceptionTableLength, sizeof(Exception));
							for (k=0; k<exceptionTableLength; ++k) {
								u16 startPc = *(u16 *)codeBase;
								startPc = swapU16(startPc);
								class->methodList[i].exceptionList[k].startPc = startPc;
								codeBase += sizeof(u16);

								u16 endPc = *(u16 *)codeBase;
								endPc = swapU16(endPc);
								class->methodList[i].exceptionList[k].endPc = endPc;
								codeBase += sizeof(u16);

								u16 handlerPc = *(u16 *)codeBase;
								handlerPc = swapU16(handlerPc);
								class->methodList[i].exceptionList[k].handlerPc = handlerPc;
								codeBase += sizeof(u16);

								u16 catchType = *(u16 *)codeBase;
								catchType = swapU16(catchType);
								class->methodList[i].exceptionList[k].catchType = catchType;
								codeBase += sizeof(u16);
							}
						}
					}
				}
			} else {
				class->methodList[i].attributeCount = 0;
				class->methodList[i].attributeList = NULL;
			}
		}
	} else {
		class->methodCount = 0;
		class->methodList = NULL;
	}

	if (classType->attributeCount>0) {
		class->attributeCount = classType->attributeCount;
		class->attributeList = (Attribute *)calloc(classType->attributeCount, sizeof(Attribute));
		for (i=0;i<classType->attributeCount;++i) {
			AttributeInfo attributeInfo = classType->attributeList[i];
			class->attributeList[i].attributeLength = attributeInfo.attrLength;

			constant = classType->constPool[attributeInfo.nameIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->attributeList[i].attributeName = constUtf8Info->bytes;

			class->attributeList[i].data = attributeInfo.data;
		}
	} else {
		class->attributeCount = 0;
		class->attributeList = NULL;
	}
}

static bool isPublic(Class *this) {
	return (this->accessFlags & ACC_PUBLIC)>0;
}

static bool isFinal(Class *this) {
	return (this->accessFlags & ACC_FINAL)>0;
}

static bool isSuper(Class *this) {
	return (this->accessFlags & ACC_SUPER)>0;
}

static bool isInterface(Class *this) {
	return (this->accessFlags & ACC_INTERFACE)>0;
}

static bool isAbstract(Class *this) {
	return (this->accessFlags & ACC_ABSTRACT)>0;
}

static bool isSynthetic(Class *this) {
	return (this->accessFlags & ACC_SYNTHETIC)>0;
}

static bool isAnnotation(Class *this) {
	return (this->accessFlags & ACC_ANNOTATION)>0;
}

static bool isEnum(Class *this) {
	return (this->accessFlags & ACC_ENUM)>0;
}

static ConstPool* getConstant(Class *this, u32 index) {
	if (this!=NULL) {
		return &(this->constPool[index]);
	}
}

static Field* findField(Class *this, char *fieldName) {
	if (this!=NULL && fieldName!=NULL) {
		int i = 0;
		for (i=0;i<this->fieldCount;++i) {
			Field field = this->fieldList[i];
			if (strcmp(fieldName, field.fieldName)==0) {
				return &(this->fieldList[i]);
			}
		}
	}
	return NULL;
}

static Method* findMethod(Class *this, char *methodName, char *descriptor) {
	if (this!=NULL && methodName!=NULL && descriptor!=NULL) {
		int i = 0;
		for (i=0;i<this->methodCount;++i) {
			Method method = this->methodList[i];
			if (strcmp(methodName, method.methodName)==0) {
				return &(this->methodList[i]);
			}
		}
	}
	return NULL;
}

static Method* findMainMethod(Class *this) {
	
	char *mainMethodName = "main";
	char *mainMethodDecs = "([Ljava/lang/String;)V";

	Method *mainMethod = this->findMethod(this, mainMethodName, mainMethodDecs);
	if ((mainMethod->accessFlags & ACC_PUBLIC) && (mainMethod->accessFlags & ACC_STATIC)) {
		return mainMethod;
	}
	return NULL;
}