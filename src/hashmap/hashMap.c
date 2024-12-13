#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "../common/constants.h"
#include "../common/commonType.h"
#include "../datatype/dataType.h"
#include "hashMap.h"

static u32 put(HashMap *this, Object *key, Object *value);

static Object* get(HashMap *this, Object *key);

static u32 getCount(HashMap *this);

static u32 reHash(HashMap *this);

static void printHashMap(HashMap *this);

static u32 removeByKey(HashMap *this, Object *key);

static bool containsKey(HashMap *this, Object *key);

static u32 putInner(HashMap *this, Object *key, Object *value);

static void reBuildMap(HashMap *this, Entry **table, Entry *entry, u32 capacity);

static Entry* getEntryByKey(HashMap *this, Object *key);

HashMap* createHashMap(HashCode hashCode, EqualFun equalFun, u32 capacity) {
	if (hashCode!=NULL && capacity>0) {
		HashMap *map = (HashMap *)malloc(sizeof(HashMap));
		if (map!=NULL) {
			map->count = 0;
			map->capacity = capacity;
			map->table = calloc(1, sizeof(Entry *)*capacity);
			map->limit = capacity*LIMIT_FACTOR;
			map->printHashMap = printHashMap;
			map->containsKey = containsKey;
			map->remove = removeByKey;
			map->hashCode = hashCode;
			map->equalFun = equalFun;
			map->getCount = getCount;
			map->put = put;
			map->get = get;			
		}
		return map;
	} else {
		return NULL;	
	}
}

static u32 put(HashMap *this, Object *key, Object *value) {
	if (this!=NULL && key!=NULL && value!=NULL) {
		if (this->count >= this->limit) {
			if (reHash(this)==STATUS_SUCCESS) {
				if (putInner(this, key, value)) {
					this->count++;
					return STATUS_SUCCESS;				
				}
			}		
		} else {
			if (putInner(this, key, value)) {
				this->count++;
				return STATUS_SUCCESS;				
			}	
		}		
	}
	return STATUS_FAILURE;
}

static Object* get(HashMap *this, Object *key) {
	if (this!=NULL && key!=NULL) {
		int hashCode = this->hashCode(key);
		u32 index = (hashCode & (this->capacity-1));
		Entry *entry = this->table[index];
		while(entry!=NULL) {
			if (this->equalFun(entry->key, key)) {
				return entry->value;			
			}
			entry = entry->next;		
		}
	}
	return NULL;
}

static u32 getCount(HashMap *this) {
	if (this!=NULL) {
		return this->count;	
	} else {
		return 0;
	}
}

static u32 removeByKey(HashMap *this, Object *key) {
	if (this!=NULL && key!=NULL) {
		int hashCode = this->hashCode(key);
		u32 index = (hashCode & (this->capacity-1));
		Entry *entry = this->table[index];
		if (entry!=NULL) {
			if (this->equalFun(entry->key, key)) {
				this->table[index] = entry->next;
				free(entry);
				this->count--;
				return STATUS_SUCCESS;
			} else {
				Entry *prev = entry;
				entry = entry->next;
				while (entry!=NULL) {
					if (this->equalFun(entry->key, key)) {
						prev->next = entry->next;
						free(entry);
						this->count--;
						return STATUS_SUCCESS; 
					} else {
						prev = entry;
						entry = entry->next;
					}					
				}			
			}
		}		
	}
	return STATUS_FAILURE;
}

static bool containsKey(HashMap *this, Object *key) {
	if (this!=NULL && key!=NULL) {
		int hashCode = this->hashCode(key);
		u32 index = (hashCode & (this->capacity-1));
		Entry *entry = this->table[index];
		while (entry!=NULL) {
			if (this->equalFun(entry->key, key)) {
				return true;			
			}
			entry = entry->next;
		}
	}
	return false;
}

static u32 reHash(HashMap *this) {
	if (this!=NULL) {
		u32 oldCapacity = this->capacity;
		u32 newCapacity = this->capacity*2;
		Entry **oldTable = this->table;
		Entry **newTable = malloc(sizeof(Entry *)*newCapacity);
		memset(newTable, 0, sizeof(Entry *)*newCapacity);
		if (newTable!=NULL) {
			u32 i = 0;
			for (i=0;i<oldCapacity;++i) {
				Entry *entry = oldTable[i];
				Entry *nextEntry = NULL;
				oldTable[i] = NULL;
				while (entry!=NULL) {
					nextEntry = entry->next;		
					reBuildMap(this, newTable, entry, newCapacity);
					entry = nextEntry;
				}		
			}
			this->table = newTable;
			this->capacity = newCapacity;
			this->limit = newCapacity*LIMIT_FACTOR;
			free(oldTable);	
			return STATUS_SUCCESS;			
		}
	}	
	return STATUS_SUCCESS;
}

static u32 putInner(HashMap *this, Object *key, Object *value) {
	Entry *entry = getEntryByKey(this, key);
	if (entry!=NULL) {
		entry->value = value;
	} else {
		int hashCode = this->hashCode(key);
		u32 index = (hashCode & (this->capacity-1));
		Entry *old = this->table[index];
		Entry *entry = (Entry *)malloc(sizeof(Entry));
		entry->key = key;
		entry->value = value;
		entry->next = old;
		this->table[index] = entry;
	}
	return STATUS_SUCCESS;
}

static void reBuildMap(HashMap *this, Entry **table, Entry *entry, u32 capacity) {
	if (this!=NULL && table!=NULL && entry!=NULL && capacity>0) {
		int hashCode = this->hashCode(entry->key);
		u32 index = hashCode & (capacity-1);
		entry->next = table[index];
		table[index] = entry;			
	}
}

static Entry* getEntryByKey(HashMap *this, Object *key) {
	if (this!=NULL && key!=NULL) {
		int hashCode = this->hashCode(key);
		u32 index = (hashCode & (this->capacity-1));
		Entry *entry = this->table[index];
		while(entry!=NULL) {
			if (this->equalFun(entry->key, key)) {
				return entry;			
			}
			entry = entry->next;		
		}
	}
	return NULL;
}

static void printHashMap(HashMap *this) {
	if (this!=NULL) {
		u32 i=0;
		for (i=0;i<this->capacity;++i) {
			Entry *entry = this->table[i];
			while (entry!=NULL) {
				printf("%s", (char *)entry->key);
				entry = entry->next;			
			}
			printf("\n");
		}
	}
}

void destroyHashMap(HashMap* this) {
	if (this!=NULL) {
		int capacity = this->capacity;
		int i=0;
		for (i=0;i<capacity;++i) {
			Entry *entry = this->table[i];			
			Entry *nextEntry = NULL;
			while(entry!=NULL) {
				nextEntry = entry->next;
				free(entry);
				entry = nextEntry;	
			}
			this->table[i] = NULL;	
		}
		free(this->table);
		free(this);			
	}
}
