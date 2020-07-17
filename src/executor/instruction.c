#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../common/commonType.h"
#include "../common/constants.h"
#include "../datatype/dataType.h"
#include "../hashmap/hashMap.h"
#include "../resolver/class.h"
#include "../runtime/vmachine.h"

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

static bool nopProcessor(StackFrame *frame);

static bool aconstNullProcessor(StackFrame *frame);

static bool iconstM1Processor(StackFrame *frame);

static bool iconst0Processor(StackFrame *frame);

static bool iconst1Processor(StackFrame *frame);

static bool iconst2Processor(StackFrame *frame);

static bool iconst3Processor(StackFrame *frame);

static bool iconst4Processor(StackFrame *frame);

static bool iconst5Processor(StackFrame *frame);

static bool lconst0Processor(StackFrame *frame);

static bool lconst1Processor(StackFrame *frame);

static bool fconst0Processor(StackFrame *frame);

static bool fconst1Processor(StackFrame *frame);

static bool fconst2Processor(StackFrame *frame);

static bool dconst0Processor(StackFrame *frame);

static bool dconst1Processor(StackFrame *frame);

static bool bipushProcessor(StackFrame *frame);

static bool sipushProcessor(StackFrame *frame);

static bool ldcProcessor(StackFrame *frame);

static bool ldcWProcessor(StackFrame *frame);

static bool ldc2WProcessor(StackFrame *frame);

static bool iloadProcessor(StackFrame *frame);

static bool lloadProcessor(StackFrame *frame);

static bool floadProcessor(StackFrame *frame);

static bool dloadProcessor(StackFrame *frame);

static bool aloadProcessor(StackFrame *frame);

static bool iload0Processor(StackFrame *frame);

static bool iload1Processor(StackFrame *frame);

static bool iload2Processor(StackFrame *frame);

static bool iload3Processor(StackFrame *frame);

static bool lload0Prcoessor(StackFrame *frame);

static bool lload1Processor(StackFrame *frame);

static bool lload2Processor(StackFrame *frame);

static bool lload3Processor(StackFrame *frame);

static bool fload0Processor(StackFrame *frame);

static bool fload1Processor(StackFrame *frame);

static bool fload2Processor(StackFrame *frame);

static bool fload3Processor(StackFrame *frame);

static bool dload0Processor(StackFrame *frame);

static bool dload1Processor(StackFrame *frame);

static bool dload2Processor(StackFrame *frame);

static bool dload3Processor(StackFrame *frame);

static bool aload0Processor(StackFrame *frame);

static bool aload1Processor(StackFrame *frame);

static bool aload2Processor(StackFrame *frame);

static bool aload3Processor(StackFrame *frame);

static bool ialoadProcessor(StackFrame *frame);

static bool laloadProcessor(StackFrame *frame);

static bool faloadProcessor(StackFrame *frame);

static bool daloadProcessor(StackFrame *frame);

static bool aaloadProcessor(StackFrame *frame);

static bool baloadProcessor(StackFrame *frame);

static bool caloadProcessor(StackFrame *frame);

static bool saloadProcessor(StackFrame *frame);

static bool istoreProcessor(StackFrame *frame);

static bool lstoredProcessor(StackFrame *frame);

static bool fstoreProcessor(StackFrame *frame);

static bool dstoreProcessor(StackFrame *frame);

static bool astoreProcessor(StackFrame *frame);

static bool istore0Processor(StackFrame *frame);

static bool istore1Processor(StackFrame *frame);

static bool istore2Processor(StackFrame *frame);

static bool istore3Processor(StackFrame *frame);

static bool lstore0Processor(StackFrame *frame);

static bool lstore1Processor(StackFrame *frame);

static bool lstore2Processor(StackFrame *frame);

static bool lstore3Processor(StackFrame *frame);

static bool fstore0Processor(StackFrame *frame);

static bool fstore1Processor(StackFrame *frame);

static bool fstore2Processor(StackFrame *frame);

static bool fstore3Processor(StackFrame *frame);

static bool dstore0Processor(StackFrame *frame);

static bool dstore1Processor(StackFrame *frame);

static bool dstore2Processor(StackFrame *frame);

static bool dstore3Processor(StackFrame *frame);

static bool astore0Processor(StackFrame *frame);

static bool astore1Processor(StackFrame *frame);

static bool astore2Processor(StackFrame *frame);

static bool astore3Processor(StackFrame *frame);

static bool iastoreProcessor(StackFrame *frame);

static bool lastoreProcessor(StackFrame *frame);

static bool fastoreProcessor(StackFrame *frame);

static bool dastoreProcessor(StackFrame *frame);

static bool aastoreProcessor(StackFrame *frame);

static bool bastoreProcessor(StackFrame *frame);

static bool castoreProcessor(StackFrame *frame);

static bool sastoreProcessor(StackFrame *frame);

static bool popProcessor(StackFrame *frame);

static bool pop2Processor(StackFrame *frame);

static bool dupProcessor(StackFrame *frame);

static bool dupX1Processor(StackFrame *frame);

static bool dupX2Processor(StackFrame *frame);

static bool dup2Processor(StackFrame *frame);

static bool dup2X1Processor(StackFrame *frame);

static bool dup2X2Processor(StackFrame *frame);

static bool swapProcessor(StackFrame *frame);

static bool iaddProcessor(StackFrame *frame);

static bool laddProcessor(StackFrame *frame);

static bool faddProcessor(StackFrame *frame);

static bool daddProcessor(StackFrame *frame);

static bool isubProcessor(StackFrame *frame);

static bool lsubProcessor(StackFrame *frame);

static bool fsubProcessor(StackFrame *frame);

static bool dsubProcessor(StackFrame *frame);

static bool imulProcessor(StackFrame *frame);

static bool lmulProcessor(StackFrame *frame);

static bool fmulProcessor(StackFrame *frame);

static bool dmulProcessor(StackFrame *frame);

static bool idivProcessor(StackFrame *frame);

static bool ldivProcessor(StackFrame *frame);

static bool fdivProcessor(StackFrame *frame);

static bool ddivProcessor(StackFrame *frame);

static bool iremProcessor(StackFrame *frame);

static bool lremProcessor(StackFrame *frame);

static bool fremProcessor(StackFrame *frame);

static bool dremProcessor(StackFrame *frame);

static bool inegProcessor(StackFrame *frame);

static bool lnegProcessor(StackFrame *frame);

static bool fnegProcessor(StackFrame *frame);

static bool dnegProcessor(StackFrame *frame);

static bool ishlProcessor(StackFrame *frame);

static bool lshlProcessor(StackFrame *frame);

static bool ishrProcessor(StackFrame *frame);

static bool lshrProcessor(StackFrame *frame);

static bool iushrProcessor(StackFrame *frame);

static bool lushrProcessor(StackFrame *frame);

static bool iandProcessor(StackFrame *frame);

static bool landProcessor(StackFrame *frame);

static bool iorProcessor(StackFrame *frame);

static bool lorProcessor(StackFrame *frame);

static bool ixorProcessor(StackFrame *frame);

static bool lxorProcessor(StackFrame *frame);

static bool iincProcessor(StackFrame *frame);

static bool i2lProcessor(StackFrame *frame);

static bool i2fProcessor(StackFrame *frame);

static bool i2dProcessor(StackFrame *frame);

