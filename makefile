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

OBJS = tgt/vmachine.o tgt/resolver.o tgt/endianSwap.o tgt/hashMap.o tgt/arrayList.o tgt/stringType.o tgt/dataType.o

.PHONY : build clean

build : clean $(TARGET)

clean :
	 rm -rf $(TARGET) $(TGT)

tgt/hashMap.o : src/hashmap/hashMap.c src/hashmap/hashMap.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/arrayList.o : src/arraylist/arrayList.c src/arraylist/arrayList.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/stringType.o : src/datatype/stringType.c src/datatype/stringType.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/dataType.o : src/datatype/dataType.c src/datatype/dataType.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/endianSwap.o : src/endian/endianSwap.c src/endian/endianSwap.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/resolver.o : src/resolver/resolver.c src/resolver/resolver.h
	$(CC) $(CCFLAGES) $< -o $@

tgt/vmachine.o : src/runtime/vmachine.c
	$(CC) $(CCFLAGES) $< -o $@

VMachine : tgt/vmachine.o tgt/resolver.o tgt/endianSwap.o tgt/hashMap.o tgt/arrayList.o tgt/stringType.o tgt/dataType.o 
	$(CC) $(OBJS) -o $@









