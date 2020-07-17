#######################################
#    Makefile for VMachine		      #
#######################################

#   The general compiling commands    #

CP = cp
CC = gcc
SUDO = sudo
CCFLAGES = -c

TARGET = VMachine

TGT = tgt/*

OBJS = tgt/starter.o tgt/utils.o tgt/vmachine.o tgt/endianSwap.o tgt/hashMap.o tgt/arrayList.o tgt/dataType.o tgt/byteReader.o tgt/class.o tgt/instruction.o tgt/operandStack.o tgt/localVariable.o tgt/stackFrame.o tgt/miniunz.o tgt/unzip.o tgt/ioapi.o tgt/descriptor.o tgt/thread.o tgt/instance.o

.PHONY : build clean

build : clean $(TARGET)

clean :
	 rm -rf $(TARGET) $(TGT)

tgt/miniunz.o : src/minizip/miniunz.c
	$(CC) $(CCFLAGES) $< -o $@ -Wimplicit-function-declaration

tgt/unzip.o : src/minizip/unzip.c src/minizip/unzip.h 
	$(CC) $(CCFLAGES) $< -o $@ -Wincompatible-pointer-types

tgt/ioapi.o : src/minizip/ioapi.c src/minizip/ioapi.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/hashMap.o : src/hashmap/hashMap.c src/hashmap/hashMap.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/descriptor.o : src/executor/descriptor.c src/executor/descriptor.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/thread.o : src/executor/thread.c src/executor/thread.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/arrayList.o : src/arraylist/arrayList.c src/arraylist/arrayList.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/dataType.o : src/datatype/dataType.c src/datatype/dataType.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/endianSwap.o : src/endian/endianSwap.c src/endian/endianSwap.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/class.o : src/resolver/class.c src/resolver/class.h
	$(CC) $(CCFLAGES) $< -o $@		

tgt/byteReader.o : src/executor/byteReader.c src/executor/byteReader.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/instance.o : src/executor/instance.c src/executor/instance.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/instruction.o : src/executor/instruction.c src/executor/instruction.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/operandStack.o : src/executor/operandStack.c src/executor/operandStack.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/localVariable.o : src/executor/localVariable.c src/executor/localVariable.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/stackFrame.o : src/executor/stackFrame.c src/executor/stackFrame.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/vmachine.o : src/runtime/vmachine.c src/runtime/vmachine.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/utils.o : src/utils/utils.c src/utils/utils.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/starter.o : src/starter.c
	$(CC) $(CCFLAGES) $< -o $@

VMachine :tgt/starter.o tgt/utils.o tgt/vmachine.o tgt/endianSwap.o tgt/hashMap.o tgt/arrayList.o tgt/dataType.o tgt/byteReader.o tgt/class.o tgt/instruction.o tgt/operandStack.o tgt/localVariable.o tgt/stackFrame.o tgt/miniunz.o tgt/unzip.o tgt/ioapi.o tgt/descriptor.o tgt/thread.o tgt/instance.o
	$(CC) $(OBJS) -o $@ -lz -lm