static bool l2iProcessor(StackFrame *frame);

static bool l2fProcessor(StackFrame *frame);

static bool l2dProcessor(StackFrame *frame);

static bool f2iProcessor(StackFrame *frame);

static bool f2lProcessor(StackFrame *frame);

static bool f2dProcessor(StackFrame *frame);

static bool d2iProcessor(StackFrame *frame);

static bool d2lProcessor(StackFrame *frame);

static bool d2fProcessor(StackFrame *frame);

static bool i2bProcessor(StackFrame *frame);

static bool i2cProcessor(StackFrame *frame);

static bool i2sProcessor(StackFrame *frame);

static bool lcmpProcessor(StackFrame *frame);

static bool fcmplProcessor(StackFrame *frame);

static bool fcmpgProcessor(StackFrame *frame);

static bool dcmplProcessor(StackFrame *frame);

static bool dcmpgProcessor(StackFrame *frame);

static bool ifeqProcessor(StackFrame *frame);

static bool ifneProcessor(StackFrame *frame);

static bool ifltProcessor(StackFrame *frame);

static bool ifgeProcessor(StackFrame *frame);

static bool ifgtProcessor(StackFrame *frame);

static bool ifleProcessor(StackFrame *frame);

static bool ifIcmpeqProcessor(StackFrame *frame);

static bool ifIcmpneProcessor(StackFrame *frame);

static bool ifIcmpltProcessor(StackFrame *frame);

static bool ifIcmpgeProcessor(StackFrame *frame);

static bool ifIcmpgtProcessor(StackFrame *frame);

static bool ifIcmpleProcessor(StackFrame *frame);

static bool ifAcmpeqProcessor(StackFrame *frame);

static bool ifAcmpneProcessor(StackFrame *frame);

static bool gotoProcessor(StackFrame *frame);

static bool jsrProcessor(StackFrame *frame);

static bool retProcessor(StackFrame *frame);

static bool tableswitchProcessor(StackFrame *frame);

static bool lookupswitchProcessor(StackFrame *frame);

static bool ireturnProcessor(StackFrame *frame);

static bool lreturnProcessor(StackFrame *frame);

static bool freturnProcessor(StackFrame *frame);

static bool dreturnProcessor(StackFrame *frame);

static bool areturnProcessor(StackFrame *frame);

static bool returnProcessor(StackFrame *frame);

static bool getStaticProcessor(StackFrame *frame);

static bool putStaticProcessor(StackFrame *frame);

static bool getFieldProcessor(StackFrame *frame);

static bool putFieldProcessor(StackFrame *frame);

static bool invokeVirtualProcessor(StackFrame *frame);

static bool invokeSpecialProcessor(StackFrame *frame);

static bool invokeStaticProcessor(StackFrame *frame);

static bool invokeInterfaceProcessor(StackFrame *frame);

static bool invokeDynamicProcessor(StackFrame *frame);

static bool newProcessor(StackFrame *frame);

static bool newArrayProcessor(StackFrame *frame);

static bool aNewArrayProcessor(StackFrame *frame);

static bool arrayLengthProcessor(StackFrame *frame);

static bool athrowProcessor(StackFrame *frame);

static bool checkCastProcessor(StackFrame *frame);

static bool instanceofProcessor(StackFrame *frame);

static bool monitorEnterProcessor(StackFrame *frame);

static bool monitorExitProcessor(StackFrame *frame);

static bool wideProcessor(StackFrame *frame);

static bool multiaNewArrayProcessor(StackFrame *frame);

static bool ifNullProcessor(StackFrame *frame);

static bool ifNonNullProcessor(StackFrame *frame);

static bool gotoWProcessor(StackFrame *frame);

static bool jsrWProcessor(StackFrame *frame);

static bool NoOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool WideOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool IIncOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool BranchOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool Index8OperandFetcher(ByteReader *reader, StackFrame *frame);

static bool Index16OperandFetcher(ByteReader *reader, StackFrame *frame);

static bool BipushOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool SipushOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool GotoWOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool NewArrayOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool LookupSwitchOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool TableSwitchOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool InvokeInterfaceOperandFetcher(ByteReader *reader, StackFrame *frame);

static bool MultiANewArrayOperandFetcher(ByteReader *reader, StackFrame *frame);

