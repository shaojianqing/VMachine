#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../datatype/dataType.h"
#include "../hashmap/hashMap.h"
#include "../arraylist/arrayList.h"
#include "../resolver/class.h"
#include "../runtime/vmachine.h"
#include "../utils/utils.h"

#include "dataStructure.h"
#include "localVariable.h"
#include "operandStack.h"
#include "byteReader.h"
#include "stackFrame.h"
#include "thread.h"
#include "instance.h"
#include "descriptor.h"
#include "instruction.h"

#define nop 			0x00		
#define aconst_null		0x01	
#define iconst_m1		0x02
#define iconst_0		0x03
#define iconst_1		0x04
#define iconst_2		0x05
#define iconst_3		0x06
#define iconst_4		0x07
#define iconst_5		0x08
#define lconst_0		0x09
#define lconst_1		0x0a
#define fconst_0		0x0b
#define fconst_1		0x0c
#define fconst_2		0x0d
#define dconst_0		0x0e
#define dconst_1		0x0f
#define bipush			0x10
#define sipush			0x11
#define ldc			    0x12
#define ldc_w			0x13
#define ldc2_w			0x14
#define iload			0x15
#define lload			0x16
#define fload			0x17
#define dload			0x18
#define aload			0x19
#define iload_0		    0x1a
#define iload_1		    0x1b
#define iload_2		    0x1c
#define iload_3		    0x1d
#define lload_0		    0x1e
#define lload_1		    0x1f
#define lload_2		    0x20
#define lload_3		    0x21
#define fload_0		    0x22
#define fload_1		    0x23
#define fload_2		    0x24
#define fload_3		    0x25
#define dload_0		    0x26
#define dload_1		    0x27
#define dload_2		    0x28
#define dload_3		    0x29
#define aload_0		    0x2a
#define aload_1		    0x2b
#define aload_2		    0x2c
#define aload_3		    0x2d
#define iaload			0x2e
#define laload			0x2f
#define faload			0x30
#define daload			0x31
#define aaload			0x32
#define baload			0x33
#define caload			0x34
#define saload			0x35
#define istore			0x36
#define lstore			0x37
#define fstore			0x38
#define dstore			0x39
#define astore			0x3a
#define istore_0		0x3b
#define istore_1		0x3c
#define istore_2		0x3d
#define istore_3		0x3e
#define lstore_0		0x3f
#define lstore_1		0x40
#define lstore_2		0x41
#define lstore_3		0x42
#define fstore_0		0x43
#define fstore_1		0x44
#define fstore_2		0x45
#define fstore_3		0x46
#define dstore_0		0x47
#define dstore_1		0x48
#define dstore_2		0x49
#define dstore_3		0x4a
#define astore_0		0x4b
#define astore_1		0x4c
#define astore_2		0x4d
#define astore_3		0x4e
#define iastore		    0x4f
#define lastore		    0x50
#define fastore		    0x51
#define dastore		    0x52
#define aastore		    0x53
#define bastore		    0x54
#define castore		    0x55
#define sastore		    0x56
#define pop			    0x57
#define pop2			0x58
#define dup			    0x59
#define dup_x1			0x5a
#define dup_x2			0x5b
#define dup2			0x5c
#define dup2_x1		    0x5d
#define dup2_x2		    0x5e
#define swap			0x5f
#define iadd			0x60
#define ladd			0x61
#define fadd			0x62
#define dadd			0x63
#define isub			0x64
#define lsub			0x65
#define fsub			0x66
#define dsub			0x67
#define imul			0x68
#define lmul			0x69
#define fmul			0x6a
#define dmul			0x6b
#define idiv			0x6c
#define ldiv			0x6d
#define fdiv			0x6e
#define ddiv			0x6f
#define irem			0x70
#define lrem			0x71
#define frem			0x72
#define drem			0x73
#define ineg			0x74
#define lneg			0x75
#define fneg			0x76
#define dneg			0x77
#define ishl			0x78
#define lshl			0x79
#define ishr			0x7a
#define lshr			0x7b
#define iushr			0x7c
#define lushr			0x7d
#define iand			0x7e
#define land			0x7f
#define ior			    0x80
#define lor			    0x81
#define ixor			0x82
#define lxor			0x83
#define iinc			0x84
#define i2l			    0x85
#define i2f			    0x86
#define i2d			    0x87
#define l2i			    0x88
#define l2f			    0x89
#define l2d			    0x8a
#define f2i			    0x8b
#define f2l			    0x8c
#define f2d			    0x8d
#define d2i			    0x8e
#define d2l			    0x8f
#define d2f			    0x90
#define i2b			    0x91
#define i2c			    0x92
#define i2s			    0x93
#define lcmp			0x94
#define fcmpl			0x95
#define fcmpg			0x96
#define dcmpl			0x97
#define dcmpg			0x98
#define ifeq			0x99
#define ifne			0x9a
#define iflt			0x9b
#define ifge			0x9c
#define ifgt			0x9d
#define ifle			0x9e
#define if_icmpeq		0x9f
#define if_icmpne		0xa0
#define if_icmplt		0xa1
#define if_icmpge		0xa2
#define if_icmpgt		0xa3
#define if_icmple		0xa4
#define if_acmpeq		0xa5
#define if_acmpne		0xa6
#define goto			0xa7
#define jsr			    0xa8
#define ret			    0xa9
#define tableswitch	    0xaa
#define lookupswitch	0xab
#define ireturn		    0xac
#define lreturn		    0xad
#define freturn		    0xae
#define dreturn		    0xaf
#define areturn		    0xb0
#define nreturn		    0xb1
#define getstatic		0xb2
#define putstatic		0xb3
#define getfield		0xb4
#define putfield		0xb5
#define invokevirtual	0xb6
#define invokespecial	0xb7
#define invokestatic	0xb8
#define invokeinterface 0xb9
#define invokedynamic	0xba
#define new			    0xbb
#define newarray		0xbc
#define anewarray		0xbd
#define arraylength 	0xbe
#define athrow			0xbf
#define checkcast		0xc0
#define instanceof		0xc1
#define monitorenter	0xc2
#define monitorexit	    0xc3
#define wide			0xc4
#define multianewarray	0xc5
#define ifnull			0xc6
#define ifnonnull		0xc7
#define goto_w			0xc8
#define jsr_w			0xc9
#define breakpoint		0xca
#define opcode_max 		0xcb
#define impdep1 		0xfe
#define impdep2			0xff

static bool nopProcessor(Instruction *instruction, StackFrame *frame);

static bool aconstNullProcessor(Instruction *instruction, StackFrame *frame);

static bool iconstM1Processor(Instruction *instruction, StackFrame *frame);

static bool iconst0Processor(Instruction *instruction, StackFrame *frame);

static bool iconst1Processor(Instruction *instruction, StackFrame *frame);

static bool iconst2Processor(Instruction *instruction, StackFrame *frame);

static bool iconst3Processor(Instruction *instruction, StackFrame *frame);

static bool iconst4Processor(Instruction *instruction, StackFrame *frame);

static bool iconst5Processor(Instruction *instruction, StackFrame *frame);

static bool lconst0Processor(Instruction *instruction, StackFrame *frame);

static bool lconst1Processor(Instruction *instruction, StackFrame *frame);

static bool fconst0Processor(Instruction *instruction, StackFrame *frame);

static bool fconst1Processor(Instruction *instruction, StackFrame *frame);

static bool fconst2Processor(Instruction *instruction, StackFrame *frame);

static bool dconst0Processor(Instruction *instruction, StackFrame *frame);

static bool dconst1Processor(Instruction *instruction, StackFrame *frame);

static bool bipushProcessor(Instruction *instruction, StackFrame *frame);

static bool sipushProcessor(Instruction *instruction, StackFrame *frame);

static bool ldcProcessor(Instruction *instruction, StackFrame *frame);

static bool ldcWProcessor(Instruction *instruction, StackFrame *frame);

static bool ldc2WProcessor(Instruction *instruction, StackFrame *frame);

static bool iloadProcessor(Instruction *instruction, StackFrame *frame);

static bool lloadProcessor(Instruction *instruction, StackFrame *frame);

static bool floadProcessor(Instruction *instruction, StackFrame *frame);

static bool dloadProcessor(Instruction *instruction, StackFrame *frame);

static bool aloadProcessor(Instruction *instruction, StackFrame *frame);

static bool iload0Processor(Instruction *instruction, StackFrame *frame);

static bool iload1Processor(Instruction *instruction, StackFrame *frame);

static bool iload2Processor(Instruction *instruction, StackFrame *frame);

static bool iload3Processor(Instruction *instruction, StackFrame *frame);

static bool lload0Prcoessor(Instruction *instruction, StackFrame *frame);

static bool lload1Processor(Instruction *instruction, StackFrame *frame);

static bool lload2Processor(Instruction *instruction, StackFrame *frame);

static bool lload3Processor(Instruction *instruction, StackFrame *frame);

static bool fload0Processor(Instruction *instruction, StackFrame *frame);

static bool fload1Processor(Instruction *instruction, StackFrame *frame);

static bool fload2Processor(Instruction *instruction, StackFrame *frame);

static bool fload3Processor(Instruction *instruction, StackFrame *frame);

static bool dload0Processor(Instruction *instruction, StackFrame *frame);

static bool dload1Processor(Instruction *instruction, StackFrame *frame);

static bool dload2Processor(Instruction *instruction, StackFrame *frame);

static bool dload3Processor(Instruction *instruction, StackFrame *frame);

static bool aload0Processor(Instruction *instruction, StackFrame *frame);

static bool aload1Processor(Instruction *instruction, StackFrame *frame);

static bool aload2Processor(Instruction *instruction, StackFrame *frame);

static bool aload3Processor(Instruction *instruction, StackFrame *frame);

static bool ialoadProcessor(Instruction *instruction, StackFrame *frame);

