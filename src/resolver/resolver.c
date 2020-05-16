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
#include "resolver.h"

byte classFileBuffer[MAX_FILE_SIZE];

ClassType* loadClassData(char *filename) {
	
	if (filename==NULL) {
		printf("Class Filename Is Blank!!!\n");
		return NULL;	
	}

	ClassType *classType = (ClassType *)malloc(sizeof(ClassType));

	int fd = open(filename, O_RDWR);
	if (fd>=0) {
		long size = read(fd, classFileBuffer, MAX_FILE_SIZE);
		if (size>0) {
			byte* classData = (byte *)malloc(size);
			classData = memcpy(classData, classFileBuffer, size);
			u32* magic = (u32 *)classFileBuffer;
			*magic = swapU32(*magic);
			printf("ClassType:%X\n", *magic);
			printf("Start Resolving Class Type^+^\n");
			resolveClassData(classType, classData);
		}
		int res = close(fd);
	} else {
		printf("Open Class Encount Error code:%d\n", fd);
	}
	return classType;
}

void resolveClassData(ClassType *classType, byte *data) {
	u32 magic = *(u32*)data;
	classType->magic = swapU32(magic);
	data += sizeof(u32);
	u16 minor = *(u16*)data;
	classType->minorVersion = swapU16(minor);
	data += sizeof(u16);
	u16 major = *(u16*)data;
	classType->majorVersion = swapU16(major);
	data += sizeof(u16);
	u16 poolCount = swapU16(*(u16*)data);
	classType->constPoolCount = poolCount;
	data += sizeof(u16);
	
	if (poolCount>0) {
		classType->constPool = (ConstPool *)malloc(sizeof(ConstPool)*poolCount);
		u16 i=1;
		for (i=1;i<poolCount;++i) {
			u8 elementType = *data;
			ConstPool *constPool = (ConstPool *)malloc(sizeof(ConstPool));
			constPool->type = elementType;
			if (elementType == CONSTANT_Utf8) {
				ConstUtf8Info *constUtf8Info = (ConstUtf8Info *)malloc(sizeof(ConstUtf8Info));
				constUtf8Info->tag = elementType;
				data++;
				constUtf8Info->length = swapU16(*(u16*)data);
				data += sizeof(u16);
				constUtf8Info->bytes = (byte*)malloc(constUtf8Info->length);
				memset(constUtf8Info->bytes, 0, constUtf8Info->length);
				u16 k = 0;
				for (k=0;k<constUtf8Info->length;++k) {
					constUtf8Info->bytes[k] = *(u8*)data;
					data++;	
				}
				constPool->value = constUtf8Info; 
			} else if (elementType == CONSTANT_Integer) {
				ConstIntegerInfo *constIntegerInfo = (ConstIntegerInfo *)malloc(sizeof(ConstIntegerInfo));
				constIntegerInfo->tag = elementType;
				data++;
				constIntegerInfo->bytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool->value = constIntegerInfo;
			} else if (elementType == CONSTANT_Float) {
				ConstFloatInfo *constFloatInfo = (ConstFloatInfo *)malloc(sizeof(ConstFloatInfo));
				constFloatInfo->tag = elementType;
				data++;
				constFloatInfo->bytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool->value = constFloatInfo;	
			} else if (elementType == CONSTANT_Long) {
				ConstLongInfo *constLongInfo = (ConstLongInfo *)malloc(sizeof(ConstLongInfo));
				constLongInfo->tag = elementType;
				data++;
				constLongInfo->highBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constLongInfo->lowBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool->value = constLongInfo;
				classType->constPool[i] = (*constPool);
				++i;
				continue;
			} else if (elementType == CONSTANT_Double) {
				ConstDoubleInfo *constDoubleInfo = (ConstDoubleInfo *)malloc(sizeof(ConstDoubleInfo));
				constDoubleInfo->tag = elementType;
				data++;
				constDoubleInfo->highBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constDoubleInfo->lowBytes = swapU32(*(u32*)data);
				data += sizeof(u32);
				constPool->value = constDoubleInfo;
				classType->constPool[i] = (*constPool);
				++i;
				continue;
			} else if (elementType == CONSTANT_Class) {
				ConstClassInfo *constClassInfo = (ConstClassInfo *)malloc(sizeof(ConstClassInfo));
				constClassInfo->tag = elementType;
				data++;
				constClassInfo->nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constClassInfo;
			} else if (elementType == CONSTANT_String) {
				ConstStringInfo *constStringInfo = (ConstStringInfo *)malloc(sizeof(ConstStringInfo));
				constStringInfo->tag = elementType;
				data++;
				constStringInfo->stringIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constStringInfo;
			} else if (elementType == CONSTANT_Fieldref) {
				ConstFieldRefInfo *constFieldRefInfo = (ConstFieldRefInfo *)malloc(sizeof(ConstFieldRefInfo));
				constFieldRefInfo->tag = elementType;
				data++;				
				constFieldRefInfo->classIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constFieldRefInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constFieldRefInfo;
			} else if (elementType == CONSTANT_Methodref) {
				ConstMethodRefInfo *constMethodRefInfo = (ConstMethodRefInfo *)malloc(sizeof(ConstMethodRefInfo));
				constMethodRefInfo->tag = elementType;
				data++;				
				constMethodRefInfo->classIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constMethodRefInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constMethodRefInfo;
			} else if (elementType == CONSTANT_InterfaceMethodref) {
				ConstInterfaceMethodRefInfo *constInterfaceMethodRefInfo = (ConstInterfaceMethodRefInfo *)malloc
				(sizeof(ConstInterfaceMethodRefInfo));
				constInterfaceMethodRefInfo->tag = elementType;
				data++;
				constInterfaceMethodRefInfo->classIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constInterfaceMethodRefInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constInterfaceMethodRefInfo;
			} else if (elementType == CONSTANT_NameAndType) {
				ConstNameAndTypeInfo *constNameAndTypeInfo = (ConstNameAndTypeInfo *)malloc
				(sizeof(ConstNameAndTypeInfo));
				constNameAndTypeInfo->tag = elementType;
				data++;
				constNameAndTypeInfo->nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constNameAndTypeInfo->descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constNameAndTypeInfo;
			} else if (elementType == CONSTANT_MethodHandle) {
				ConstMethodHandleInfo *constMethodHandleInfo = (ConstMethodHandleInfo *)malloc
				(sizeof(ConstMethodHandleInfo));
				constMethodHandleInfo->tag = elementType;
				data++;
				constMethodHandleInfo->referenceKind = *(u8*)data;
				data += sizeof(u8);
				constMethodHandleInfo->referenceIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constMethodHandleInfo;
			} else if (elementType == CONSTANT_MethodType) {
				ConstMethodTypeInfo *constMethodTypeInfo = (ConstMethodTypeInfo *)malloc
				(sizeof(ConstMethodTypeInfo));
				constMethodTypeInfo->tag = elementType;
				data++;
				constMethodTypeInfo->descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constMethodTypeInfo;
			} else if (elementType == CONSTANT_InvokeDynamic) {
				ConstInvokeDynamicInfo *constInvokeDynamicInfo = (ConstInvokeDynamicInfo *)malloc
				(sizeof(ConstInvokeDynamicInfo));
				constInvokeDynamicInfo->tag = elementType;
				data++;
				constInvokeDynamicInfo->bootstrapMethodAttrIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constInvokeDynamicInfo->nameAndTypeIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				constPool->value = constInvokeDynamicInfo;
			}
			classType->constPool[i] = (*constPool);	
		}

		classType->accessFlags = swapU16(*(u16*)data);
		data += sizeof(u16);
		classType->thisClass = swapU16(*(u16*)data);
		data += sizeof(u16);
		classType->superClass = swapU16(*(u16*)data);
		data += sizeof(u16);
		classType->interfaceCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 interfaceCount = classType->interfaceCount;
		if (interfaceCount>0) {
			classType->interfaceList = (u16 *)malloc(sizeof(u16)*interfaceCount);
			for (i=0;i<interfaceCount;++i) {
				classType->interfaceList[i] = swapU16(*(u16*)data);
				data += sizeof(u16);
			}
		}

		classType->fieldCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 fieldCount = classType->fieldCount;
		if (fieldCount>0) {
			classType->fieldList = (FieldInfo *)malloc(sizeof(FieldInfo)*fieldCount);
			for (i=0;i<fieldCount;++i) {
				classType->fieldList[i].accessFlags = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->fieldList[i].nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->fieldList[i].descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->fieldList[i].attributeCount = swapU16(*(u16*)data);
				data += sizeof(u16);
				u16 attributeCount = classType->fieldList[i].attributeCount;
				if (attributeCount>0) {
					classType->fieldList[i].attributeList = (AttributeInfo *)malloc
					(sizeof(AttributeInfo)*attributeCount);
					u32 m=0;
					for (m=0;m<attributeCount;++m) {
						classType->fieldList[i].attributeList[m].nameIndex = swapU16(*(u16*)data);
						data += sizeof(u16);
						classType->fieldList[i].attributeList[m].attrLength = swapU32(*(u32*)data);
						data += sizeof(u32);
						u32 attrLength = classType->fieldList[i].attributeList[m].attrLength;
						u32 n=0;
						for (n=0;n<attrLength;++n) {
							classType->fieldList[i].attributeList[m].data[n] = *(u8*)data;
							data += sizeof(u8);						
						}
					}			
				}
			}
		}

		
		classType->methodCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 methodCount = classType->methodCount;
		if (methodCount>0) {
			classType->methodList = (MethodInfo *)malloc(sizeof(MethodInfo)*methodCount);
			for (i=0;i<methodCount;++i) {
				classType->methodList[i].accessFlags = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->methodList[i].nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->methodList[i].descriptorIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->methodList[i].attributeCount = swapU16(*(u16*)data);
				data += sizeof(u16);
				u16 attributeCount = classType->methodList[i].attributeCount;
				if (attributeCount>0) {
					classType->methodList[i].attributeList = (AttributeInfo *)malloc
					(sizeof(AttributeInfo)*attributeCount);
					u32 m=0;
					for (m=0;m<attributeCount;++m) {
						classType->methodList[i].attributeList[m].nameIndex = swapU16(*(u16*)data);
						data += sizeof(u16);
						classType->methodList[i].attributeList[m].attrLength = swapU32(*(u32*)data);
						data += sizeof(u32);
						u32 attrLength = classType->methodList[i].attributeList[m].attrLength;
						classType->methodList[i].attributeList[m].data = (byte *)malloc(sizeof(byte)*attrLength);
						u32 n=0;
						for (n=0;n<attrLength;++n) {
							classType->methodList[i].attributeList[m].data[n] = *(byte*)data;
							data += sizeof(byte);						
						}
					}		
				}
			}
		}

		classType->attributeCount = swapU16(*(u16*)data);
		data += sizeof(u16);
		u16 attributeCount = classType->attributeCount;
		if (attributeCount>0) {
			classType->attributeList = (AttributeInfo *)malloc(sizeof(AttributeInfo)*attributeCount);
			for (i=0;i<attributeCount;++i) {
				classType->attributeList[i].nameIndex = swapU16(*(u16*)data);
				data += sizeof(u16);
				classType->attributeList[i].attrLength = swapU32(*(u32*)data);
				data += sizeof(u32);
				u32 attrLength = classType->attributeList[i].attrLength;
				if (attrLength>0) {					
					classType->attributeList[i].data = (byte *)malloc(attrLength);
					u32 m=0;
					for (m=0;m<attrLength;m++) {
						classType->attributeList[i].data[m] = *(byte*)data;
						data += sizeof(byte);				
					}		
				}
			}		
		}
	}
}















