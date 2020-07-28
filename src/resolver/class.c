#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../endian/endianSwap.h"
#include "../executor/descriptor.h"

#include "class.h"

byte classFileBuffer[MAX_FILE_SIZE];

static void initFuncations(Class *class);

static ConstPool* getConstant(Class *this, u32 index);

static Field* findField(Class *this, char *fieldName);

static Method* findMethod(Class *this, char *methodName, char *descriptor);

static Method* findMainMethod(Class *this);

static void resolveClassData(Class *class, byte *data);

Class* loadClassFromFile(char *filename) {
	if (filename==NULL) {
		printf("Jar Filename Is Blank!!!\n");
		return NULL;	
	}

	int fd = open(filename, O_RDWR);
	if (fd>=0) {
		long size = read(fd, classFileBuffer, MAX_FILE_SIZE);
		if (size>0) {
			byte* classData = (byte *)calloc(1, size);
			classData = memcpy(classData, classFileBuffer, size);
			Class *class = defineClass(classData);
			free(classData);
			return class;
		}
		int res = close(fd);
	} else {
		printf("Open Class Encount Error code:%d\n", fd);
	}
	return NULL;
}

Class* defineClass(byte* classData) {
	if (classData!=NULL) {
		Class* class = (Class *)calloc(1, sizeof(Class));
		initFuncations(class);
		resolveClassData(class, classData);
		return class;
	}
	return NULL;
}

