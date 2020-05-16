#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "../common/constants.h"
#include "../common/commonType.h"
#include "stringType.h"

static void bindFunction(String *this);

static u32 getLength(String *this);

static char* getValue(String *this);

static String* subString(String *this, int start, int end);

static String* catString(String *this, String *string); 

String *createString(char *value) {
	String *string = (String *)malloc(sizeof(String));
	u32 length = strlen(value);
	if (value!=NULL && length>0) {
		bindFunction(string);
		string->length = length;
		string->free = length;
		string->value = malloc(length*2+1);
		memcpy(string->value, value, length);
		string->value[length] = '\0';
	} else {
		string->length = 0;
		string->free = DEFALUT_STRING_SPACE;
		string->value = malloc(DEFALUT_STRING_SPACE);
		memset(string->value, 0, DEFALUT_STRING_SPACE);
	}
	return string;
}

String* createStringWithRange(char *value, int range) {
	String *string = (String *)malloc(sizeof(String));
	u32 length = range;
	if (value!=NULL && length>0) {
		bindFunction(string);
		string->length = length;
		string->free = length;
		string->value = malloc(length*2+1);
		memcpy(string->value, value, length);
		string->value[length] = '\0';
	} else {
		string->length = 0;
		string->free = DEFALUT_STRING_SPACE;
		string->value = malloc(DEFALUT_STRING_SPACE);
		memset(string->value, 0, DEFALUT_STRING_SPACE);
	}
	return string;
}

static void bindFunction(String *this) {
	if (this!=NULL) {
		this->getLength = getLength;
		this->getValue = getValue;
		this->subString = subString;
		this->catString = catString;	
	}
}

static u32 getLength(String *this) {
	if (this!=NULL) {
		return this->length;
	} else {
		return 0;	
	}
}

static char* getValue(String *this) {
	if (this!=NULL) {
		return this->value;
	} else {
		return NULL;	
	}
}

static String* subString(String *this, int start, int end) {
	if (this!=NULL) {
		if (start<=end && start>=0 && end<this->length) {
			String *string = (String *)malloc(sizeof(String));
			bindFunction(string);
			string->length = end-start;
			string->free = string->length;
			string->value = malloc(string->length*2+1);
			memcpy(string->value, this->value+start, string->length);
			string->value[string->length] = '\0';
			return string;
		}
	}
	return NULL;
}

static String* catString(String *this, String *string) {
	if (this!=NULL) {
		if (string!=NULL && string->length>0) {
			if (this->free>=string->length) {
				char *dest = this->value+this->length;
				char *src = string->value;
				u32 length = string->length;
				memcpy(dest, src, length);
				this->free -= string->length;
				this->length += string->length;
				return this;
			} else {
				u32 totalLength = this->length + string->length;
				char *newValue = malloc(totalLength*2+1);
				memcpy(newValue, this->value, this->length);
				memcpy(newValue+this->length, string->value, string->length);
				newValue[totalLength] = '\0';
				free(this->value);
				this->value = newValue;
				this->length = totalLength;
				this->free = totalLength;
				return this;
			}
		}	
	}
	return this;
}