Instruction instructionTable[] = {
    {nop, "nop", {0}, 0, 1, nopProcessor, NoOperandFetcher, 0},
    {aconst_null, "aconst_null", {0}, 0, 1, aconstNullProcessor, NoOperandFetcher, 0},
    {iconst_m1, "iconst_m1", {0}, 0, 1, iconstM1Processor, NoOperandFetcher, 0},
    {iconst_0, "iconst_0", {0}, 0, 1, iconst0Processor, NoOperandFetcher, 0},
    {iconst_1, "iconst_1", {0}, 0, 1, iconst1Processor, NoOperandFetcher, 0},
    {iconst_2, "iconst_2", {0}, 0, 1, iconst2Processor, NoOperandFetcher, 0},
    {iconst_3, "iconst_3", {0}, 0, 1, iconst3Processor, NoOperandFetcher, 0},
    {iconst_4, "iconst_4", {0}, 0, 1, iconst4Processor, NoOperandFetcher, 0},
    {iconst_5, "iconst_5", {0}, 0, 1, iconst5Processor, NoOperandFetcher, 0},
    {lconst_0, "lconst_0", {0}, 0, 1, lconst0Processor, NoOperandFetcher, 0},
    {lconst_1, "lconst_1", {0}, 0, 1, lconst1Processor, NoOperandFetcher, 0},
    {fconst_0, "fconst_0", {0}, 0, 1, fconst0Processor, NoOperandFetcher, 0},
    {fconst_1, "fconst_1", {0}, 0, 1, fconst1Processor, NoOperandFetcher, 0},
    {fconst_2, "fconst_2", {0}, 0, 1, fconst2Processor, NoOperandFetcher, 0},
    {dconst_0, "dconst_0", {0}, 0, 1, dconst0Processor, NoOperandFetcher, 0},
    {dconst_1, "dconst_1", {0}, 0, 1, dconst1Processor, NoOperandFetcher, 0},
    {bipush, "bipush", {0}, 1, 2, bipushProcessor, BipushOperandFetcher, 0},
    {sipush, "sipush", {0}, 2, 3, sipushProcessor, SipushOperandFetcher, 0},
    {ldc, "ldc", {0}, 1, 2, ldcProcessor, Index8OperandFetcher, 0},
    {ldc_w, "ldc_w", {0}, 2, 3, ldcWProcessor, Index16OperandFetcher, 0},
    {ldc2_w, "ldc2_w", {0}, 2, 3, ldc2WProcessor, Index16OperandFetcher, 0},
    {iload, "iload", {0}, 1, 2, iloadProcessor, Index8OperandFetcher, 0},
    {lload, "lload", {0}, 1, 2, lloadProcessor, Index8OperandFetcher, 0},
    {fload, "fload", {0}, 1, 2, floadProcessor, Index8OperandFetcher, 0},
    {dload, "dload", {0}, 1, 2, dloadProcessor, Index8OperandFetcher, 0},
    {aload, "aload", {0}, 1, 2, aloadProcessor, Index8OperandFetcher, 0},
    {iload_0, "iload_0", {0}, 0, 1, iload0Processor, NoOperandFetcher, 0},
    {iload_1, "iload_1", {0}, 0, 1, iload1Processor, NoOperandFetcher, 0},
    {iload_2, "iload_2", {0}, 0, 1, iload2Processor, NoOperandFetcher, 0},
    {iload_3, "iload_3", {0}, 0, 1, iload3Processor, NoOperandFetcher, 0},
    {lload_0, "lload_0", {0}, 0, 1, lload0Prcoessor, NoOperandFetcher, 0},
    {lload_1, "lload_1", {0}, 0, 1, lload1Processor, NoOperandFetcher, 0},
    {lload_2, "lload_2", {0}, 0, 1, lload2Processor, NoOperandFetcher, 0},
    {lload_3, "lload_3", {0}, 0, 1, lload3Processor, NoOperandFetcher, 0},
    {fload_0, "fload_0", {0}, 0, 1, fload0Processor, NoOperandFetcher, 0},
    {fload_1, "fload_1", {0}, 0, 1, fload1Processor, NoOperandFetcher, 0},
    {fload_2, "fload_2", {0}, 0, 1, fload2Processor, NoOperandFetcher, 0},
    {fload_3, "fload_3", {0}, 0, 1, fload3Processor, NoOperandFetcher, 0},
    {dload_0, "dload_0", {0}, 0, 1, dload0Processor, NoOperandFetcher, 0},
    {dload_1, "dload_1", {0}, 0, 1, dload1Processor, NoOperandFetcher, 0},
    {dload_2, "dload_2", {0}, 0, 1, dload2Processor, NoOperandFetcher, 0},
    {dload_3, "dload_3", {0}, 0, 1, dload3Processor, NoOperandFetcher, 0},
    {aload_0, "aload_0", {0}, 0, 1, aload0Processor, NoOperandFetcher, 0},
    {aload_1, "aload_1", {0}, 0, 1, aload1Processor, NoOperandFetcher, 0},
    {aload_2, "aload_2", {0}, 0, 1, aload2Processor, NoOperandFetcher, 0},
    {aload_3, "aload_3", {0}, 0, 1, aload3Processor, NoOperandFetcher, 0},
    {iaload, "iaload", {0}, 0, 1, ialoadProcessor, NoOperandFetcher, 0},
    {laload, "laload", {0}, 0, 1, laloadProcessor, NoOperandFetcher, 0},
    {faload, "faload", {0}, 0, 1, faloadProcessor, NoOperandFetcher, 0},
    {daload, "daload", {0}, 0, 1, daloadProcessor, NoOperandFetcher, 0},
    {aaload, "aaload", {0}, 0, 1, aaloadProcessor, NoOperandFetcher, 0},
    {baload, "baload", {0}, 0, 1, baloadProcessor, NoOperandFetcher, 0},
    {caload, "caload", {0}, 0, 1, caloadProcessor, NoOperandFetcher, 0},
    {saload, "saload", {0}, 0, 1, saloadProcessor, NoOperandFetcher, 0},
    {istore, "istore", {0}, 1, 2, istoreProcessor, Index8OperandFetcher, 0},
    {lstore, "lstore", {0}, 1, 2, lstoredProcessor, Index8OperandFetcher, 0},
    {fstore, "fstore", {0}, 1, 2, fstoreProcessor, Index8OperandFetcher, 0},
    {dstore, "dstore", {0}, 1, 2, dstoreProcessor, Index8OperandFetcher, 0},
    {astore, "astore", {0}, 1, 2, astoreProcessor, Index8OperandFetcher, 0},
    {istore_0, "istore_0", {0}, 0, 1, istore0Processor, NoOperandFetcher, 0},
    {istore_1, "istore_1", {0}, 0, 1, istore1Processor, NoOperandFetcher, 0},
    {istore_2, "istore_2", {0}, 0, 1, istore2Processor, NoOperandFetcher, 0},
    {istore_3, "istore_3", {0}, 0, 1, istore3Processor, NoOperandFetcher, 0},
    {lstore_0, "lstore_0", {0}, 0, 1, lstore0Processor, NoOperandFetcher, 0},
    {lstore_1, "lstore_1", {0}, 0, 1, lstore1Processor, NoOperandFetcher, 0},
    {lstore_2, "lstore_2", {0}, 0, 1, lstore2Processor, NoOperandFetcher, 0},
    {lstore_3, "lstore_3", {0}, 0, 1, lstore3Processor, NoOperandFetcher, 0},
    {fstore_0, "fstore_0", {0}, 0, 1, fstore0Processor, NoOperandFetcher, 0},
    {fstore_1, "fstore_1", {0}, 0, 1, fstore1Processor, NoOperandFetcher, 0},
    {fstore_2, "fstore_2", {0}, 0, 1, fstore2Processor, NoOperandFetcher, 0},
    {fstore_3, "fstore_3", {0}, 0, 1, fstore3Processor, NoOperandFetcher, 0},
    {dstore_0, "dstore_0", {0}, 0, 1, dstore0Processor, NoOperandFetcher, 0},
    {dstore_1, "dstore_1", {0}, 0, 1, dstore1Processor, NoOperandFetcher, 0},
    {dstore_2, "dstore_2", {0}, 0, 1, dstore2Processor, NoOperandFetcher, 0},
    {dstore_3, "dstore_3", {0}, 0, 1, dstore3Processor, NoOperandFetcher, 0},
    {astore_0, "astore_0", {0}, 0, 1, astore0Processor, NoOperandFetcher, 0},
    {astore_1, "astore_1", {0}, 0, 1, astore1Processor, NoOperandFetcher, 0},
    {astore_2, "astore_2", {0}, 0, 1, astore2Processor, NoOperandFetcher, 0},
    {astore_3, "astore_3", {0}, 0, 1, astore3Processor, NoOperandFetcher, 0},
    {iastore, "iastore", {0}, 0, 1, iastoreProcessor, NoOperandFetcher, 0},
    {lastore, "lastore", {0}, 0, 1, lastoreProcessor, NoOperandFetcher, 0},
    {fastore, "fastore", {0}, 0, 1, fastoreProcessor, NoOperandFetcher, 0},
    {dastore, "dastore", {0}, 0, 1, dastoreProcessor, NoOperandFetcher, 0},
    {aastore, "aastore", {0}, 0, 1, aastoreProcessor, NoOperandFetcher, 0},
    {bastore, "bastore", {0}, 0, 1, bastoreProcessor, NoOperandFetcher, 0},
    {castore, "castore", {0}, 0, 1, castoreProcessor, NoOperandFetcher, 0},
    {sastore, "sastore", {0}, 0, 1, sastoreProcessor, NoOperandFetcher, 0},
    {pop, "pop", {0}, 0, 1, popProcessor, NoOperandFetcher, 0},
    {pop2, "pop2", {0}, 0, 1, pop2Processor, NoOperandFetcher, 0},
    {dup, "dup", {0}, 0, 1, dupProcessor, NoOperandFetcher, 0},
    {dup_x1, "dup_x1", {0}, 0, 1, dupX1Processor, NoOperandFetcher, 0},
    {dup_x2, "dup_x2", {0}, 0, 1, dupX2Processor, NoOperandFetcher, 0},
    {dup2, "dup2", {0}, 0, 1, dup2Processor, NoOperandFetcher, 0},
    {dup2_x1, "dup2_x1", {0}, 0, 1, dup2X1Processor, NoOperandFetcher, 0},
    {dup2_x2, "dup2_x2", {0}, 0, 1, dup2X2Processor, NoOperandFetcher, 0},
    {swap, "swap", {0}, 0, 1, swapProcessor, NoOperandFetcher, 0},
    {iadd, "iadd", {0}, 0, 1, iaddProcessor, NoOperandFetcher, 0},
    {ladd, "ladd", {0}, 0, 1, laddProcessor, NoOperandFetcher, 0},
    {fadd, "fadd", {0}, 0, 1, faddProcessor, NoOperandFetcher, 0},
    {dadd, "dadd", {0}, 0, 1, daddProcessor, NoOperandFetcher, 0},
    {isub, "isub", {0}, 0, 1, isubProcessor, NoOperandFetcher, 0},
    {lsub, "lsub", {0}, 0, 1, lsubProcessor, NoOperandFetcher, 0},
    {fsub, "fsub", {0}, 0, 1, fsubProcessor, NoOperandFetcher, 0},
    {dsub, "dsub", {0}, 0, 1, dsubProcessor, NoOperandFetcher, 0},
    {imul, "imul", {0}, 0, 1, imulProcessor, NoOperandFetcher, 0},
    {lmul, "lmul", {0}, 0, 1, lmulProcessor, NoOperandFetcher, 0},
    {fmul, "fmul", {0}, 0, 1, fmulProcessor, NoOperandFetcher, 0},
    {dmul, "dmul", {0}, 0, 1, dmulProcessor, NoOperandFetcher, 0},
    {idiv, "idiv", {0}, 0, 1, idivProcessor, NoOperandFetcher, 0},
    {ldiv, "ldiv", {0}, 0, 1, ldivProcessor, NoOperandFetcher, 0},
    {fdiv, "fdiv", {0}, 0, 1, fdivProcessor, NoOperandFetcher, 0},
    {ddiv, "ddiv", {0}, 0, 1, ddivProcessor, NoOperandFetcher, 0},
    {irem, "irem", {0}, 0, 1, iremProcessor, NoOperandFetcher, 0},
    {lrem, "lrem", {0}, 0, 1, lremProcessor, NoOperandFetcher, 0},
    {frem, "frem", {0}, 0, 1, fremProcessor, NoOperandFetcher, 0},
    {drem, "drem", {0}, 0, 1, dremProcessor, NoOperandFetcher, 0},
    {ineg, "ineg", {0}, 0, 1, inegProcessor, NoOperandFetcher, 0},
    {lneg, "lneg", {0}, 0, 1, lnegProcessor, NoOperandFetcher, 0},
    {fneg, "fneg", {0}, 0, 1, fnegProcessor, NoOperandFetcher, 0},
    {dneg, "dneg", {0}, 0, 1, dnegProcessor, NoOperandFetcher, 0},
    {ishl, "ishl", {0}, 0, 1, ishlProcessor, NoOperandFetcher, 0},
    {lshl, "lshl", {0}, 0, 1, lshlProcessor, NoOperandFetcher, 0},
    {ishr, "ishr", {0}, 0, 1, ishrProcessor, NoOperandFetcher, 0},
    {lshr, "lshr", {0}, 0, 1, lshrProcessor, NoOperandFetcher, 0},
    {iushr, "iushr", {0}, 0, 1, iushrProcessor, NoOperandFetcher, 0},
    {lushr, "lushr", {0}, 0, 1, lushrProcessor, NoOperandFetcher, 0},
    {iand, "iand", {0}, 0, 1, iandProcessor, NoOperandFetcher, 0},
    {land, "land", {0}, 0, 1, landProcessor, NoOperandFetcher, 0},
    {ior, "ior", {0}, 0, 1, iorProcessor, NoOperandFetcher, 0},
    {lor, "lor", {0}, 0, 1, lorProcessor, NoOperandFetcher, 0},
    {ixor, "ixor", {0}, 0, 1, ixorProcessor, NoOperandFetcher, 0},
    {lxor, "lxor", {0}, 0, 1, lxorProcessor, NoOperandFetcher, 0},
    {iinc, "iinc", {0}, 0, 1, iincProcessor, NoOperandFetcher, 0},
    {i2l, "i2l", {0}, 0, 1, i2lProcessor, NoOperandFetcher, 0},
    {i2f, "i2f", {0}, 0, 1, i2fProcessor, NoOperandFetcher, 0},
    {i2d, "i2d", {0}, 0, 1, i2dProcessor, NoOperandFetcher, 0},
    {l2i, "l2i", {0}, 0, 1, l2iProcessor, NoOperandFetcher, 0},
    {l2f, "l2f", {0}, 0, 1, l2fProcessor, NoOperandFetcher, 0},
    {l2d, "l2d", {0}, 0, 1, l2dProcessor, NoOperandFetcher, 0},
    {f2i, "f2i", {0}, 0, 1, f2iProcessor, NoOperandFetcher, 0},
    {f2l, "f2l", {0}, 0, 1, f2lProcessor, NoOperandFetcher, 0},
    {f2d, "f2d", {0}, 0, 1, f2dProcessor, NoOperandFetcher, 0},
    {d2i, "d2i", {0}, 0, 1, d2iProcessor, NoOperandFetcher, 0},
    {d2l, "d2l", {0}, 0, 1, d2lProcessor, NoOperandFetcher, 0},
    {d2f, "d2f", {0}, 0, 1, d2fProcessor, NoOperandFetcher, 0},
    {i2b, "i2b", {0}, 0, 1, i2bProcessor, NoOperandFetcher, 0},
    {i2c, "i2c", {0}, 0, 1, i2cProcessor, NoOperandFetcher, 0},
    {i2s, "i2s", {0}, 0, 1, i2sProcessor, NoOperandFetcher, 0},
    {lcmp, "lcmp", {0}, 0, 1, lcmpProcessor, NoOperandFetcher, 0},
    {fcmpl, "fcmpl", {0}, 0, 1, fcmplProcessor, NoOperandFetcher, 0},
    {fcmpg, "fcmpg", {0}, 0, 1, fcmpgProcessor, NoOperandFetcher, 0},
    {dcmpl, "dcmpl", {0}, 0, 1, dcmplProcessor, NoOperandFetcher, 0},
    {dcmpg, "dcmpg", {0}, 0, 1, dcmpgProcessor, NoOperandFetcher, 0},
    {ifeq, "ifeq", {0}, 2, 3, ifeqProcessor, BranchOperandFetcher, 0},
    {ifne, "ifne", {0}, 2, 3, ifneProcessor, BranchOperandFetcher, 0},
    {iflt, "iflt", {0}, 2, 3, ifltProcessor, BranchOperandFetcher, 0},
    {ifge, "ifge", {0}, 2, 3, ifgeProcessor, BranchOperandFetcher, 0},
    {ifgt, "ifgt", {0}, 2, 3, ifgtProcessor, BranchOperandFetcher, 0},
    {ifle, "ifle", {0}, 2, 3, ifleProcessor, BranchOperandFetcher, 0},
    {if_icmpeq, "if_icmpeq", {0}, 2, 3, ifIcmpeqProcessor, BranchOperandFetcher, 0},
    {if_icmpne, "if_icmpne", {0}, 2, 3, ifIcmpneProcessor, BranchOperandFetcher, 0},
    {if_icmplt, "if_icmplt", {0}, 2, 3, ifIcmpltProcessor, BranchOperandFetcher, 0},
    {if_icmpge, "if_icmpge", {0}, 2, 3, ifIcmpgeProcessor, BranchOperandFetcher, 0},
    {if_icmpgt, "if_icmpgt", {0}, 2, 3, ifIcmpgtProcessor, BranchOperandFetcher, 0},
    {if_icmple, "if_icmple", {0}, 2, 3, ifIcmpleProcessor, BranchOperandFetcher, 0},
    {if_acmpeq, "if_acmpeq", {0}, 2, 3, ifAcmpeqProcessor, BranchOperandFetcher, 0},
    {if_acmpne, "if_acmpne", {0}, 2, 3, ifAcmpneProcessor, BranchOperandFetcher, 0},
    {goto, "goto", {0}, 2, 3, gotoProcessor, BranchOperandFetcher, 0},
    {jsr, "jsr", {0}, 2, 3, jsrProcessor, BranchOperandFetcher, 0},
    {ret, "ret", {0}, 1, 2, retProcessor, BranchOperandFetcher, 0},
    {tableswitch, "tableswitch", {0}, 15, 16, tableswitchProcessor, TableSwitchOperandFetcher, 0},
    {lookupswitch, "lookupswitch", {0}, 16, 17, lookupswitchProcessor, LookupSwitchOperandFetcher, 0},
    {ireturn, "ireturn", {0}, 0, 1, ireturnProcessor, NoOperandFetcher, 0},
    {lreturn, "lreturn", {0}, 0, 1, lreturnProcessor, NoOperandFetcher, 0},
    {freturn, "freturn", {0}, 0, 1, freturnProcessor, NoOperandFetcher, 0},
    {dreturn, "dreturn", {0}, 0, 1, dreturnProcessor, NoOperandFetcher, 0},
    {areturn, "areturn", {0}, 0, 1, areturnProcessor, NoOperandFetcher, 0},
    {nreturn, "nreturn", {0}, 0, 1, returnProcessor, NoOperandFetcher, 0},
    {getstatic, "getstatic", {0}, 2, 3, getStaticProcessor, Index16OperandFetcher, 0},
    {putstatic, "putstatic", {0}, 2, 3, putStaticProcessor, Index16OperandFetcher, 0},
    {getfield, "getfield", {0}, 2, 3, getFieldProcessor, Index16OperandFetcher, 0},
    {putfield, "putfield", {0}, 2, 3, putFieldProcessor, Index16OperandFetcher, 0},
    {invokevirtual, "invokevirtual", {0}, 2, 3, invokeVirtualProcessor, Index16OperandFetcher, 0},
    {invokespecial, "invokespecial", {0}, 2, 3, invokeSpecialProcessor, Index16OperandFetcher, 0},
    {invokestatic, "invokestatic", {0}, 2, 3, invokeStaticProcessor, Index16OperandFetcher, 0},
    {invokeinterface, "invokeinterface", {0}, 2, 3, invokeInterfaceProcessor, InvokeInterfaceOperandFetcher, 0},
    {invokedynamic, "invokedynamic", {0}, 2, 3, invokeDynamicProcessor, Index16OperandFetcher, 0},
    {new, "new", {0}, 2, 3, newProcessor, Index16OperandFetcher, 0},
    {newarray, "newarray", {0}, 1, 2, newArrayProcessor, NewArrayOperandFetcher, 0},
    {anewarray, "anewarray", {0}, 2, 3, aNewArrayProcessor, Index16OperandFetcher, 0},
    {arraylength, "arraylength", {0}, 0, 1, arrayLengthProcessor, NoOperandFetcher, 0},
    {athrow, "athrow", {0}, 0, 1, athrowProcessor, NoOperandFetcher, 0},
    {checkcast, "checkcast", {0}, 2, 3, checkCastProcessor, Index16OperandFetcher, 0},
    {instanceof, "instanceof", {0}, 2, 3, instanceofProcessor, Index16OperandFetcher, 0},
    {monitorenter, "monitorenter", {0}, 0, 1, monitorEnterProcessor, NoOperandFetcher, 0},
    {monitorexit, "monitorexit", {0}, 0, 1, monitorExitProcessor, NoOperandFetcher, 0},
    {wide, "wide", {0}, 17, 18, wideProcessor, WideOperandFetcher, 0},
    {multianewarray, "multianewarray", {0}, 3, 4, multiaNewArrayProcessor, MultiANewArrayOperandFetcher, 0},
    {ifnull, "ifnull", {0}, 2, 3, ifNullProcessor, BranchOperandFetcher, 0},
    {ifnonnull, "ifnonnull", {0}, 2, 3, ifNonNullProcessor, BranchOperandFetcher, 0},
    {goto_w, "goto_w", {0}, 4, 5, gotoWProcessor, GotoWOperandFetcher, 0},
    {jsr_w, "jsr_w", {0}, 4, 5, jsrWProcessor, BranchOperandFetcher, 0}
};