static bool laloadProcessor(Instruction *instruction, StackFrame *frame);

static bool faloadProcessor(Instruction *instruction, StackFrame *frame);

static bool daloadProcessor(Instruction *instruction, StackFrame *frame);

static bool aaloadProcessor(Instruction *instruction, StackFrame *frame);

static bool baloadProcessor(Instruction *instruction, StackFrame *frame);

static bool caloadProcessor(Instruction *instruction, StackFrame *frame);

static bool saloadProcessor(Instruction *instruction, StackFrame *frame);

static bool istoreProcessor(Instruction *instruction, StackFrame *frame);

static bool lstoredProcessor(Instruction *instruction, StackFrame *frame);

static bool fstoreProcessor(Instruction *instruction, StackFrame *frame);

static bool dstoreProcessor(Instruction *instruction, StackFrame *frame);

static bool astoreProcessor(Instruction *instruction, StackFrame *frame);

static bool istore0Processor(Instruction *instruction, StackFrame *frame);

static bool istore1Processor(Instruction *instruction, StackFrame *frame);

static bool istore2Processor(Instruction *instruction, StackFrame *frame);

static bool istore3Processor(Instruction *instruction, StackFrame *frame);

static bool lstore0Processor(Instruction *instruction, StackFrame *frame);

static bool lstore1Processor(Instruction *instruction, StackFrame *frame);

static bool lstore2Processor(Instruction *instruction, StackFrame *frame);

static bool lstore3Processor(Instruction *instruction, StackFrame *frame);

static bool fstore0Processor(Instruction *instruction, StackFrame *frame);

static bool fstore1Processor(Instruction *instruction, StackFrame *frame);

static bool fstore2Processor(Instruction *instruction, StackFrame *frame);

static bool fstore3Processor(Instruction *instruction, StackFrame *frame);

static bool dstore0Processor(Instruction *instruction, StackFrame *frame);

static bool dstore1Processor(Instruction *instruction, StackFrame *frame);

static bool dstore2Processor(Instruction *instruction, StackFrame *frame);

static bool dstore3Processor(Instruction *instruction, StackFrame *frame);

static bool astore0Processor(Instruction *instruction, StackFrame *frame);

static bool astore1Processor(Instruction *instruction, StackFrame *frame);

static bool astore2Processor(Instruction *instruction, StackFrame *frame);

static bool astore3Processor(Instruction *instruction, StackFrame *frame);

static bool iastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool lastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool fastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool dastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool aastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool bastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool castoreProcessor(Instruction *instruction, StackFrame *frame);

static bool sastoreProcessor(Instruction *instruction, StackFrame *frame);

static bool popProcessor(Instruction *instruction, StackFrame *frame);

static bool pop2Processor(Instruction *instruction, StackFrame *frame);

static bool dupProcessor(Instruction *instruction, StackFrame *frame);

static bool dupX1Processor(Instruction *instruction, StackFrame *frame);

static bool dupX2Processor(Instruction *instruction, StackFrame *frame);

static bool dup2Processor(Instruction *instruction, StackFrame *frame);

static bool dup2X1Processor(Instruction *instruction, StackFrame *frame);

static bool dup2X2Processor(Instruction *instruction, StackFrame *frame);

static bool swapProcessor(Instruction *instruction, StackFrame *frame);

static bool iaddProcessor(Instruction *instruction, StackFrame *frame);

static bool laddProcessor(Instruction *instruction, StackFrame *frame);

static bool faddProcessor(Instruction *instruction, StackFrame *frame);

static bool daddProcessor(Instruction *instruction, StackFrame *frame);

static bool isubProcessor(Instruction *instruction, StackFrame *frame);

static bool lsubProcessor(Instruction *instruction, StackFrame *frame);

static bool fsubProcessor(Instruction *instruction, StackFrame *frame);

static bool dsubProcessor(Instruction *instruction, StackFrame *frame);

static bool imulProcessor(Instruction *instruction, StackFrame *frame);

static bool lmulProcessor(Instruction *instruction, StackFrame *frame);

static bool fmulProcessor(Instruction *instruction, StackFrame *frame);

static bool dmulProcessor(Instruction *instruction, StackFrame *frame);

static bool idivProcessor(Instruction *instruction, StackFrame *frame);

static bool ldivProcessor(Instruction *instruction, StackFrame *frame);

static bool fdivProcessor(Instruction *instruction, StackFrame *frame);

static bool ddivProcessor(Instruction *instruction, StackFrame *frame);

static bool iremProcessor(Instruction *instruction, StackFrame *frame);

static bool lremProcessor(Instruction *instruction, StackFrame *frame);

static bool fremProcessor(Instruction *instruction, StackFrame *frame);

static bool dremProcessor(Instruction *instruction, StackFrame *frame);

static bool inegProcessor(Instruction *instruction, StackFrame *frame);

static bool lnegProcessor(Instruction *instruction, StackFrame *frame);

static bool fnegProcessor(Instruction *instruction, StackFrame *frame);

static bool dnegProcessor(Instruction *instruction, StackFrame *frame);

static bool ishlProcessor(Instruction *instruction, StackFrame *frame);

static bool lshlProcessor(Instruction *instruction, StackFrame *frame);

static bool ishrProcessor(Instruction *instruction, StackFrame *frame);

static bool lshrProcessor(Instruction *instruction, StackFrame *frame);

static bool iushrProcessor(Instruction *instruction, StackFrame *frame);

static bool lushrProcessor(Instruction *instruction, StackFrame *frame);

static bool iandProcessor(Instruction *instruction, StackFrame *frame);

static bool landProcessor(Instruction *instruction, StackFrame *frame);

static bool iorProcessor(Instruction *instruction, StackFrame *frame);

static bool lorProcessor(Instruction *instruction, StackFrame *frame);

static bool ixorProcessor(Instruction *instruction, StackFrame *frame);

static bool lxorProcessor(Instruction *instruction, StackFrame *frame);

static bool iincProcessor(Instruction *instruction, StackFrame *frame);

static bool i2lProcessor(Instruction *instruction, StackFrame *frame);

static bool i2fProcessor(Instruction *instruction, StackFrame *frame);

static bool i2dProcessor(Instruction *instruction, StackFrame *frame);

static bool l2iProcessor(Instruction *instruction, StackFrame *frame);

static bool l2fProcessor(Instruction *instruction, StackFrame *frame);

static bool l2dProcessor(Instruction *instruction, StackFrame *frame);

static bool f2iProcessor(Instruction *instruction, StackFrame *frame);

static bool f2lProcessor(Instruction *instruction, StackFrame *frame);

static bool f2dProcessor(Instruction *instruction, StackFrame *frame);

static bool d2iProcessor(Instruction *instruction, StackFrame *frame);

static bool d2lProcessor(Instruction *instruction, StackFrame *frame);

static bool d2fProcessor(Instruction *instruction, StackFrame *frame);

static bool i2bProcessor(Instruction *instruction, StackFrame *frame);

static bool i2cProcessor(Instruction *instruction, StackFrame *frame);

static bool i2sProcessor(Instruction *instruction, StackFrame *frame);

static bool lcmpProcessor(Instruction *instruction, StackFrame *frame);

static bool fcmplProcessor(Instruction *instruction, StackFrame *frame);

static bool fcmpgProcessor(Instruction *instruction, StackFrame *frame);

static bool dcmplProcessor(Instruction *instruction, StackFrame *frame);

static bool dcmpgProcessor(Instruction *instruction, StackFrame *frame);

static bool ifeqProcessor(Instruction *instruction, StackFrame *frame);

static bool ifneProcessor(Instruction *instruction, StackFrame *frame);

static bool ifltProcessor(Instruction *instruction, StackFrame *frame);

static bool ifgeProcessor(Instruction *instruction, StackFrame *frame);

static bool ifgtProcessor(Instruction *instruction, StackFrame *frame);

static bool ifleProcessor(Instruction *instruction, StackFrame *frame);

static bool ifIcmpeqProcessor(Instruction *instruction, StackFrame *frame);

static bool ifIcmpneProcessor(Instruction *instruction, StackFrame *frame);

static bool ifIcmpltProcessor(Instruction *instruction, StackFrame *frame);

static bool ifIcmpgeProcessor(Instruction *instruction, StackFrame *frame);

static bool ifIcmpgtProcessor(Instruction *instruction, StackFrame *frame);

static bool ifIcmpleProcessor(Instruction *instruction, StackFrame *frame);

static bool ifAcmpeqProcessor(Instruction *instruction, StackFrame *frame);

static bool ifAcmpneProcessor(Instruction *instruction, StackFrame *frame);

static bool gotoProcessor(Instruction *instruction, StackFrame *frame);

static bool jsrProcessor(Instruction *instruction, StackFrame *frame);

static bool retProcessor(Instruction *instruction, StackFrame *frame);

static bool tableswitchProcessor(Instruction *instruction, StackFrame *frame);

static bool lookupswitchProcessor(Instruction *instruction, StackFrame *frame);

static bool ireturnProcessor(Instruction *instruction, StackFrame *frame);

static bool lreturnProcessor(Instruction *instruction, StackFrame *frame);

static bool freturnProcessor(Instruction *instruction, StackFrame *frame);

static bool dreturnProcessor(Instruction *instruction, StackFrame *frame);

static bool areturnProcessor(Instruction *instruction, StackFrame *frame);

static bool returnProcessor(Instruction *instruction, StackFrame *frame);

static bool getStaticProcessor(Instruction *instruction, StackFrame *frame);

static bool putStaticProcessor(Instruction *instruction, StackFrame *frame);

static bool getFieldProcessor(Instruction *instruction, StackFrame *frame);

static bool putFieldProcessor(Instruction *instruction, StackFrame *frame);

static bool invokeVirtualProcessor(Instruction *instruction, StackFrame *frame);

