typedef struct FieldData {

    Field *field;

    SlotData slotData;

} FieldData;

typedef struct Instance {

    Class *class;

    u32 fieldDataCount;

    FieldData *fieldDataList;

} Instance;

Instance *createInstance(Class *class);