Instruction *getInstructionByCode(u8 opercode) {
    return &instructionTable[opercode];
}

static bool nopProcessor(StackFrame *frame) {
    return true;
}

static bool aconstNullProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushReference(operandStack, NULL);
    return true;
}

static bool iconstM1Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, -1);
    return true;
}

static bool iconst0Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 0);
    return true;
}

static bool iconst1Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 1);
    return true;
}

static bool iconst2Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 2);
    return true;
}

static bool iconst3Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 3);
    return true;
}

static bool iconst4Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 4);
    return true;
}

static bool iconst5Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, 5);
    return true;
}

static bool lconst0Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushLong(operandStack, 0);
    return true;
}

static bool lconst1Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushLong(operandStack, 1);
    return true;
}

static bool fconst0Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, 0.0);
    return true;
}

static bool fconst1Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, 1.0);
    return true;
}

static bool fconst2Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushFloat(operandStack, 2.0);
    return true;
}

static bool dconst0Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushDouble(operandStack, 0.0);
    return true;
}

static bool dconst1Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushDouble(operandStack, 1.0);
    return true;
}

static bool bipushProcessor(StackFrame *frame) {
    int value = frame->operandStore.bipushOperand.value;
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, value);
    return true;
}

static bool sipushProcessor(StackFrame *frame) {
    int value = frame->operandStore.sipushOperand.value;
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushInt(operandStack, value);
    return true;
}