static bool invokeSpecialProcessor(Instruction *instruction, StackFrame *frame);

static bool invokeStaticProcessor(Instruction *instruction, StackFrame *frame);

static bool invokeInterfaceProcessor(Instruction *instruction, StackFrame *frame);

static bool invokeDynamicProcessor(Instruction *instruction, StackFrame *frame);

static bool newProcessor(Instruction *instruction, StackFrame *frame);

static bool newArrayProcessor(Instruction *instruction, StackFrame *frame);

static bool aNewArrayProcessor(Instruction *instruction, StackFrame *frame);

static bool arrayLengthProcessor(Instruction *instruction, StackFrame *frame);

static bool athrowProcessor(Instruction *instruction, StackFrame *frame);

static bool checkCastProcessor(Instruction *instruction, StackFrame *frame);

static bool instanceofProcessor(Instruction *instruction, StackFrame *frame);

static bool monitorEnterProcessor(Instruction *instruction, StackFrame *frame);

static bool monitorExitProcessor(Instruction *instruction, StackFrame *frame);

static bool wideProcessor(Instruction *instruction, StackFrame *frame);

static bool multiaNewArrayProcessor(Instruction *instruction, StackFrame *frame);

static bool ifNullProcessor(Instruction *instruction, StackFrame *frame);

static bool ifNonNullProcessor(Instruction *instruction, StackFrame *frame);

static bool gotoWProcessor(Instruction *instruction, StackFrame *frame);

static bool jsrWProcessor(Instruction *instruction, StackFrame *frame);

static bool NoOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool WideOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool IIncOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool BranchOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool Index8OperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool Index16OperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool BipushOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool SipushOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool GotoWOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool NewArrayOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool LookupSwitchOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool TableSwitchOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool InvokeInterfaceOperandFetcher(Instruction *instruction, ByteReader *byteReader);

static bool MultiANewArrayOperandFetcher(Instruction *instruction, ByteReader *byteReader);