static void initFuncations(Class *class) {
	class->getConstant = getConstant;
	class->findField = findField;
	class->findMethod = findMethod;
	class->findMainMethod = findMainMethod;
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

static void resolveClassData(Class *class, byte *data) {

	ConstPool constant;
	ConstPool *constPool;
	ConstUtf8Info *constUtf8Info;
	ConstFieldRefInfo *constFieldRefInfo;
	ConstNameAndTypeInfo *constNameAndTypeInfo;

	u32 magic = *(u32*)data;
	class->magic = swapU32(magic);
	data += sizeof(u32);

	u16 minor = *(u16*)data;
	class->minorVersion = swapU16(minor);
	data += sizeof(u16);

	u16 major = *(u16*)data;
	class->majorVersion = swapU16(major);
	data += sizeof(u16);
	
	u16 poolCount = swapU16(*(u16*)data);
	data += sizeof(u16);
	
	if (poolCount>0) {
		constPool = (ConstPool *)calloc(1, sizeof(ConstPool)*poolCount);
		u16 i=1;
		for (i=1;i<poolCount;++i) {
			u8 elementType = *data;
			constPool[i].type = elementType;
			if (elementType == CONSTANT_Utf8) {
				ConstUtf8Info *constUtf8Info = (ConstUtf8Info *)calloc(1, sizeof(ConstUtf8Info));
				constUtf8Info->tag = elementType;
				data++;
				constUtf8Info->length = swapU16(*(u16*)data);
				data += sizeof(u16);
				constUtf8Info->bytes = (byte*)calloc(1, constUtf8Info->length);
				memset(constUtf8Info->bytes, 0, constUtf8Info->length);
				u16 k = 0;
				for (k=0;k<constUtf8Info->length;++k) {
					constUtf8Info->bytes[k] = *(u8*)data;
					data++;	
				}
				constPool[i].value = constUtf8Info; 
			} else if (elementType == CONSTANT_Integer) {
				ConstIntegerInfo *constIntegerInfo = (ConstIntegerInfo *)calloc(1, sizeof(ConstIntegerInfo));
				constIntegerInfo->tag = elementType;
				data++;
				constIntegerInfo->bytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool[i].value = constIntegerInfo;
			} else if (elementType == CONSTANT_Float) {
				ConstFloatInfo *constFloatInfo = (ConstFloatInfo *)calloc(1, sizeof(ConstFloatInfo));
				constFloatInfo->tag = elementType;
				data++;
				constFloatInfo->bytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool[i].value = constFloatInfo;	
			} else if (elementType == CONSTANT_Long) {
				ConstLongInfo *constLongInfo = (ConstLongInfo *)calloc(1, sizeof(ConstLongInfo));
				constLongInfo->tag = elementType;
				data++;
				constLongInfo->highBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constLongInfo->lowBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool[i].value = constLongInfo;
				++i;
				continue;
			} else if (elementType == CONSTANT_Double) {
				ConstDoubleInfo *constDoubleInfo = (ConstDoubleInfo *)calloc(1, sizeof(ConstDoubleInfo));
				constDoubleInfo->tag = elementType;
				data++;
				constDoubleInfo->highBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constDoubleInfo->lowBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool[i].value = constDoubleInfo;
				++i;
				continue;
			} else if (elementType == CONSTANT_Class) {
				ConstClassInfo *constClassInfo = (ConstClassInfo *)calloc(1, sizeof(ConstClassInfo));
				constClassInfo->tag = elementType;
				data++;
				constClassInfo->nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constClassInfo;
			} else if (elementType == CONSTANT_String) {
				ConstStringInfo *constStringInfo = (ConstStringInfo *)calloc(1, sizeof(ConstStringInfo));
				constStringInfo->tag = elementType;
				data++;
				constStringInfo->stringIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constStringInfo;
			} else if (elementType == CONSTANT_Fieldref) {
				ConstFieldRefInfo *constFieldRefInfo = (ConstFieldRefInfo *)calloc(1, sizeof(ConstFieldRefInfo));
				constFieldRefInfo->tag = elementType;
				data++;				
				constFieldRefInfo->classIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constFieldRefInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constFieldRefInfo;
			} else if (elementType == CONSTANT_Methodref) {
				ConstMethodRefInfo *constMethodRefInfo = (ConstMethodRefInfo *)calloc(1, sizeof(ConstMethodRefInfo));
				constMethodRefInfo->tag = elementType;
				data++;				
				constMethodRefInfo->classIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constMethodRefInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constMethodRefInfo;
			} else if (elementType == CONSTANT_InterfaceMethodref) {
				ConstInterfaceMethodRefInfo *constInterfaceMethodRefInfo = (ConstInterfaceMethodRefInfo *)calloc(1, sizeof(ConstInterfaceMethodRefInfo));
				constInterfaceMethodRefInfo->tag = elementType;
				data++;
				constInterfaceMethodRefInfo->classIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constInterfaceMethodRefInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constInterfaceMethodRefInfo;
			} else if (elementType == CONSTANT_NameAndType) {
				ConstNameAndTypeInfo *constNameAndTypeInfo = (ConstNameAndTypeInfo *)calloc(1, sizeof(ConstNameAndTypeInfo));
				constNameAndTypeInfo->tag = elementType;
				data++;
				constNameAndTypeInfo->nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constNameAndTypeInfo->descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constNameAndTypeInfo;
			} else if (elementType == CONSTANT_MethodHandle) {
				ConstMethodHandleInfo *constMethodHandleInfo = (ConstMethodHandleInfo *)calloc(1, sizeof(ConstMethodHandleInfo));
				constMethodHandleInfo->tag = elementType;
				data++;
				constMethodHandleInfo->referenceKind = *(u8*)data;
				data += sizeof(u8);
				constMethodHandleInfo->referenceIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constMethodHandleInfo;
			} else if (elementType == CONSTANT_MethodType) {
				ConstMethodTypeInfo *constMethodTypeInfo = (ConstMethodTypeInfo *)calloc(1, sizeof(ConstMethodTypeInfo));
				constMethodTypeInfo->tag = elementType;
				data++;
				constMethodTypeInfo->descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constMethodTypeInfo;
			} else if (elementType == CONSTANT_InvokeDynamic) {
				ConstInvokeDynamicInfo *constInvokeDynamicInfo = (ConstInvokeDynamicInfo *)calloc(1, sizeof(ConstInvokeDynamicInfo));
				constInvokeDynamicInfo->tag = elementType;
				data++;
				constInvokeDynamicInfo->bootstrapMethodAttrIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constInvokeDynamicInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool[i].value = constInvokeDynamicInfo;
			}	
		}

		class->constPoolCount = poolCount;
		class->constPool = constPool;
		class->accessFlags = swapU16(*(u16*)data);
		data += sizeof(u16);

		u16 thisClass = swapU16(*(u16*)data);
		data += sizeof(u16);

		constant = constPool[thisClass];
		constNameAndTypeInfo = (ConstNameAndTypeInfo *)constant.value;
		constant = constPool[constNameAndTypeInfo->nameIndex];
		constUtf8Info = (ConstUtf8Info *)constant.value;
		class->className = constUtf8Info->bytes;

		u16 superClass = swapU16(*(u16*)data);
		data += sizeof(u16);

		if (superClass>0) {
			constant = constPool[superClass];
			constNameAndTypeInfo = (ConstNameAndTypeInfo *)constant.value;
			constant = constPool[constNameAndTypeInfo->nameIndex];
			constUtf8Info = (ConstUtf8Info *)constant.value;
			class->superClassName = constUtf8Info->bytes;
		} else {
			class->superClassName = "";
		}
		
		class->interfaceCount = swapU16(*(u16*)data);
		data += sizeof(u16);

		u16 interfaceCount = class->interfaceCount;
		if (interfaceCount>0) {
			class->interfaceNames = (char **)calloc(interfaceCount, sizeof(char *));
			for (i=0;i<interfaceCount;++i) {
				u16 index = swapU16(*(u16*)data);
				data += sizeof(u16);

				constant = constPool[index];
				constNameAndTypeInfo = (ConstNameAndTypeInfo *)constant.value;
				constant = constPool[constNameAndTypeInfo->nameIndex];
				constUtf8Info = (ConstUtf8Info *)constant.value;
				class->interfaceNames[i] = constUtf8Info->bytes;
			}
		}

		class->fieldCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 fieldCount = class->fieldCount;
		if (fieldCount>0) {
			class->fieldList = (Field *)calloc(1, sizeof(Field)*fieldCount);
			for (i=0;i<fieldCount;++i) {

				class->fieldList[i].class = class;
				class->fieldList[i].accessFlags = swapU16(*(u16*)data);
				data += sizeof(u16);

				u16 nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constant = constPool[nameIndex];
				constUtf8Info = (ConstUtf8Info *)constant.value;
				class->fieldList[i].fieldName = constUtf8Info->bytes;

				u16 descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constant = constPool[descriptorIndex];
				constUtf8Info = (ConstUtf8Info *)constant.value;
				class->fieldList[i].descriptor = constUtf8Info->bytes;

				class->fieldList[i].attributeCount = swapU16(*(u16*)data);
				data += sizeof(u16);

				u16 attributeCount = class->fieldList[i].attributeCount;
				if (attributeCount>0) {
					class->fieldList[i].attributeList = (Attribute *)calloc(class->fieldList[i].attributeCount, sizeof(Attribute));
					u32 m=0;
					for (m=0;m<attributeCount;++m) {

						u16 attributeNameIndex = swapU16(*(u16*)data);
						data += sizeof(u16);
						constant = constPool[attributeNameIndex];
						constUtf8Info = (ConstUtf8Info *)constant.value;
						class->fieldList[i].attributeList[m].attributeName = constUtf8Info->bytes;

						class->fieldList[i].attributeList[m].attributeLength = swapU32(*(u32*)data);
						data += sizeof(u32);

						u32 attributeLength = class->fieldList[i].attributeList[m].attributeLength;
						class->fieldList[i].attributeList[m].data = (void *)calloc(1, sizeof(byte)*attributeLength);
						u32 n=0;
						for (n=0;n<attributeLength;++n) {
							class->fieldList[i].attributeList[m].data[n] = *(byte*)data;
							data += sizeof(byte);						
						}
					}			
				}
			}
		}

		class->methodCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 methodCount = class->methodCount;
		if (methodCount>0) {
			class->methodList = (Method *)malloc(sizeof(Method)*methodCount);
			for (i=0;i<methodCount;++i) {

				class->methodList[i].class = class;
				class->methodList[i].accessFlags = swapU16(*(u16*)data);
				data += sizeof(u16);

				u16 methodNameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constant = constPool[methodNameIndex];
				constUtf8Info = (ConstUtf8Info *)constant.value;
				class->methodList[i].methodName = constUtf8Info->bytes;

				u16 methodDescriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constant = constPool[methodDescriptorIndex];
				constUtf8Info = (ConstUtf8Info *)constant.value;
				class->methodList[i].descriptor = constUtf8Info->bytes;
				class->methodList[i].methodDescriptor = parseMethodDescriptor(class->methodList[i].descriptor);

				class->methodList[i].attributeCount = swapU16(*(u16*)data);
				data += sizeof(u16);
				u16 attributeCount = class->methodList[i].attributeCount;
				if (attributeCount>0) {
					class->methodList[i].attributeList = (Attribute *)calloc(1, sizeof(Attribute)*attributeCount);
					u32 m=0;
					for (m=0;m<attributeCount;++m) {
						u16 attributeNameIndex = swapU16(*(u16*)data);
						data += sizeof(u16);
						constant = constPool[attributeNameIndex];
						constUtf8Info = (ConstUtf8Info *)constant.value;
						class->methodList[i].attributeList[m].attributeName = constUtf8Info->bytes;

						class->methodList[i].attributeList[m].attributeLength = swapU32(*(u32*)data);
						data += sizeof(u32);

						u32 attributeLength = class->methodList[i].attributeList[m].attributeLength;
						class->methodList[i].attributeList[m].data = (void *)calloc(1, sizeof(byte)*attributeLength);
						u32 n=0;
						for (n=0;n<attributeLength;++n) {
							class->methodList[i].attributeList[m].data[n] = *(byte*)data;
							data += sizeof(byte);						
						}

						if (strcmp(class->methodList[i].attributeList[m].attributeName, "Code")==0) {
							byte *codeBase = class->methodList[i].attributeList[m].data;

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
								u32 k = 0;
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
				}
			}
		}

		class->attributeCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 attributeCount = class->attributeCount;
		if (attributeCount>0) {
			class->attributeList = (Attribute *)calloc(1, sizeof(Attribute)*attributeCount);
			for (i=0;i<attributeCount;++i) {
				
				u16 attributeNameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constant = constPool[attributeNameIndex];
				constUtf8Info = (ConstUtf8Info *)constant.value;
				class->attributeList[i].attributeName = constUtf8Info->bytes;

				class->attributeList[i].attributeLength = swapU32(*(u32*)data);
				data += sizeof(u32);

				u32 attributeLength = class->attributeList[i].attributeLength;
				if (attributeLength>0) {					
					class->attributeList[i].data = (void *)calloc(1, sizeof(byte)*attributeLength);
					u32 m=0;
					for (m=0;m<attributeLength;m++) {
						class->attributeList[i].data[m] = *(byte*)data;
						data += sizeof(byte);				
					}		
				}
			}		
		}
	}
}

bool isPublic(u16 accessFlags) {
	return (accessFlags & ACC_PUBLIC)>0;
}

bool isPrivate(u16 accessFlags) {
	return (accessFlags & ACC_PRIVATE)>0;
}

bool isProtected(u16 accessFlags) {
	return (accessFlags & ACC_PROTECTED)>0;
}

bool isStatic(u16 accessFlags) {
	return (accessFlags & ACC_STATIC)>0;
}

bool isFinal(u16 accessFlags) {
	return (accessFlags & ACC_FINAL)>0;
}

bool isSuper(u16 accessFlags) {
	return (accessFlags & ACC_SUPER)>0;
}

bool isSynchronized(u16 accessFlags) {
	return (accessFlags & ACC_SYNCHRONIZED)>0;
}

bool isVolatile(u16 accessFlags) {
	return (accessFlags & ACC_VOLATILE)>0;
}

bool isBridge(u16 accessFlags) {
	return (accessFlags & ACC_BRIDGE)>0;
}

bool isTranslent(u16 accessFlags) {
	return (accessFlags & ACC_TRANSLENT)>0;
}

bool isVarargs(u16 accessFlags) {
	return (accessFlags & ACC_VARARGS)>0;
}

bool isNative(u16 accessFlags) {
	return (accessFlags & ACC_NATIVE)>0;
}

bool isInterface(u16 accessFlags) {
	return (accessFlags & ACC_INTERFACE)>0;
}

bool isAbstract(u16 accessFlags) {
	return (accessFlags & ACC_ABSTRACT)>0;
}

bool isStrict(u16 accessFlags) {
	return (accessFlags & ACC_STRICT)>0;
}

bool isSynthetic(u16 accessFlags) {
	return (accessFlags & ACC_SYNTHETIC)>0;
}

bool isAnnotation(u16 accessFlags) {
	return (accessFlags & ACC_ANNOTATION)>0;
}

bool isEnum(u16 accessFlags) {
	return (accessFlags & ACC_ENUM)>0;
}