static bool ldcProcessor(StackFrame *frame) {

    return true;
}

static bool ldcWProcessor(StackFrame *frame) {
    return true;
}

static bool ldc2WProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    Class *class = frame->method->class;
    u32 index = frame->operandStore.index16Operand.index;
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

static bool iloadProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return iloadProcess(frame, index8Operand.index);
}

static bool lloadProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return lloadProcess(frame, index8Operand.index);
}

static bool floadProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return floadProcess(frame, index8Operand.index);
}

static bool dloadProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return dloadProcess(frame, index8Operand.index);
}

static bool aloadProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    
    return aloadProcess(frame, index8Operand.index);
}

static bool iload0Processor(StackFrame *frame) {
    return iloadProcess(frame, 0);
}

static bool iload1Processor(StackFrame *frame) {
    return iloadProcess(frame, 1);
}

static bool iload2Processor(StackFrame *frame) {
    return iloadProcess(frame, 2);
}

static bool iload3Processor(StackFrame *frame) {
    return iloadProcess(frame, 3);
}

static bool lload0Prcoessor(StackFrame *frame) {
    return lloadProcess(frame, 0);
}

static bool lload1Processor(StackFrame *frame) {
    return lloadProcess(frame, 1);
}

static bool lload2Processor(StackFrame *frame) {
    return lloadProcess(frame, 2);
}