Instruction instructionTable[] = {
    {nop, "nop", {0}, nopProcessor, NoOperandFetcher},
    {aconst_null, "aconst_null", {0}, aconstNullProcessor, NoOperandFetcher},
    {iconst_m1, "iconst_m1", {0}, iconstM1Processor, NoOperandFetcher},
    {iconst_0, "iconst_0", {0}, iconst0Processor, NoOperandFetcher},
    {iconst_1, "iconst_1", {0}, iconst1Processor, NoOperandFetcher},
    {iconst_2, "iconst_2", {0}, iconst2Processor, NoOperandFetcher},
    {iconst_3, "iconst_3", {0}, iconst3Processor, NoOperandFetcher},
    {iconst_4, "iconst_4", {0}, iconst4Processor, NoOperandFetcher},
    {iconst_5, "iconst_5", {0}, iconst5Processor, NoOperandFetcher},
    {lconst_0, "lconst_0", {0}, lconst0Processor, NoOperandFetcher},
    {lconst_1, "lconst_1", {0}, lconst1Processor, NoOperandFetcher},
    {fconst_0, "fconst_0", {0}, fconst0Processor, NoOperandFetcher},
    {fconst_1, "fconst_1", {0}, fconst1Processor, NoOperandFetcher},
    {fconst_2, "fconst_2", {0}, fconst2Processor, NoOperandFetcher},
    {dconst_0, "dconst_0", {0}, dconst0Processor, NoOperandFetcher},
    {dconst_1, "dconst_1", {0}, dconst1Processor, NoOperandFetcher},
    {bipush, "bipush", {0}, bipushProcessor, BipushOperandFetcher},
    {sipush, "sipush", {0}, sipushProcessor, SipushOperandFetcher},
    {ldc, "ldc", {0}, ldcProcessor, Index8OperandFetcher},
    {ldc_w, "ldc_w", {0}, ldcWProcessor, Index16OperandFetcher},
    {ldc2_w, "ldc2_w", {0}, ldc2WProcessor, Index16OperandFetcher},
    {iload, "iload", {0}, iloadProcessor, Index8OperandFetcher},
    {lload, "lload", {0}, lloadProcessor, Index8OperandFetcher},
    {fload, "fload", {0}, floadProcessor, Index8OperandFetcher},
    {dload, "dload", {0}, dloadProcessor, Index8OperandFetcher},
    {aload, "aload", {0}, aloadProcessor, Index8OperandFetcher},
    {iload_0, "iload_0", {0}, iload0Processor, NoOperandFetcher},
    {iload_1, "iload_1", {0}, iload1Processor, NoOperandFetcher},
    {iload_2, "iload_2", {0}, iload2Processor, NoOperandFetcher},
    {iload_3, "iload_3", {0}, iload3Processor, NoOperandFetcher},
    {lload_0, "lload_0", {0}, lload0Prcoessor, NoOperandFetcher},
    {lload_1, "lload_1", {0}, lload1Processor, NoOperandFetcher},
    {lload_2, "lload_2", {0}, lload2Processor, NoOperandFetcher},
    {lload_3, "lload_3", {0}, lload3Processor, NoOperandFetcher},
    {fload_0, "fload_0", {0}, fload0Processor, NoOperandFetcher},
    {fload_1, "fload_1", {0}, fload1Processor, NoOperandFetcher},
    {fload_2, "fload_2", {0}, fload2Processor, NoOperandFetcher},
    {fload_3, "fload_3", {0}, fload3Processor, NoOperandFetcher},
    {dload_0, "dload_0", {0}, dload0Processor, NoOperandFetcher},
    {dload_1, "dload_1", {0}, dload1Processor, NoOperandFetcher},
    {dload_2, "dload_2", {0}, dload2Processor, NoOperandFetcher},
    {dload_3, "dload_3", {0}, dload3Processor, NoOperandFetcher},
    {aload_0, "aload_0", {0}, aload0Processor, NoOperandFetcher},
    {aload_1, "aload_1", {0}, aload1Processor, NoOperandFetcher},
    {aload_2, "aload_2", {0}, aload2Processor, NoOperandFetcher},
    {aload_3, "aload_3", {0}, aload3Processor, NoOperandFetcher},
    {iaload, "iaload", {0}, ialoadProcessor, NoOperandFetcher},
    {laload, "laload", {0}, laloadProcessor, NoOperandFetcher},
    {faload, "faload", {0}, faloadProcessor, NoOperandFetcher},
    {daload, "daload", {0}, daloadProcessor, NoOperandFetcher},
    {aaload, "aaload", {0}, aaloadProcessor, NoOperandFetcher},
    {baload, "baload", {0}, baloadProcessor, NoOperandFetcher},
    {caload, "caload", {0}, caloadProcessor, NoOperandFetcher},
    {saload, "saload", {0}, saloadProcessor, NoOperandFetcher},
    {istore, "istore", {0}, istoreProcessor, Index8OperandFetcher},
    {lstore, "lstore", {0}, lstoredProcessor, Index8OperandFetcher},
    {fstore, "fstore", {0}, fstoreProcessor, Index8OperandFetcher},
    {dstore, "dstore", {0}, dstoreProcessor, Index8OperandFetcher},
    {astore, "astore", {0}, astoreProcessor, Index8OperandFetcher},
    {istore_0, "istore_0", {0}, istore0Processor, NoOperandFetcher},
    {istore_1, "istore_1", {0}, istore1Processor, NoOperandFetcher},
    {istore_2, "istore_2", {0}, istore2Processor, NoOperandFetcher},
    {istore_3, "istore_3", {0}, istore3Processor, NoOperandFetcher},
    {lstore_0, "lstore_0", {0}, lstore0Processor, NoOperandFetcher},
    {lstore_1, "lstore_1", {0}, lstore1Processor, NoOperandFetcher},
    {lstore_2, "lstore_2", {0}, lstore2Processor, NoOperandFetcher},
    {lstore_3, "lstore_3", {0}, lstore3Processor, NoOperandFetcher},
    {fstore_0, "fstore_0", {0}, fstore0Processor, NoOperandFetcher},
    {fstore_1, "fstore_1", {0}, fstore1Processor, NoOperandFetcher},
    {fstore_2, "fstore_2", {0}, fstore2Processor, NoOperandFetcher},
    {fstore_3, "fstore_3", {0}, fstore3Processor, NoOperandFetcher},
    {dstore_0, "dstore_0", {0}, dstore0Processor, NoOperandFetcher},
    {dstore_1, "dstore_1", {0}, dstore1Processor, NoOperandFetcher},
    {dstore_2, "dstore_2", {0}, dstore2Processor, NoOperandFetcher},
    {dstore_3, "dstore_3", {0}, dstore3Processor, NoOperandFetcher},
    {astore_0, "astore_0", {0}, astore0Processor, NoOperandFetcher},
    {astore_1, "astore_1", {0}, astore1Processor, NoOperandFetcher},
    {astore_2, "astore_2", {0}, astore2Processor, NoOperandFetcher},
    {astore_3, "astore_3", {0}, astore3Processor, NoOperandFetcher},
    {iastore, "iastore", {0}, iastoreProcessor, NoOperandFetcher},
    {lastore, "lastore", {0}, lastoreProcessor, NoOperandFetcher},
    {fastore, "fastore", {0}, fastoreProcessor, NoOperandFetcher},
    {dastore, "dastore", {0}, dastoreProcessor, NoOperandFetcher},
    {aastore, "aastore", {0}, aastoreProcessor, NoOperandFetcher},
    {bastore, "bastore", {0}, bastoreProcessor, NoOperandFetcher},
    {castore, "castore", {0}, castoreProcessor, NoOperandFetcher},
    {sastore, "sastore", {0}, sastoreProcessor, NoOperandFetcher},
    {pop, "pop", {0}, popProcessor, NoOperandFetcher},
    {pop2, "pop2", {0}, pop2Processor, NoOperandFetcher},
    {dup, "dup", {0}, dupProcessor, NoOperandFetcher},
    {dup_x1, "dup_x1", {0}, dupX1Processor, NoOperandFetcher},
    {dup_x2, "dup_x2", {0}, dupX2Processor, NoOperandFetcher},
    {dup2, "dup2", {0}, dup2Processor, NoOperandFetcher},
    {dup2_x1, "dup2_x1", {0}, dup2X1Processor, NoOperandFetcher},
    {dup2_x2, "dup2_x2", {0}, dup2X2Processor, NoOperandFetcher},
    {swap, "swap", {0}, swapProcessor, NoOperandFetcher},
    {iadd, "iadd", {0}, iaddProcessor, NoOperandFetcher},
    {ladd, "ladd", {0}, laddProcessor, NoOperandFetcher},
    {fadd, "fadd", {0}, faddProcessor, NoOperandFetcher},
    {dadd, "dadd", {0}, daddProcessor, NoOperandFetcher},
    {isub, "isub", {0}, isubProcessor, NoOperandFetcher},
    {lsub, "lsub", {0}, lsubProcessor, NoOperandFetcher},
    {fsub, "fsub", {0}, fsubProcessor, NoOperandFetcher},
    {dsub, "dsub", {0}, dsubProcessor, NoOperandFetcher},
    {imul, "imul", {0}, imulProcessor, NoOperandFetcher},
    {lmul, "lmul", {0}, lmulProcessor, NoOperandFetcher},
    {fmul, "fmul", {0}, fmulProcessor, NoOperandFetcher},
    {dmul, "dmul", {0}, dmulProcessor, NoOperandFetcher},
    {idiv, "idiv", {0}, idivProcessor, NoOperandFetcher},
    {ldiv, "ldiv", {0}, ldivProcessor, NoOperandFetcher},
    {fdiv, "fdiv", {0}, fdivProcessor, NoOperandFetcher},
    {ddiv, "ddiv", {0}, ddivProcessor, NoOperandFetcher},
    {irem, "irem", {0}, iremProcessor, NoOperandFetcher},
    {lrem, "lrem", {0}, lremProcessor, NoOperandFetcher},
    {frem, "frem", {0}, fremProcessor, NoOperandFetcher},
    {drem, "drem", {0}, dremProcessor, NoOperandFetcher},
    {ineg, "ineg", {0}, inegProcessor, NoOperandFetcher},
    {lneg, "lneg", {0}, lnegProcessor, NoOperandFetcher},
    {fneg, "fneg", {0}, fnegProcessor, NoOperandFetcher},
    {dneg, "dneg", {0}, dnegProcessor, NoOperandFetcher},
    {ishl, "ishl", {0}, ishlProcessor, NoOperandFetcher},
    {lshl, "lshl", {0}, lshlProcessor, NoOperandFetcher},
    {ishr, "ishr", {0}, ishrProcessor, NoOperandFetcher},
    {lshr, "lshr", {0}, lshrProcessor, NoOperandFetcher},
    {iushr, "iushr", {0}, iushrProcessor, NoOperandFetcher},
    {lushr, "lushr", {0}, lushrProcessor, NoOperandFetcher},
    {iand, "iand", {0}, iandProcessor, NoOperandFetcher},
    {land, "land", {0}, landProcessor, NoOperandFetcher},
    {ior, "ior", {0}, iorProcessor, NoOperandFetcher},
    {lor, "lor", {0}, lorProcessor, NoOperandFetcher},
    {ixor, "ixor", {0}, ixorProcessor, NoOperandFetcher},
    {lxor, "lxor", {0}, lxorProcessor, NoOperandFetcher},
    {iinc, "iinc", {0}, iincProcessor, IIncOperandFetcher},
    {i2l, "i2l", {0}, i2lProcessor, NoOperandFetcher},
    {i2f, "i2f", {0}, i2fProcessor, NoOperandFetcher},
    {i2d, "i2d", {0}, i2dProcessor, NoOperandFetcher},
    {l2i, "l2i", {0}, l2iProcessor, NoOperandFetcher},
    {l2f, "l2f", {0}, l2fProcessor, NoOperandFetcher},
    {l2d, "l2d", {0}, l2dProcessor, NoOperandFetcher},
    {f2i, "f2i", {0}, f2iProcessor, NoOperandFetcher},
    {f2l, "f2l", {0}, f2lProcessor, NoOperandFetcher},
    {f2d, "f2d", {0}, f2dProcessor, NoOperandFetcher},
    {d2i, "d2i", {0}, d2iProcessor, NoOperandFetcher},
    {d2l, "d2l", {0}, d2lProcessor, NoOperandFetcher},
    {d2f, "d2f", {0}, d2fProcessor, NoOperandFetcher},
    {i2b, "i2b", {0}, i2bProcessor, NoOperandFetcher},
    {i2c, "i2c", {0}, i2cProcessor, NoOperandFetcher},
    {i2s, "i2s", {0}, i2sProcessor, NoOperandFetcher},
    {lcmp, "lcmp", {0}, lcmpProcessor, NoOperandFetcher},
    {fcmpl, "fcmpl", {0}, fcmplProcessor, NoOperandFetcher},
    {fcmpg, "fcmpg", {0}, fcmpgProcessor, NoOperandFetcher},
    {dcmpl, "dcmpl", {0}, dcmplProcessor, NoOperandFetcher},
    {dcmpg, "dcmpg", {0}, dcmpgProcessor, NoOperandFetcher},
    {ifeq, "ifeq", {0}, ifeqProcessor, BranchOperandFetcher},
    {ifne, "ifne", {0}, ifneProcessor, BranchOperandFetcher},
    {iflt, "iflt", {0}, ifltProcessor, BranchOperandFetcher},
    {ifge, "ifge", {0}, ifgeProcessor, BranchOperandFetcher},
    {ifgt, "ifgt", {0}, ifgtProcessor, BranchOperandFetcher},
    {ifle, "ifle", {0}, ifleProcessor, BranchOperandFetcher},
    {if_icmpeq, "if_icmpeq", {0}, ifIcmpeqProcessor, BranchOperandFetcher},
    {if_icmpne, "if_icmpne", {0}, ifIcmpneProcessor, BranchOperandFetcher},
    {if_icmplt, "if_icmplt", {0}, ifIcmpltProcessor, BranchOperandFetcher},
    {if_icmpge, "if_icmpge", {0}, ifIcmpgeProcessor, BranchOperandFetcher},
    {if_icmpgt, "if_icmpgt", {0}, ifIcmpgtProcessor, BranchOperandFetcher},
    {if_icmple, "if_icmple", {0}, ifIcmpleProcessor, BranchOperandFetcher},
    {if_acmpeq, "if_acmpeq", {0}, ifAcmpeqProcessor, BranchOperandFetcher},
    {if_acmpne, "if_acmpne", {0}, ifAcmpneProcessor, BranchOperandFetcher},
    {goto, "goto", {0}, gotoProcessor, BranchOperandFetcher},
    {jsr, "jsr", {0}, jsrProcessor, BranchOperandFetcher},
    {ret, "ret", {0}, retProcessor, BranchOperandFetcher},
    {tableswitch, "tableswitch", {0}, tableswitchProcessor, TableSwitchOperandFetcher},
    {lookupswitch, "lookupswitch", {0}, lookupswitchProcessor, LookupSwitchOperandFetcher},
    {ireturn, "ireturn", {0}, ireturnProcessor, NoOperandFetcher},
    {lreturn, "lreturn", {0}, lreturnProcessor, NoOperandFetcher},
    {freturn, "freturn", {0}, freturnProcessor, NoOperandFetcher},
    {dreturn, "dreturn", {0}, dreturnProcessor, NoOperandFetcher},
    {areturn, "areturn", {0}, areturnProcessor, NoOperandFetcher},
    {nreturn, "nreturn", {0}, returnProcessor, NoOperandFetcher},
    {getstatic, "getstatic", {0}, getStaticProcessor, Index16OperandFetcher},
    {putstatic, "putstatic", {0}, putStaticProcessor, Index16OperandFetcher},
    {getfield, "getfield", {0}, getFieldProcessor, Index16OperandFetcher},
    {putfield, "putfield", {0}, putFieldProcessor, Index16OperandFetcher},
    {invokevirtual, "invokevirtual", {0}, invokeVirtualProcessor, Index16OperandFetcher},
    {invokespecial, "invokespecial", {0}, invokeSpecialProcessor, Index16OperandFetcher},
    {invokestatic, "invokestatic", {0}, invokeStaticProcessor, Index16OperandFetcher},
    {invokeinterface, "invokeinterface", {0}, invokeInterfaceProcessor, InvokeInterfaceOperandFetcher},
    {invokedynamic, "invokedynamic", {0}, invokeDynamicProcessor, Index16OperandFetcher},
    {new, "new", {0}, newProcessor, Index16OperandFetcher},
    {newarray, "newarray", {0}, newArrayProcessor, NewArrayOperandFetcher},
    {anewarray, "anewarray", {0}, aNewArrayProcessor, Index16OperandFetcher},
    {arraylength, "arraylength", {0}, arrayLengthProcessor, NoOperandFetcher},
    {athrow, "athrow", {0}, athrowProcessor, NoOperandFetcher},
    {checkcast, "checkcast", {0}, checkCastProcessor, Index16OperandFetcher},
    {instanceof, "instanceof", {0}, instanceofProcessor, Index16OperandFetcher},
    {monitorenter, "monitorenter", {0}, monitorEnterProcessor, NoOperandFetcher},
    {monitorexit, "monitorexit", {0}, monitorExitProcessor, NoOperandFetcher},
    {wide, "wide", {0}, wideProcessor, WideOperandFetcher},
    {multianewarray, "multianewarray", {0}, multiaNewArrayProcessor, MultiANewArrayOperandFetcher},
    {ifnull, "ifnull", {0}, ifNullProcessor, BranchOperandFetcher},
    {ifnonnull, "ifnonnull", {0}, ifNonNullProcessor, BranchOperandFetcher},
    {goto_w, "goto_w", {0}, gotoWProcessor, GotoWOperandFetcher},
    {jsr_w, "jsr_w", {0}, jsrWProcessor, BranchOperandFetcher}
};

