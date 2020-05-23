
typedef bool (*Processor)(void *);

typedef struct CasePair {

    int value;

    u32  position;

} CasePair;

typedef struct TableSwitch {

    int caseMin;

    int caseMax;

    u32 defaultPos;

    u32 pairCount;

    CasePair *pairs;

} TableSwitch;

typedef struct LookupSwitch {

    u32 defaultPos;

    u32 pairCount;

    CasePair *pairs;

} LookupSwitch;

typedef struct {

    U1 operateCode;
	   
    char *name;

    union {

        u8 u8operand;

        u16 u16operand;

        u32 u32operand;

        u64 u64operand;

        TableSwitch  tableSwitch;

        LookupSwitch lookupSwitch;

    } operand;
     
    u8 tag;			      

    u16 length;

    Processor processor;

    u8 reserve;

} Instruction;