static bool lload3Processor(StackFrame *frame) {
    return lloadProcess(frame, 3);
}

static bool fload0Processor(StackFrame *frame) {
    return floadProcess(frame, 0);
}

static bool fload1Processor(StackFrame *frame) {
    return floadProcess(frame, 1);
}

static bool fload2Processor(StackFrame *frame) {
    return floadProcess(frame, 2);
}

static bool fload3Processor(StackFrame *frame) {
    return floadProcess(frame, 3);
}

static bool dload0Processor(StackFrame *frame) {
    return dloadProcess(frame, 0);
}

static bool dload1Processor(StackFrame *frame) {
    return dloadProcess(frame, 1);
}

static bool dload2Processor(StackFrame *frame) {
    return dloadProcess(frame, 2);
}

static bool dload3Processor(StackFrame *frame) {
    return dloadProcess(frame, 3);
}

static bool aload0Processor(StackFrame *frame) {
    return aloadProcess(frame, 0);
}

static bool aload1Processor(StackFrame *frame) {
    return aloadProcess(frame, 1);
}

static bool aload2Processor(StackFrame *frame) {
   return aloadProcess(frame, 2);
}

static bool aload3Processor(StackFrame *frame) {
    return aloadProcess(frame, 3);
}

static bool ialoadProcessor(StackFrame *frame) {
    return true;
}

static bool laloadProcessor(StackFrame *frame) {
    return true;
}

static bool faloadProcessor(StackFrame *frame) {
    return true;
}

static bool daloadProcessor(StackFrame *frame) {
    return true;
}

static bool aaloadProcessor(StackFrame *frame) {
    return true;
}

static bool baloadProcessor(StackFrame *frame) {
    return true;
}

static bool caloadProcessor(StackFrame *frame) {
    return true;
}

static bool saloadProcessor(StackFrame *frame) {
    return true;
}

static bool istoreProcess(StackFrame *frame, u32 index) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);

    LocalVariables *localVariables = frame->localVariables;
    localVariables->setInt(localVariables, index, value);

    return true;
}

static bool istoreProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
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

static bool lstoredProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
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

static bool fstoreProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
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

static bool dstoreProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
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

static bool astoreProcessor(StackFrame *frame) {
    OperandStore operandStore = frame->operandStore;
    Index8Operand index8Operand = operandStore.index8Operand;
    u32 index = index8Operand.index;

    return astoreProcess(frame, index);
}

static bool istore0Processor(StackFrame *frame) {
    return istoreProcess(frame, 0);
}

static bool istore1Processor(StackFrame *frame) {
    return istoreProcess(frame, 1);
}

static bool istore2Processor(StackFrame *frame) {
    return istoreProcess(frame, 2);
}

static bool istore3Processor(StackFrame *frame) {
    return istoreProcess(frame, 3);
}

static bool lstore0Processor(StackFrame *frame) {
    return lstoreProcess(frame, 0);
}

static bool lstore1Processor(StackFrame *frame) {
    return lstoreProcess(frame, 1);
}

static bool lstore2Processor(StackFrame *frame) {
    return lstoreProcess(frame, 2);
}

static bool lstore3Processor(StackFrame *frame) {
    return lstoreProcess(frame, 3);
}

static bool fstore0Processor(StackFrame *frame) {
    return fstoreProcess(frame, 0);
}

static bool fstore1Processor(StackFrame *frame) {
    return fstoreProcess(frame, 1);
}

static bool fstore2Processor(StackFrame *frame) {
    return fstoreProcess(frame, 2);
}

static bool fstore3Processor(StackFrame *frame) {
    return fstoreProcess(frame, 3);
}

static bool dstore0Processor(StackFrame *frame) {
    return dstoreProcess(frame, 0);
}

static bool dstore1Processor(StackFrame *frame) {
    return dstoreProcess(frame, 1);
}

static bool dstore2Processor(StackFrame *frame) {
    return dstoreProcess(frame, 2);
}

static bool dstore3Processor(StackFrame *frame) {
    return dstoreProcess(frame, 3);
}

static bool astore0Processor(StackFrame *frame) {
    return astoreProcess(frame, 0);
}

static bool astore1Processor(StackFrame *frame) {
    return astoreProcess(frame, 1);
}

static bool astore2Processor(StackFrame *frame) {
    return astoreProcess(frame, 2);
}

static bool astore3Processor(StackFrame *frame) {
    return astoreProcess(frame, 3);
}

static bool iastoreProcessor(StackFrame *frame) {
    return true;
}

static bool lastoreProcessor(StackFrame *frame) {
    return true;
}

static bool fastoreProcessor(StackFrame *frame) {
    return true;
}

static bool dastoreProcessor(StackFrame *frame) {
    return true;
}

static bool aastoreProcessor(StackFrame *frame) {
    return true;
}

static bool bastoreProcessor(StackFrame *frame) {
    return true;
}

static bool castoreProcessor(StackFrame *frame) {
    return true;
}

static bool sastoreProcessor(StackFrame *frame) {
    return true;
}

static bool popProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->popSlotData(operandStack);
    
    return true;
}

static bool pop2Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    operandStack->popSlotData(operandStack);
    operandStack->popSlotData(operandStack);

    return true;
}

static bool dupProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData = operandStack->popSlotData(operandStack);
    operandStack->pushSlotData(operandStack, slotData);

    return true;
}

static bool dupX1Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);
    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);

    return true;
}

static bool dupX2Processor(StackFrame *frame) {
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

static bool dup2Processor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData2);
    operandStack->pushSlotData(operandStack, slotData1);

    return true;
}

static bool dup2X1Processor(StackFrame *frame) {
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

static bool dup2X2Processor(StackFrame *frame) {
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

static bool swapProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    SlotData *slotData1 = operandStack->popSlotData(operandStack);
    SlotData *slotData2 = operandStack->popSlotData(operandStack);

    operandStack->pushSlotData(operandStack, slotData1);
    operandStack->pushSlotData(operandStack, slotData2);
    return true;
}

static bool iaddProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 + value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool laddProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 + value2;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool faddProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 + value2;
    operandStack->pushFloat(operandStack, result);

    return true;
}

static bool daddProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 + value2;
    operandStack->pushDouble(operandStack, result);

    return true;
}

static bool isubProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2= operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 - value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lsubProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 - value2;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool fsubProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 - value2;
    operandStack->pushFloat(operandStack, result);

    return true;
}

static bool dsubProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 - value2;
    operandStack->pushDouble(operandStack, result);

    return true;
}

static bool imulProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 * value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lmulProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 * value2;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool fmulProcessor(StackFrame *frame) {
   OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 * value2;
    operandStack->pushFloat(operandStack, result);

    return true;
}

