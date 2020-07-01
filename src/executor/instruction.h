typedef struct StackFrame StackFrame;

typedef bool (*Processor)(StackFrame *frame);

typedef bool (*Fetcher)(ByteReader *reader, StackFrame *frame);

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

typedef struct Instruction {

    u8 operateCode;
	   
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

    Fetcher fetcher;

    u8 reserve;

} Instruction;

Instruction *getInstructionByCode(u8 opercode);