Instruction *getInstructionByCode(u8 opercode) {
    return &instructionTable[opercode];
}

static bool nopProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool aconstNullProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushReference(operandStack, NULL);
    return true;
}

static bool iconstM1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, -1);
    return true;
}

static bool iconst0Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 0);
    return true;
}

static bool iconst1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 1);
    return true;
}

static bool iconst2Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 2);
    return true;
}

static bool iconst3Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 3);
    return true;
}

static bool iconst4Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 4);
    return true;
}

static bool iconst5Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 5);
    return true;
}

static bool lconst0Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushLong(operandStack, 0);
    return true;
}

static bool lconst1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushLong(operandStack, 1);
    return true;
}

static bool fconst0Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, 0.0);
    return true;
}

static bool fconst1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, 1.0);
    return true;
}

static bool fconst2Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, 2.0);
    return true;
}

static bool dconst0Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushDouble(operandStack, 0.0);
    return true;
}

static bool dconst1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushDouble(operandStack, 1.0);
    return true;
}

static bool bipushProcessor(Instruction *instruction, StackFrame *frame) {
    int value = instruction->operandStore.bipushOperand.value;
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, value);
    return true;
}

static bool sipushProcessor(Instruction *instruction, StackFrame *frame) {
    int value = instruction->operandStore.sipushOperand.value;
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, value);
    return true;
}

static bool ldcProcessor(Instruction *instruction, StackFrame *frame) {

    return true;
}

static bool ldcWProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ldc2WProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    Class *class = frame->method->class;
    u32 index = instruction->operandStore.index16Operand.index;
    ConstPool *constant = class->getConstant(class, index);
    if (constant->type == CONSTANT_Long) {
        ConstLongInfo *constLongInfo = (ConstLongInfo *)constant->value;
        long lowBytes = constLongInfo->lowBytes;
        long highBytes = constLongInfo->highBytes;
        long value = lowBytes + highBytes<<32;
        operandStack->pushLong(operandStack, value);
    } else if (constant->type == CONSTANT_Double) {
        ConstDoubleInfo *constDoubleInfo = (ConstDoubleInfo *)constant->value;
        long lowBytes = constDoubleInfo->lowBytes;
        long highBytes = constDoubleInfo->highBytes;
        double value = lowBytes + highBytes<<32;
        operandStack->pushDouble(operandStack, value);
    }

    return true;
}

static bool iloadProcess(StackFrame *frame, u32 index) {
    LocalVariables *localVariables = frame->localVariables;
    int value = localVariables->getInt(localVariables, index);

    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, value);
    return true;
}

static bool lloadProcess(StackFrame *frame, u32 index) {
    LocalVariables *localVariables = frame->localVariables;
    long value = localVariables->getLong(localVariables, index);

    OperandStack *operandStack = frame->operandStack;
    operandStack->pushLong(operandStack, value);
    return true;
}

static bool floadProcess(StackFrame *frame, u32 index) {
    LocalVariables *localVariables = frame->localVariables;
    float value = localVariables->getFloat(localVariables, index);

    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, value);
    return true;
}

static bool dloadProcess(StackFrame *frame, u32 index) {
    LocalVariables *localVariables = frame->localVariables;
    double value = localVariables->getDouble(localVariables, index);

    OperandStack *operandStack = frame->operandStack;
    operandStack->pushDouble(operandStack, value);
    return true;
}

static bool aloadProcess(StackFrame *frame, u32 index) {
    LocalVariables *localVariables = frame->localVariables;
    void *reference = localVariables->getReference(localVariables, index);

    OperandStack *operandStack = frame->operandStack;
    operandStack->pushReference(operandStack, reference);
    return true;
}

static bool iloadProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return iloadProcess(frame, index8Operand.index);
}

static bool lloadProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return lloadProcess(frame, index8Operand.index);
}

static bool floadProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return floadProcess(frame, index8Operand.index);
}

static bool dloadProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return dloadProcess(frame, index8Operand.index);
}

static bool aloadProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return aloadProcess(frame, index8Operand.index);
}

static bool iload0Processor(Instruction *instruction, StackFrame *frame) {
    return iloadProcess(frame, 0);
}

static bool iload1Processor(Instruction *instruction, StackFrame *frame) {
    return iloadProcess(frame, 1);
}

static bool iload2Processor(Instruction *instruction, StackFrame *frame) {
    return iloadProcess(frame, 2);
}

static bool iload3Processor(Instruction *instruction, StackFrame *frame) {
    return iloadProcess(frame, 3);
}

static bool lload0Prcoessor(Instruction *instruction, StackFrame *frame) {
    return lloadProcess(frame, 0);
}

static bool lload1Processor(Instruction *instruction, StackFrame *frame) {
    return lloadProcess(frame, 1);
}

static bool lload2Processor(Instruction *instruction, StackFrame *frame) {
    return lloadProcess(frame, 2);
}

static bool lload3Processor(Instruction *instruction, StackFrame *frame) {
    return lloadProcess(frame, 3);
}

static bool fload0Processor(Instruction *instruction, StackFrame *frame) {
    return floadProcess(frame, 0);
}

static bool fload1Processor(Instruction *instruction, StackFrame *frame) {
    return floadProcess(frame, 1);
}

static bool fload2Processor(Instruction *instruction, StackFrame *frame) {
    return floadProcess(frame, 2);
}

static bool fload3Processor(Instruction *instruction, StackFrame *frame) {
    return floadProcess(frame, 3);
}

static bool dload0Processor(Instruction *instruction, StackFrame *frame) {
    return dloadProcess(frame, 0);
}

static bool dload1Processor(Instruction *instruction, StackFrame *frame) {
    return dloadProcess(frame, 1);
}

static bool dload2Processor(Instruction *instruction, StackFrame *frame) {
    return dloadProcess(frame, 2);
}

static bool dload3Processor(Instruction *instruction, StackFrame *frame) {
    return dloadProcess(frame, 3);
}

static bool aload0Processor(Instruction *instruction, StackFrame *frame) {
    return aloadProcess(frame, 0);
}

static bool aload1Processor(Instruction *instruction, StackFrame *frame) {
    return aloadProcess(frame, 1);
}

static bool aload2Processor(Instruction *instruction, StackFrame *frame) {
   return aloadProcess(frame, 2);
}

static bool aload3Processor(Instruction *instruction, StackFrame *frame) {
    return aloadProcess(frame, 3);
}

static bool ialoadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool laloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool faloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool daloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool aaloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool baloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool caloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool saloadProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool istoreProcess(StackFrame *frame, u32 index) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);

    if (index == 2) {
        printf("The result is:%d\n", value);
    }

    LocalVariables *localVariables = frame->localVariables;
    localVariables->setInt(localVariables, index, value);
    return true;
}

static bool istoreProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    u32 index = index8Operand.index;
    return istoreProcess(frame, index);
}

static bool lstoreProcess(StackFrame *frame, u32 index) {
    OperandStack *operandStack = frame->operandStack;
    long value = operandStack->popLong(operandStack);

    LocalVariables *localVariables = frame->localVariables;
    localVariables->setLong(localVariables, index, value);
    return true;
}

static bool lstoredProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    u32 index = index8Operand.index;
    return lstoreProcess(frame, index);
}

static bool fstoreProcess(StackFrame *frame, u32 index) {
    OperandStack *operandStack = frame->operandStack;
    float value = operandStack->popFloat(operandStack);

    LocalVariables *localVariables = frame->localVariables;
    localVariables->setFloat(localVariables, index, value);
    return true;
}

static bool fstoreProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    u32 index = index8Operand.index;
    return fstoreProcess(frame, index);
}

static bool dstoreProcess(StackFrame *frame, u32 index) {
    OperandStack *operandStack = frame->operandStack;
    double value = operandStack->popDouble(operandStack);

    LocalVariables *localVariables = frame->localVariables;
    localVariables->setDouble(localVariables, index, value);
    return true;
}

static bool dstoreProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    u32 index = index8Operand.index;
    return dstoreProcess(frame, index);
}

static bool astoreProcess(StackFrame *frame, u32 index) {
    OperandStack *operandStack = frame->operandStack;
    void *reference = operandStack->popReference(operandStack);

    LocalVariables *localVariables = frame->localVariables;
    localVariables->setReference(localVariables, index, reference);
    return true;
}

static bool astoreProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStore operandStore = instruction->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    u32 index = index8Operand.index;
    return astoreProcess(frame, index);
}

static bool istore0Processor(Instruction *instruction, StackFrame *frame) {
    return istoreProcess(frame, 0);
}

static bool istore1Processor(Instruction *instruction, StackFrame *frame) {
    return istoreProcess(frame, 1);
}

static bool istore2Processor(Instruction *instruction, StackFrame *frame) {
    return istoreProcess(frame, 2);
}

static bool istore3Processor(Instruction *instruction, StackFrame *frame) {
    return istoreProcess(frame, 3);
}

static bool lstore0Processor(Instruction *instruction, StackFrame *frame) {
    return lstoreProcess(frame, 0);
}

static bool lstore1Processor(Instruction *instruction, StackFrame *frame) {
    return lstoreProcess(frame, 1);
}

static bool lstore2Processor(Instruction *instruction, StackFrame *frame) {
    return lstoreProcess(frame, 2);
}

static bool lstore3Processor(Instruction *instruction, StackFrame *frame) {
    return lstoreProcess(frame, 3);
}