static bool dmulProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 * value2;
    operandStack->pushDouble(operandStack, result);

    return true;
}

static bool idivProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 / value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool ldivProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 / value2;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool fdivProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = value1 / value2;
    operandStack->pushFloat(operandStack, result);

    return true;
}

static bool ddivProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = value1 / value2;
    operandStack->pushDouble(operandStack, result);

    return true;
}

static bool iremProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);

    int result = value1 % value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lremProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popLong(operandStack);
    long value1 = operandStack->popLong(operandStack);

    long result = value1 % value2;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool fremProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value2 = operandStack->popFloat(operandStack);
    float value1 = operandStack->popFloat(operandStack);

    float result = (float)fmod((double)value1, (double)value2);
    operandStack->pushFloat(operandStack, result);
    return true;
}

static bool dremProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value2 = operandStack->popDouble(operandStack);
    double value1 = operandStack->popDouble(operandStack);

    double result = fmod(value1, value2);
    operandStack->pushDouble(operandStack, result);
    return true;
}

static bool inegProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value = operandStack->popInt(operandStack);

    operandStack->pushInt(operandStack, -value);
    return true;
}

static bool lnegProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value = operandStack->popLong(operandStack);

    operandStack->pushLong(operandStack, -value);
    return true;
}

static bool fnegProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    float value = operandStack->popFloat(operandStack);

    operandStack->pushFloat(operandStack, -value);
    return true;
}

static bool dnegProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    double value = operandStack->popDouble(operandStack);

    operandStack->pushDouble(operandStack, -value);
    return true;
}

static bool ishlProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);
    u32 num = ((u32)value2) & 0X1F;
    int result = value1 << num;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lshlProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popInt(operandStack);
    long value1 = operandStack->popLong(operandStack);
    u32 num = ((u32)value2) & 0X1F;
    long result = value1 << num;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool ishrProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);
    u32 num = ((u32)value2) & 0X1F;
    int result = value1 >> num;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lshrProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popInt(operandStack);
    long value1 = operandStack->popLong(operandStack);
    u32 num = ((u32)value2) & 0X1F;
    long result = value1 >> num;
    operandStack->pushLong(operandStack, result);

    return true;
}

static bool iushrProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value2 = operandStack->popInt(operandStack);
    int value1 = operandStack->popInt(operandStack);
    u32 num = ((u32)value2) & 0X1F;
    int result = (int)(((u32)value1) >> num);
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lushrProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value2 = operandStack->popInt(operandStack);
    long value1 = operandStack->popLong(operandStack);
    u32 num = ((u32)value2) & 0X1F;
    long result = (long)(((u64)value1) >> num);
    operandStack->pushLong(operandStack, result);
}

static bool iandProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value1 = operandStack->popInt(operandStack);
    int value2 = operandStack->popInt(operandStack);

    int result = value1 & value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool landProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value1 = operandStack->popLong(operandStack);
    long value2 = operandStack->popLong(operandStack);

    long result = value1 & value2;
    operandStack->pushLong(operandStack, result);
    
    return true;
}

static bool iorProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value1 = operandStack->popInt(operandStack);
    int value2 = operandStack->popInt(operandStack);

    int result = value1 | value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lorProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    long value1 = operandStack->popLong(operandStack);
    long value2 = operandStack->popLong(operandStack);

    long result = value1 | value2;
    operandStack->pushLong(operandStack, result);
    
    return true;
}

static bool ixorProcessor(StackFrame *frame) {
    OperandStack *operandStack = frame->operandStack;
    int value1 = operandStack->popInt(operandStack);
    int value2 = operandStack->popInt(operandStack);

    int result = value1 ^ value2;
    operandStack->pushInt(operandStack, result);

    return true;
}

static bool lxorProcessor(StackFrame *frame) {
   OperandStack *operandStack = frame->operandStack;
    long value1 = operandStack->popLong(operandStack);
    long value2 = operandStack->popLong(operandStack);

    long result = value1 ^ value2;
    operandStack->pushLong(operandStack, result);
    
    return true;
}

static bool iincProcessor(StackFrame *frame) {
    return true;
}

static bool i2lProcessor(StackFrame *frame) {
    return true;
}

static bool i2fProcessor(StackFrame *frame) {
    return true;
}

static bool i2dProcessor(StackFrame *frame) {
    return true;
}

static bool l2iProcessor(StackFrame *frame) {
    return true;
}

static bool l2fProcessor(StackFrame *frame) {
    return true;
}

static bool l2dProcessor(StackFrame *frame) {
    return true;
}

static bool f2iProcessor(StackFrame *frame) {
    return true;
}

static bool f2lProcessor(StackFrame *frame) {
    return true;
}

static bool f2dProcessor(StackFrame *frame) {
    return true;
}

static bool d2iProcessor(StackFrame *frame) {
    return true;
}

static bool d2lProcessor(StackFrame *frame) {
    return true;
}

static bool d2fProcessor(StackFrame *frame) {
    return true;
}

static bool i2bProcessor(StackFrame *frame) {
    return true;
}

static bool i2cProcessor(StackFrame *frame) {
    return true;
}

static bool i2sProcessor(StackFrame *frame) {
    return true;
}

static bool lcmpProcessor(StackFrame *frame) {
    return true;
}

static bool fcmplProcessor(StackFrame *frame) {
    return true;
}

static bool fcmpgProcessor(StackFrame *frame) {
    return true;
}

static bool dcmplProcessor(StackFrame *frame) {
    return true;
}

static bool dcmpgProcessor(StackFrame *frame) {
    return true;
}

static bool ifeqProcessor(StackFrame *frame) {
    return true;
}

static bool ifneProcessor(StackFrame *frame) {
    return true;
}

static bool ifltProcessor(StackFrame *frame) {
    return true;
}

static bool ifgeProcessor(StackFrame *frame) {
    return true;
}

static bool ifgtProcessor(StackFrame *frame) {
    return true;
}

static bool ifleProcessor(StackFrame *frame) {
    return true;
}

static bool ifIcmpeqProcessor(StackFrame *frame) {
    return true;
}

static bool ifIcmpneProcessor(StackFrame *frame) {
    return true;
}

static bool ifIcmpltProcessor(StackFrame *frame) {
    return true;
}

static bool ifIcmpgeProcessor(StackFrame *frame) {
    return true;
}

static bool ifIcmpgtProcessor(StackFrame *frame) {
    return true;
}

static bool ifIcmpleProcessor(StackFrame *frame) {
    return true;
}

static bool ifAcmpeqProcessor(StackFrame *frame) {
    return true;
}

static bool ifAcmpneProcessor(StackFrame *frame) {
    return true;
}

static bool gotoProcessor(StackFrame *frame) {
    return true;
}

static bool jsrProcessor(StackFrame *frame) {
    return true;
}

static bool retProcessor(StackFrame *frame) {
    return true;
}

static bool tableswitchProcessor(StackFrame *frame) {
    return true;
}

static bool lookupswitchProcessor(StackFrame *frame) {
    return true;
}

