
typedef struct Instance {

    Class *class;

    u32 slotDataCount;

    SlotData *slotDataList;

} Instance;

Instance *createInstance(Class *class);