typedef struct Instance {

    Class *class;

    u32 fieldDataCount;

    SlotData *fieldDataList;

} Instance;

Instance *createInstance(Class *class);

int getInteger(Instance *instance, u32 slotId);

void setInteger(Instance *instance, u32 slotId, int intValue);

long getLong(Instance *instance, u32 slotId);

void setLong(Instance *instance, u32 slotId, long longValue);

float getFloat(Instance *instance, u32 slotId);

void setFloat(Instance *instance, u32 slotId, float floatValue);

double getDouble(Instance *instance, u32 slotId);

void setDouble(Instance *instance, u32 slotId, double doubleValue);

void* getReference(Instance *instance, u32 slotId);

void setReference(Instance *instance, u32 slotId, void* reference);