static bool ireturnProcessor(StackFrame *frame) {
    Thread *thread = frame->thread;
    StackFrame *currentFrame = thread->popStackFrame(thread);
    StackFrame *invokeFrame = thread->peekStackFrame(thread);

    OperandStack *currentOperandStack = currentFrame->operandStack;
    int value = currentOperandStack->popInt(currentOperandStack);
    
    OperandStack *invokeOperandStack = invokeFrame->operandStack;
    invokeOperandStack->pushInt(invokeOperandStack, value);

    return true;
}

static bool lreturnProcessor(StackFrame *frame) {
    return true;
}

static bool freturnProcessor(StackFrame *frame) {
    return true;
}

static bool dreturnProcessor(StackFrame *frame) {
    return true;
}

static bool areturnProcessor(StackFrame *frame) {
    return true;
}

static bool returnProcessor(StackFrame *frame) {
    return true;
}

static bool getStaticProcessor(StackFrame *frame) {
    return true;
}

static bool putStaticProcessor(StackFrame *frame) {
    return true;
}

static bool getFieldProcessor(StackFrame *frame) {
    return true;
}

static bool putFieldProcessor(StackFrame *frame) {
    return true;
}

static bool invokeMethodProcess(Thread *thread, StackFrame *stackFrame, Method *method) {
	StackFrame *newStackFrame = createStackFrame(thread, method);
	ByteReader *byteReader = createByteReader(method->codeData, method->codeLength, 0);

    RuntimeStack *runtimeStack = thread->runtimeStack;
    runtimeStack->pushStack(runtimeStack, newStackFrame);

    MethodDescriptor *descriptor = method->methodDescriptor;
    OperandStack *operandStack = stackFrame->operandStack;
    LocalVariables *newLocalVariables = newStackFrame->localVariables;

    int i = 0;
    for (i=descriptor->paramTypeCount-1; i>=0; --i) {
        SlotData *slotData = operandStack->popSlotData(operandStack);
        newLocalVariables->setSlotData(newLocalVariables, i, slotData);
    }

	while(byteReader->pc < byteReader->length) {
		u32 pc = byteReader->pc;
		u8 operCode = byteReader->readByte(byteReader);
		Instruction *instruction = getInstructionByCode(operCode);
		printf("    PC:%02d %s\n", pc, instruction->name);
		instruction->fetcher(byteReader, newStackFrame);
		instruction->processor(newStackFrame);
	}
    return true;
}

static bool invokeVirtualProcessor(StackFrame *frame) {
    return true;
}

static bool invokeSpecialProcessor(StackFrame *frame) {
    return true;
}

static bool invokeStaticProcessor(StackFrame *frame) {

    ConstPool *constPool;
    ConstUtf8Info *constUtf8Info;
    ConstClassInfo *constClassInfo;
    ConstMethodRefInfo *constMethodRefInfo;
    ConstNameAndTypeInfo *constNameAndTypeInfo;

    OperandStore operandStore = frame->operandStore;
    u16 index = operandStore.index16Operand.index;
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

    invokeMethodProcess(thread, frame, invokeMethod);

    return true;
}

static bool invokeInterfaceProcessor(StackFrame *frame) {
    return true;
}

static bool invokeDynamicProcessor(StackFrame *frame) {
    return true;
}

static bool newProcessor(StackFrame *frame) {
    Index16Operand index16Operand = frame->operandStore.index16Operand;
    u16 index = index16Operand.index;
    Class *class = frame->method->class;
    ConstPool *constPool = class->getConstant(class, index);
    ConstClassInfo *constClassInfo = (ConstClassInfo *)constPool->value;
    constPool = class->getConstant(class, constClassInfo->nameIndex);
    ConstUtf8Info *constUtf8Info = (ConstUtf8Info *)constPool->value;
    char *instClassName = constUtf8Info->bytes;
    Class *instClass = vmachine->findClassByName(vmachine, instClassName);
    Instance *instance = createInstance(instClass);
    OperandStack *operandStack = frame->operandStack;
    operandStack->pushReference(operandStack, instance);

    return true;
}

static bool newArrayProcessor(StackFrame *frame) {
    return true;
}

static bool aNewArrayProcessor(StackFrame *frame) {
    return true;
}

static bool arrayLengthProcessor(StackFrame *frame) {
    return true;
}

static bool athrowProcessor(StackFrame *frame) {
    return true;
}

static bool checkCastProcessor(StackFrame *frame) {
    return true;
}

static bool instanceofProcessor(StackFrame *frame) {
    return true;
}

static bool monitorEnterProcessor(StackFrame *frame) {
    return true;
}

static bool monitorExitProcessor(StackFrame *frame) {
    return true;
}

static bool wideProcessor(StackFrame *frame) {
    return true;
}

static bool multiaNewArrayProcessor(StackFrame *frame) {
    return true;
}

static bool ifNullProcessor(StackFrame *frame) {
    return true;
}

static bool ifNonNullProcessor(StackFrame *frame) {
    return true;
}

static bool gotoWProcessor(StackFrame *frame) {
    return true;
}

static bool jsrWProcessor(StackFrame *frame) {
    return true;
}


static bool NoOperandFetcher(ByteReader *reader, StackFrame *frame) {
    return true;
}

static bool WideOperandFetcher(ByteReader *reader, StackFrame *frame) {
    return true;
}

static bool IIncOperandFetcher(ByteReader *reader, StackFrame *frame) {
    u32 operand1 = reader->readByte(reader);
    int operand2 = reader->readByte(reader);
    frame->operandStore.iincOperand.index = operand1;
    frame->operandStore.iincOperand.constValue = operand2;
    return true;
}

static bool BranchOperandFetcher(ByteReader *reader, StackFrame *frame) {
    short operand = reader->readShort(reader);
    frame->operandStore.branchOperand.offset = operand;
    return true;
}

static bool Index8OperandFetcher(ByteReader *reader, StackFrame *frame) {
    byte operand = reader->readByte(reader);
    frame->operandStore.index8Operand.index = operand;
    return true;
}

static bool Index16OperandFetcher(ByteReader *reader, StackFrame *frame) {
    short operand = reader->readShort(reader);
    frame->operandStore.index16Operand.index = operand;
    return true;
}

static bool BipushOperandFetcher(ByteReader *reader, StackFrame *frame) {
    byte operand = reader->readByte(reader);
    frame->operandStore.bipushOperand.value = operand;
    return true;
}

static bool SipushOperandFetcher(ByteReader *reader, StackFrame *frame) {
    short operand = reader->readShort(reader);
    frame->operandStore.sipushOperand.value = operand;
    return true;
}

static bool GotoWOperandFetcher(ByteReader *reader, StackFrame *frame) {
    int operand = reader->readInteger(reader);
    frame->operandStore.gotoWOperand.offset = operand;
    return true;
}

static bool NewArrayOperandFetcher(ByteReader *reader, StackFrame *frame) {
    return true;
}

static bool LookupSwitchOperandFetcher(ByteReader *reader, StackFrame *frame) {
    return true;
}

static bool TableSwitchOperandFetcher(ByteReader *reader, StackFrame *frame) {
    return true;
}

static bool InvokeInterfaceOperandFetcher(ByteReader *reader, StackFrame *frame) {
    u16 operand = reader->readShort(reader);
    frame->operandStore.invokeInterfaceOperand.index = operand;
    reader->readByte(reader);
    reader->readByte(reader);

    return true;
}

static bool MultiANewArrayOperandFetcher(ByteReader *reader, StackFrame *frame) {
    return true;
}