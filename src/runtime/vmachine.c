#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "../common/constants.h"
#include "../common/commonType.h"
#include "../resolver/class.h"
#include "../executor/dataStructure.h"
#include "../executor/localVariable.h"
#include "../executor/operandStack.h"
#include "../executor/byteReader.h"
#include "../executor/stackFrame.h"
#include "../executor/instruction.h"
#include "../datatype/dataType.h"
#include "../minizip/unzip.h"
#include "../hashmap/hashMap.h"
#include "../executor/thread.h"
#include "../utils/utils.h"

#include "vmachine.h"

Class* findClassByName(VMachine *this, char* name);

void startVMachine(VMachine *this);

void initMainEntryClass(VMachine *this, char *classname);

void initClassFromJarFile(HashMap *classPool, char *filename);

void initClassFromRuntimeJarFile(VMachine *this);

void initClassFromUserDefineJarFile(VMachine *this, char *filename);

VMachine* buildVMachine() {
	VMachine *vmachine = (VMachine*)calloc(1, sizeof(VMachine));
	
	vmachine->startVMachine = startVMachine;
	vmachine->findClassByName = findClassByName;
	vmachine->initMainEntryClass = initMainEntryClass;
	vmachine->initClassFromRuntimeJarFile = initClassFromRuntimeJarFile;
	vmachine->initClassFromUserDefineJarFile = initClassFromUserDefineJarFile;

	vmachine->runtimeClassPool = createHashMap(StringHashCode, StringEqualFun, 20000);
	vmachine->userDefineClassPool = createHashMap(StringHashCode, StringEqualFun, 100);

	return vmachine;
}

void startVMachine(VMachine *this) {
	Class *mainEntryClass = this->mainEntryClass;
	Method *mainMethod = mainEntryClass->findMainMethod(mainEntryClass);

	Thread *thread = createThread(this);
	thread->startThread(thread, mainMethod);
}

Class* findClassByName(VMachine *this, char *name) {
	HashMap *classPool = this->runtimeClassPool;
	Class *class = classPool->get(classPool, name);
	if (class==NULL) {
		classPool = this->userDefineClassPool;
		class = classPool->get(classPool, name);
	}
	return class;
}

void initMainEntryClass(VMachine *this, char *classname) {
	Class* mainEntryClass = findClassByName(this, classname);
	this->mainEntryClass = mainEntryClass;
}

void initClassFromRuntimeJarFile(VMachine *this){
	char *javaPath = getenv(JAVA_HOME);
    javaPath = strcat(javaPath, "/jre/lib/rt.jar");
	initClassFromJarFile(this->runtimeClassPool, javaPath);
}

void initClassFromUserDefineJarFile(VMachine *this, char *filename) {
	initClassFromJarFile(this->userDefineClassPool, filename);
}

void initClassFromJarFile(HashMap *classPool, char *filename) {
	if (filename==NULL) {
		return;
	}

	unzFile uzf = unzOpen(filename);
	if (uzf==NULL) {
		return;
	}

	do {
		unz_global_info info;
		int ret = unzGetGlobalInfo(uzf, &info);
		if (ret != UNZ_OK) {
			return;
		}

		uLong num = info.number_entry;
		unzGoToFirstFile(uzf);
		unz_file_info finfo;
		char filename[MAX_FILE_NAME];
		int i = 0;
		for (i = 0; i < num; ++i) {
			memset (filename, 0, MAX_FILE_NAME);
			unzGetCurrentFileInfo(uzf, &finfo, filename, MAX_FILE_NAME, NULL, 0, NULL, 0);
			if (unzOpenCurrentFile(uzf) != UNZ_OK) {
				printf("Failed unzOpenCurrentFile.\n");
			}

            byte *mem = (byte *)calloc (1, finfo.uncompressed_size);
            if (NULL == mem) {
                printf("calloc memory failed.\n");
                break;
            }
            int nread = unzReadCurrentFile(uzf, mem, finfo.uncompressed_size);
            if (nread != finfo.uncompressed_size) {
                printf("Unzip not read complete. FIXME!\n");
            }

			if (isEndWith(filename, ".class")) {
				Class *class = defineClass(mem);
				classPool->put(classPool, class->className, class);
			}

            free (mem);
			unzCloseCurrentFile(uzf);
			unzGoToNextFile(uzf);
		}

		if (i < num) {
            break;
        }

		unzCloseCurrentFile(uzf);
		unzClose (uzf);
		return;
	} while (0);
}