static bool fstore0Processor(Instruction *instruction, StackFrame *frame) {
    return fstoreProcess(frame, 0);
}

static bool fstore1Processor(Instruction *instruction, StackFrame *frame) {
    return fstoreProcess(frame, 1);
}

static bool fstore2Processor(Instruction *instruction, StackFrame *frame) {
    return fstoreProcess(frame, 2);
}

static bool fstore3Processor(Instruction *instruction, StackFrame *frame) {
    return fstoreProcess(frame, 3);
}

static bool dstore0Processor(Instruction *instruction, StackFrame *frame) {
    return dstoreProcess(frame, 0);
}

static bool dstore1Processor(Instruction *instruction, StackFrame *frame) {
    return dstoreProcess(frame, 1);
}

static bool dstore2Processor(Instruction *instruction, StackFrame *frame) {
    return dstoreProcess(frame, 2);
}

static bool dstore3Processor(Instruction *instruction, StackFrame *frame) {
    return dstoreProcess(frame, 3);
}

static bool astore0Processor(Instruction *instruction, StackFrame *frame) {
    return astoreProcess(frame, 0);
}

static bool astore1Processor(Instruction *instruction, StackFrame *frame) {
    return astoreProcess(frame, 1);
}

static bool astore2Processor(Instruction *instruction, StackFrame *frame) {
    return astoreProcess(frame, 2);
}

static bool astore3Processor(Instruction *instruction, StackFrame *frame) {
    return astoreProcess(frame, 3);
}

static bool iastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool lastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool fastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool dastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool aastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool bastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool castoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool sastoreProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool popProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->popSlotData(operandStack);
    return true;
}

static bool pop2Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->popSlotData(operandStack);
    operandStack->popSlotData(operandStack);
    return true;
}

static bool dupProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData = operandStack->popSlotData(operandStack);
    printf("slotData:%lu\n", slotData->reference);
    operandStack->pushSlotData(operandStack, slotData);
    operandStack->pushSlotData(operandStack, slotData);
    return true;
}

static bool dupX1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    return true;
}

static bool dupX2Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);
    SlotData *slotData3 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData3);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    return true;
}

static bool dup2Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    return true;
}

static bool dup2X1Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);
    SlotData *slotData3 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData3);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    return true;
}

static bool dup2X2Processor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);
    SlotData *slotData3 = operandStack->popSlotData(operandStack);
    SlotData *slotData4 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData4);
    operandStack->pushSlotData(operandStack, slotData3);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    return true;
}

static bool swapProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData2);
    return true;
}

static bool iaddProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 + value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool laddProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 + value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool faddProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 + value2;
    operandStack->pushFloat(operandStack, result);
    return true;
}

static bool daddProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 + value2;
    operandStack->pushDouble(operandStack, result);
    return true;
}

static bool isubProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2= operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 - value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lsubProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 - value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool fsubProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 - value2;
    operandStack->pushFloat(operandStack, result);
    return true;
}

static bool dsubProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 - value2;
    operandStack->pushDouble(operandStack, result);
    return true;
}

static bool imulProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 * value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lmulProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 * value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool fmulProcessor(Instruction *instruction, StackFrame *frame) {
   OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 * value2;
    operandStack->pushFloat(operandStack, result);
    return true;
}

static bool dmulProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 * value2;
    operandStack->pushDouble(operandStack, result);
    return true;
}

static bool idivProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 / value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool ldivProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 / value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool fdivProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 / value2;
    operandStack->pushFloat(operandStack, result);
    return true;
}

static bool ddivProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 / value2;
    operandStack->pushDouble(operandStack, result);
    return true;
}

static bool iremProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 % value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lremProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 % value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool fremProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = (float)fmod((double)value1, (double)value2);
    operandStack->pushFloat(operandStack, result);
    return true;
}

static bool dremProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = fmod(value1, value2);
    operandStack->pushDouble(operandStack, result);
    return true;
}

static bool inegProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);

    operandStack->pushInt(operandStack, -value);
    return true;
}

static bool lnegProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value = operandStack->popLong(operandStack);

    operandStack->pushLong(operandStack, -value);
    return true;
}

static bool fnegProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value = operandStack->popFloat(operandStack);

    operandStack->pushFloat(operandStack, -value);
    return true;
}

static bool dnegProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value = operandStack->popDouble(operandStack);

    operandStack->pushDouble(operandStack, -value);
    return true;
}

static bool ishlProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    u32 num = ((u32)value2) & 0X1F;
    int result = value1 << num;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lshlProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popInt(operandStack);
    long value1 = operandStack->popLong(operandStack);

    u32 num = ((u32)value2) & 0X1F;
    long result = value1 << num;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool ishrProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    u32 num = ((u32)value2) & 0X1F;
    int result = value1 >> num;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lshrProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popInt(operandStack);
    long value1 = operandStack->popLong(operandStack);

    u32 num = ((u32)value2) & 0X1F;
    long result = value1 >> num;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool iushrProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    u32 num = ((u32)value2) & 0X1F;
    int result = (int)(((u32)value1) >> num);
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lushrProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popInt(operandStack);
    long value1 = operandStack->popLong(operandStack);

    u32 num = ((u32)value2) & 0X1F;
    long result = (long)(((u64)value1) >> num);
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool iandProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value1 = operandStack->popInt(operandStack);
    int value2 = operandStack->popInt(operandStack);

    int result = value1 & value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool landProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value1 = operandStack->popLong(operandStack);
    long value2 = operandStack->popLong(operandStack);

    long result = value1 & value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool iorProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value1 = operandStack->popInt(operandStack);
    int value2 = operandStack->popInt(operandStack);

    int result = value1 | value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lorProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value1 = operandStack->popLong(operandStack);
    long value2 = operandStack->popLong(operandStack);

    long result = value1 | value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool ixorProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value1 = operandStack->popInt(operandStack);
    int value2 = operandStack->popInt(operandStack);

    int result = value1 ^ value2;
    operandStack->pushInt(operandStack, result);
    return true;
}

static bool lxorProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value1 = operandStack->popLong(operandStack);
    long value2 = operandStack->popLong(operandStack);

    long result = value1 ^ value2;
    operandStack->pushLong(operandStack, result);
    return true;
}

static bool iincProcessor(Instruction *instruction, StackFrame *frame) {
    u32 index = instruction->operandStore.iincOperand.index;
    int constValue = instruction->operandStore.iincOperand.constValue;

    LocalVariables *localVariables = frame->localVariables;
    int value = localVariables->getInt(localVariables, index);
    value += constValue;
    localVariables->setInt(localVariables, index, value);
    return true;
}

static bool i2lProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int intValue = operandStack->popInt(operandStack);
    long longValue = (long)intValue;
    operandStack->pushLong(operandStack, longValue);
    return true;
}

static bool i2fProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int intValue = operandStack->popInt(operandStack);
    float floatValue = (float)intValue;
    operandStack->pushFloat(operandStack, floatValue);
    return true;
}

static bool i2dProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int intValue = operandStack->popInt(operandStack);
    double doubleValue = (double)intValue;
    operandStack->pushDouble(operandStack, doubleValue);
    return true;
}

static bool l2iProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long longValue = operandStack->popLong(operandStack);
    int intValue = (int)longValue;
    operandStack->pushInt(operandStack, intValue);
    return true;
}

static bool l2fProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long longValue = operandStack->popLong(operandStack);
    float floatValue = (float)longValue;
    operandStack->pushFloat(operandStack, floatValue);
    return true;
}

static bool l2dProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long longValue = operandStack->popLong(operandStack);
    double doubleValue = (double)longValue;
    operandStack->pushDouble(operandStack, doubleValue);
    return true;
}

static bool f2iProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float floatValue = operandStack->popFloat(operandStack);
    int intValue = (int)floatValue;
    operandStack->pushInt(operandStack, intValue);
    return true;
}

static bool f2lProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float floatValue = operandStack->popFloat(operandStack);
    long longValue = (long)floatValue;
    operandStack->pushLong(operandStack, longValue);
    return true;
}

static bool f2dProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float floatValue = operandStack->popFloat(operandStack);
    double doubleValue = (double)floatValue;
    operandStack->pushDouble(operandStack, doubleValue);
    return true;
}

static bool d2iProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double doubleValue = operandStack->popDouble(operandStack);
    int intValue = (int)doubleValue;
    operandStack->pushInt(operandStack, intValue);
    return true;
}

static bool d2lProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double doubleValue = operandStack->popDouble(operandStack);
    long longValue = (long)doubleValue;
    operandStack->pushLong(operandStack, longValue);
    return true;
}

static bool d2fProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double doubleValue = operandStack->popDouble(operandStack);
    float floatValue = (float)doubleValue;
    operandStack->pushFloat(operandStack, floatValue);
    return true;
}

static bool i2bProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int intValue = operandStack->popInt(operandStack);
    byte byteValue = (byte)intValue;
    operandStack->pushInt(operandStack, byteValue);
    return true;
}

static bool i2cProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int intValue = operandStack->popInt(operandStack);
    u16 shortValue = (u16)intValue;
    operandStack->pushInt(operandStack, shortValue);
    return true;
}

static bool i2sProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int intValue = operandStack->popInt(operandStack);
    short shortValue = (short)intValue;
    operandStack->pushInt(operandStack, shortValue);
    return true;
}

static bool lcmpProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    if (value1 > value2) {
        operandStack->pushInt(operandStack, 1);
    } else if (value1 == value2) {
        operandStack->pushInt(operandStack, 0);
    } else {
        operandStack->pushInt(operandStack, -1);
    }
    return true;
}

static bool fcmplProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    if (value1 > value2) {
        operandStack->pushInt(operandStack, 1);
    } else if (isEqualFloatNum(value1, value2)) {
        operandStack->pushInt(operandStack, 0);
    } else if (value1 < value2) {
        operandStack->pushInt(operandStack, -1);
    } else {
        operandStack->pushInt(operandStack, -1);
    }

    return true;
}

static bool fcmpgProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    if (value1 > value2) {
        operandStack->pushInt(operandStack, 1);
    } else if (isEqualFloatNum(value1, value2)) {
        operandStack->pushInt(operandStack, 0);
    } else if (value1 < value2) {
        operandStack->pushInt(operandStack, -1);
    } else {
        operandStack->pushInt(operandStack, 1);
    }

    return true;
}

static bool dcmplProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    if (value1 > value2) {
        operandStack->pushInt(operandStack, 1);
    } else if (isEqualDoubleNum(value1, value2)) {
        operandStack->pushInt(operandStack, 0);
    } else if (value1 < value2) {
        operandStack->pushInt(operandStack, -1);
    } else {
        operandStack->pushInt(operandStack, -1);
    }

    return true;
}

static bool dcmpgProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    if (value1 > value2) {
        operandStack->pushInt(operandStack, 1);
    } else if (isEqualDoubleNum(value1, value2)) {
        operandStack->pushInt(operandStack, 0);
    } else if (value1 < value2) {
        operandStack->pushInt(operandStack, -1);
    } else {
        operandStack->pushInt(operandStack, 1);
    }
    
    return true;
}

static void branchProcess(StackFrame *frame, int offset) {
    ByteReader *byteReader = frame->byteReader;
    u32 nextPC = byteReader->pc + offset;
    byteReader->setPC(byteReader, nextPC);
}

static bool ifeqProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);
    int offset = instruction->operandStore.branchOperand.offset;

    if (value==0) {
        branchProcess(frame, offset);
    }
    return true;
}

static bool ifneProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);
    int offset = instruction->operandStore.branchOperand.offset;

    if (value!=0) {
        branchProcess(frame, offset);
    }
    return true;
}

static bool ifltProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);
    int offset = instruction->operandStore.branchOperand.offset;

    if (value<0) {
        branchProcess(frame, offset);
    }
    return true;
}

static bool ifgeProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);
    int offset = instruction->operandStore.branchOperand.offset;

    if (value>=0) {
        branchProcess(frame, offset);
    }
    return true;
}

static bool ifgtProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);
    int offset = instruction->operandStore.branchOperand.offset;

    if (value>0) {
        branchProcess(frame, offset);
    }
    return true;
}

static bool ifleProcessor(Instruction *instruction, StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);
    int offset = instruction->operandStore.branchOperand.offset;

    if (value<=0) {
        branchProcess(frame, offset);
    }
    return true;
}

static bool ifIcmpeqProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifIcmpneProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifIcmpltProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifIcmpgeProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifIcmpgtProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifIcmpleProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifAcmpeqProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifAcmpneProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool gotoProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool jsrProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool retProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool tableswitchProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool lookupswitchProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ireturnProcessor(Instruction *instruction, StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    StackFrame *invokeFrame = thread->peekStackFrame(thread);

    OperandStack *currentOperandStack = currentFrame->operandStack;
    int value = currentOperandStack->popInt(currentOperandStack);

    OperandStack *invokeOperandStack = invokeFrame->operandStack;
    invokeOperandStack->pushInt(invokeOperandStack, value);

    return true;
}

static bool lreturnProcessor(Instruction *instruction, StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    StackFrame *invokeFrame = thread->peekStackFrame(thread);

    OperandStack *currentOperandStack = currentFrame->operandStack;
    long value = currentOperandStack->popLong(currentOperandStack);

    OperandStack *invokeOperandStack = invokeFrame->operandStack;
    invokeOperandStack->pushLong(invokeOperandStack, value);

    return true;
}

static bool freturnProcessor(Instruction *instruction, StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    StackFrame *invokeFrame = thread->peekStackFrame(thread);

    OperandStack *currentOperandStack = currentFrame->operandStack;
    float value = currentOperandStack->popFloat(currentOperandStack);

    OperandStack *invokeOperandStack = invokeFrame->operandStack;
    invokeOperandStack->pushFloat(invokeOperandStack, value);

    return true;
}

static bool dreturnProcessor(Instruction *instruction, StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    StackFrame *invokeFrame = thread->peekStackFrame(thread);

    OperandStack *currentOperandStack = currentFrame->operandStack;
    double value = currentOperandStack->popDouble(currentOperandStack);

    OperandStack *invokeOperandStack = invokeFrame->operandStack;
    invokeOperandStack->pushDouble(invokeOperandStack, value);

    return true;
}

static bool areturnProcessor(Instruction *instruction, StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    StackFrame *invokeFrame = thread->peekStackFrame(thread);

    OperandStack *currentOperandStack = currentFrame->operandStack;
    void* reference = currentOperandStack->popReference(currentOperandStack);

    OperandStack *invokeOperandStack = invokeFrame->operandStack;
    invokeOperandStack->pushReference(invokeOperandStack, reference);

    return true;
}

static bool returnProcessor(Instruction *instruction, StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    return true;
}

static bool getStaticProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool putStaticProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool getFieldProcessor(Instruction *instruction, StackFrame *frame) {

    ConstPool *constPool;
    ConstUtf8Info* constUtf8Info;
    ConstClassInfo* constClassInfo;
    ConstFieldRefInfo *constFieldRefInfo;
    ConstNameAndTypeInfo* constNameAndTypeInfo;

    Class *class = frame->method->class;
    u16 index = instruction->operandStore.index16Operand.index;
    constPool = class->getConstant(class, index);
    constFieldRefInfo = (ConstFieldRefInfo *)constPool->value;
    u16 classIndex = constFieldRefInfo->classIndex;
    u16 fieldIndex = constFieldRefInfo->nameAndTypeIndex;

    constPool = class->getConstant(class, classIndex);
    constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *className = constUtf8Info->bytes;

    constPool = class->getConstant(class, fieldIndex);
    constNameAndTypeInfo = (ConstNameAndTypeInfo *)constPool->value;

    u16 fieldNameIndex = constNameAndTypeInfo->nameIndex;
    u16 fieldTypeIndex = constNameAndTypeInfo->descriptorIndex;

    constPool = class->getConstant(class, fieldNameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *fieldName = constUtf8Info->bytes;

    constPool = class->getConstant(class, fieldTypeIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *fieldType = constUtf8Info->bytes;

    Class *targetClass = vmachine->findClassByName(vmachine, className);
    Field *targetField = targetClass->findField(targetClass, fieldName);

    u32 slodId = targetField->slotId;
    OperandStack *operandStack = frame->operandStack;
    Instance *instance = (Instance *) operandStack->popReference(operandStack);

    char type = fieldType[0];
    if (type == 'Z' || type == 'B' || type == 'C' || type == 'S' || type == 'I') {
        operandStack->pushInt(operandStack, getInteger(instance, slodId));
    } else if (type == 'F') {
        operandStack->pushFloat(operandStack, getFloat(instance, slodId));
    } else if (type == 'J') {
        operandStack->pushLong(operandStack, getLong(instance, slodId));
    } else if (type == 'D') {
        operandStack->pushDouble(operandStack, getDouble(instance, slodId));
    } else if (type == 'L' || type == '[') {
        operandStack->pushReference(operandStack, getReference(instance, slodId));
    }
    return true;
}

static bool putFieldProcessor(Instruction *instruction, StackFrame *frame) {
    ConstPool *constPool;
    ConstUtf8Info* constUtf8Info;
    ConstClassInfo* constClassInfo;
    ConstFieldRefInfo *constFieldRefInfo;
    ConstNameAndTypeInfo* constNameAndTypeInfo;

    Class *class = frame->method->class;
    u16 index = instruction->operandStore.index16Operand.index;
    constPool = class->getConstant(class, index);
    constFieldRefInfo = (ConstFieldRefInfo *)constPool->value;
    u16 classIndex = constFieldRefInfo->classIndex;
    u16 fieldIndex = constFieldRefInfo->nameAndTypeIndex;

    constPool = class->getConstant(class, classIndex);
    constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *className = constUtf8Info->bytes;

    printf("putFieldProcessor->className:%s\n", className);

    constPool = class->getConstant(class, fieldIndex);
    constNameAndTypeInfo = (ConstNameAndTypeInfo *)constPool->value;

    u16 fieldNameIndex = constNameAndTypeInfo->nameIndex;
    u16 fieldTypeIndex = constNameAndTypeInfo->descriptorIndex;

    constPool = class->getConstant(class, fieldNameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *fieldName = constUtf8Info->bytes;

    printf("putFieldProcessor->fieldName:%s\n", fieldName);

    constPool = class->getConstant(class, fieldTypeIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *fieldType = constUtf8Info->bytes;

    printf("putFieldProcessor->fieldType:%s\n", fieldType);

    Class *targetClass = vmachine->findClassByName(vmachine, className);
    Field *targetField = targetClass->findField(targetClass, fieldName);

    u32 slodId = targetField->slotId;

    printf("putFieldProcessor->targetField->slotId:%d\n", slodId);

    OperandStack *operandStack = frame->operandStack;
    char type = fieldType[0];

    printf("putFieldProcessor->type:%c\n", type);

    if (type == 'Z' || type == 'B' || type == 'C' || type == 'S' || type == 'I') {
        int intValue = operandStack->popInt(operandStack);
        Instance *instance = (Instance *) operandStack->popReference(operandStack);
        setInteger(instance, slodId, intValue);
    } else if (type == 'F') {
        float floatValue = operandStack->popFloat(operandStack);
        Instance *instance = (Instance *) operandStack->popReference(operandStack);
        setFloat(instance, slodId, floatValue);
    } else if (type == 'J') {
        long longValue = operandStack->popLong(operandStack);
        Instance *instance = (Instance *) operandStack->popReference(operandStack);
        setLong(instance, slodId, longValue);
    } else if (type == 'D') {
        double doubleValue = operandStack->popDouble(operandStack);
        Instance *instance = (Instance *) operandStack->popReference(operandStack);
        setDouble(instance, slodId, doubleValue);
    } else if (type == 'L' || type == '[') {
        void* reference = operandStack->popReference(operandStack);
        Instance *instance = (Instance *) operandStack->popReference(operandStack);
        setReference(instance, slodId, reference);
    }
    return true;
}

static bool invokeMethodProcess(Thread *thread, StackFrame *stackFrame, Method *method, bool isStatic) {
    ByteReader *byteReader = createByteReader(method->codeData, method->codeLength, 0);
	StackFrame *newStackFrame = createStackFrame(thread, method, byteReader);
	
    RuntimeStack *runtimeStack = thread->runtimeStack;
    runtimeStack->pushStack(runtimeStack, newStackFrame);

    MethodDescriptor *descriptor = method->methodDescriptor;
    OperandStack *operandStack = stackFrame->operandStack;
    LocalVariables *newLocalVariables = newStackFrame->localVariables;

    //instance method, param includes this pointer
    u32 paramLimit = descriptor->paramTypeCount;
    if (isStatic) {
        //static method, param not includes this pointer
        paramLimit = descriptor->paramTypeCount-1;
    }

    int i = 0;
    for (i = paramLimit; i>=0; --i) {
        SlotData *slotData = operandStack->popSlotData(operandStack);
        newLocalVariables->setSlotData(newLocalVariables, i, slotData);
    }

	while(byteReader->pc < byteReader->length) {
		u32 pc = byteReader->pc;
		u8 operCode = byteReader->readByte(byteReader);
		Instruction *instruction = getInstructionByCode(operCode);
		printf("    PC:%02d %s\n", pc, instruction->name);
		instruction->fetcher(instruction, byteReader);
		instruction->processor(instruction, newStackFrame);
	}
    return true;
}

static bool invokeVirtualProcessor(Instruction *instruction, StackFrame *frame) {

    ConstPool *constPool;
    ConstUtf8Info *constUtf8Info;
    ConstClassInfo *constClassInfo;
    ConstMethodRefInfo *constMethodRefInfo;
    ConstNameAndTypeInfo *constNameAndTypeInfo;

    u16 index = instruction->operandStore.index16Operand.index;
    Class *class = frame->method->class;
    constPool = class->getConstant(class, index);
    constMethodRefInfo = (ConstMethodRefInfo *)constPool->value;
    u16 classIndex = constMethodRefInfo->classIndex;
    u16 methodIndex = constMethodRefInfo->nameAndTypeIndex;

    constPool = class->getConstant(class, classIndex);
    constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *className = constUtf8Info->bytes;

    constPool = class->getConstant(class, methodIndex);
    constNameAndTypeInfo = (ConstNameAndTypeInfo *)constPool->value;

    u16 methodNameIndex = constNameAndTypeInfo->nameIndex;
    u16 methodTypeIndex = constNameAndTypeInfo->descriptorIndex;

    constPool = class->getConstant(class, methodNameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *methodName = constUtf8Info->bytes;

    constPool = class->getConstant(class, methodTypeIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *methodType = constUtf8Info->bytes;
    
    Class *invokeClass = vmachine->findClassByName(vmachine, className);
    Method *invokeMethod = class->findMethod(invokeClass, methodName, methodType);
    Thread *thread = frame->thread;

    invokeMethodProcess(thread, frame, invokeMethod, false);
    return true;
}

static bool invokeSpecialProcessor(Instruction *instruction, StackFrame *frame) {

    ConstPool *constPool;
    ConstUtf8Info *constUtf8Info;
    ConstClassInfo *constClassInfo;
    ConstMethodRefInfo *constMethodRefInfo;
    ConstNameAndTypeInfo *constNameAndTypeInfo;

    u16 index = instruction->operandStore.index16Operand.index;
    Class *class = frame->method->class;
    constPool = class->getConstant(class, index);
    constMethodRefInfo = (ConstMethodRefInfo *)constPool->value;
    u16 classIndex = constMethodRefInfo->classIndex;
    u16 methodIndex = constMethodRefInfo->nameAndTypeIndex;

    constPool = class->getConstant(class, classIndex);
    constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *className = constUtf8Info->bytes;

    constPool = class->getConstant(class, methodIndex);
    constNameAndTypeInfo = (ConstNameAndTypeInfo *)constPool->value;

    u16 methodNameIndex = constNameAndTypeInfo->nameIndex;
    u16 methodTypeIndex = constNameAndTypeInfo->descriptorIndex;

    constPool = class->getConstant(class, methodNameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *methodName = constUtf8Info->bytes;

    constPool = class->getConstant(class, methodTypeIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *methodType = constUtf8Info->bytes;

    printf("invokeSpecialProcessor->methodName:%s\n", methodName);
    printf("invokeSpecialProcessor->methodType:%s\n", methodType);
    
    Class *invokeClass = vmachine->findClassByName(vmachine, className);
    Method *invokeMethod = class->findMethod(invokeClass, methodName, methodType);
    Thread *thread = frame->thread;

    invokeMethodProcess(thread, frame, invokeMethod, false);
    return true;
}

static bool invokeStaticProcessor(Instruction *instruction, StackFrame *frame) {

    ConstPool *constPool;
    ConstUtf8Info *constUtf8Info;
    ConstClassInfo *constClassInfo;
    ConstMethodRefInfo *constMethodRefInfo;
    ConstNameAndTypeInfo *constNameAndTypeInfo;

    u16 index = instruction->operandStore.index16Operand.index;
    Class *class = frame->method->class;
    constPool = class->getConstant(class, index);
    constMethodRefInfo = (ConstMethodRefInfo *)constPool->value;
    u16 classIndex = constMethodRefInfo->classIndex;
    u16 methodIndex = constMethodRefInfo->nameAndTypeIndex;

    constPool = class->getConstant(class, classIndex);
    constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *className = constUtf8Info->bytes;

    constPool = class->getConstant(class, methodIndex);
    constNameAndTypeInfo = (ConstNameAndTypeInfo *)constPool->value;

    u16 methodNameIndex = constNameAndTypeInfo->nameIndex;
    u16 methodTypeIndex = constNameAndTypeInfo->descriptorIndex;

    constPool = class->getConstant(class, methodNameIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *methodName = constUtf8Info->bytes;

    constPool = class->getConstant(class, methodTypeIndex);
    constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *methodType = constUtf8Info->bytes;
    
    Class *invokeClass = vmachine->findClassByName(vmachine, className);
    Method *invokeMethod = class->findMethod(invokeClass, methodName, methodType);
    Thread *thread = frame->thread;

    invokeMethodProcess(thread, frame, invokeMethod, true);
    return true;
}

static bool invokeInterfaceProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool invokeDynamicProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool newProcessor(Instruction *instruction, StackFrame *frame) {
    u16 index =  instruction->operandStore.index16Operand.index;
    Class *class = frame->method->class;
    ConstPool *constPool = class->getConstant(class, index);
    ConstClassInfo *constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    ConstUtf8Info *constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *instClassName = constUtf8Info->bytes;
    Class *instClass = vmachine->findClassByName(vmachine, instClassName);
    Instance *instance = createInstance(instClass);

    printf("instance:%lu\n", instance);

    OperandStack *operandStack = frame->operandStack;
    operandStack->pushReference(operandStack, instance);
    return true;
}

static bool newArrayProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool aNewArrayProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool arrayLengthProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool athrowProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool checkCastProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool instanceofProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool monitorEnterProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool monitorExitProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool wideProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool multiaNewArrayProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifNullProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool ifNonNullProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool gotoWProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool jsrWProcessor(Instruction *instruction, StackFrame *frame) {
    return true;
}

static bool NoOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    return true;
}

static bool WideOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    return true;
}

static bool IIncOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    u32 operand1 = byteReader->readByte(byteReader);
    int operand2 = byteReader->readByte(byteReader);
    instruction->operandStore.iincOperand.index = operand1;
    instruction->operandStore.iincOperand.constValue = operand2;
    return true;
}

static bool BranchOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    short operand = byteReader->readShort(byteReader);
    instruction->operandStore.branchOperand.offset = operand;
    return true;
}

static bool Index8OperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    byte operand = byteReader->readByte(byteReader);
    instruction->operandStore.index8Operand.index = operand;
    return true;
}

static bool Index16OperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    short operand = byteReader->readShort(byteReader);
    instruction->operandStore.index16Operand.index = operand;
    return true;
}

static bool BipushOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    byte operand = byteReader->readByte(byteReader);
    instruction->operandStore.bipushOperand.value = operand;
    return true;
}

static bool SipushOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    short operand = byteReader->readShort(byteReader);
    instruction->operandStore.sipushOperand.value = operand;
    return true;
}

static bool GotoWOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    int operand = byteReader->readInteger(byteReader);
    instruction->operandStore.gotoWOperand.offset = operand;
    return true;
}

static bool NewArrayOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    return true;
}

static bool LookupSwitchOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    return true;
}

static bool TableSwitchOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    return true;
}

static bool InvokeInterfaceOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    u16 operand = byteReader->readShort(byteReader);
    instruction->operandStore.invokeInterfaceOperand.index = operand;
    byteReader->readByte(byteReader);
    byteReader->readByte(byteReader);

    return true;
}

static bool MultiANewArrayOperandFetcher(Instruction *instruction, ByteReader *byteReader) {
    return true;
}