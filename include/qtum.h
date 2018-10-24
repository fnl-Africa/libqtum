#ifndef QTUM_H
#define QTUM_H

#include <stdint.h>
#include <stddef.h>

//internal addresses

#define CODE_ADDRESS 0x1000
#define MAX_CODE_SIZE 0x10000
#define DATA_ADDRESS 0x100000
#define MAX_DATA_SIZE 0x10000
#define STACK_ADDRESS 0x200000
#define MAX_STACK_SIZE 1024 * 8

#define TX_DATA_ADDRESS 0xD0000000
#define TX_DATA_ADDRESS_END 0xF0000000

//BEGIN copy from qtum

//constants below this line should exactly match libqtum's qtum.h! 

static const int QTUM_SYSTEM_ERROR_INT = 0xFF;

//interrupt 0x40
//QSC = Qtum System Call
//use defines here so that we can use it in C easily

//basic execution calls, 0x0000
#define QSC_BlockGasLimit           1
#define QSC_BlockCreator            2
#define QSC_BlockDifficulty         3
#define QSC_BlockHeight             4
#define QSC_GetBlockHash            5
#define QSC_IsCreate                6
#define QSC_SelfAddress             7
#define QSC_PreviousBlockTime       8
#define QSC_UsedGas                 9
#define QSC_SetFaultHandler         10
#define QSC_SetDoubleFaultHandler   11
#define QSC_CodeSize                12
#define QSC_DataSize                13
#define QSC_ScratchSize             14
#define QSC_SelfDestruct            15

#define QSC_AddEvent                16
/* -- this quickly gets very complicated. Defer/cancel implementation
#define QSC_GetEvent                17
#define QSC_GetEventSize            18
#define QSC_ExecutingCallID         19
#define QSC_NextCallID              20
*/


    //storage commands, 0x1000
#define QSC_ReadStorage             0x1000
#define QSC_WriteStorage            0x1001

    //value commands, 0x2000
#define QSC_SendValue               0x2000 //send coins somewhere
#define QSC_GetBalance              0x2001
#define QSC_SendValueAndCall        0x2002

    //callee commands, 0x3000
#define QSC_GasProvided             0x3000
#define QSC_CallerTransaction       0x3001 //provides output scripts, etc
#define QSC_ValueProvided           0x3002
#define QSC_OriginAddress           0x3003
#define QSC_SenderAddress           0x3004
#define QSC_CallStackSize           0x3005
//SCCS = Smart Contract Communication Stack
//note: Upon contract error, this stack is not cleared. Thus an error contract can have side effects
#define QSC_SCCSItemCount               0x3006
//#define QSC_SCCSMaxItems            0x3007
//#define QSC_SCCSMaxSize             0x3008
#define QSC_SCCSSize                0x3009
#define QSC_SCCSItemSize            0x300A
#define QSC_SCCSPop                 0x300B
#define QSC_SCCSPeek                0x300C
#define QSC_SCCSPush                0x300D
#define QSC_SCCSDiscard             0x300E //pops off the stack without any data transfer possible (for cheaper gas cost)
#define QSC_SCCSClear               0x300F

    //caller commands, 0x4000
#define QSC_CallContract            0x4000
#define QSC_CallLibrary             0x4001

//error code types
//These will cause appropriate revert of state etc
//note, this is the last value pushed onto SCCS upon contract termination
#define QTUM_EXIT_SUCCESS 0 //successful execution
#define QTUM_EXIT_HAS_DATA 1 //there is user defined data pushed onto the stack (optional, no consensus function)
#define QTUM_EXIT_REVERT 2 //execution that reverted state
#define QTUM_EXIT_ERROR 4 //error execution (which may or may not revert state)
#define QTUM_EXIT_OUT_OF_GAS 8 //execution which ended in out of gas exception
#define QTUM_EXIT_CRASH 16 //execution which ended due to CPU or memory errors
#define QTUM_EXIT_SYSCALL_EXCEPTION 32 //execution which ended due to an exception by a syscall, such as transfering more money than the contract owns

//NOTE: only QTUM_EXIT_SUCCESS, QTUM_EXIT_ERROR, QTUM_EXIT_REVERT, and QTUM_HAS_DATA may be specified by __qtum_terminate

//ABI type prefixes
//note the limit for this is 15, since it should fit into a nibble
#define ABI_TYPE_UNKNOWN 0
#define ABI_TYPE_INT 1
#define ABI_TYPE_UINT 2
#define ABI_TYPE_HEX 3
#define ABI_TYPE_STRING 4
#define ABI_TYPE_BOOL 5
#define ABI_TYPE_ADDRESS 6

enum QtumEndpoint{
    QtumExit = 0xF0,
    QtumSystem = 0x40,
    QtumTrustedLibrary = 0x41,
    InteralUI = 0x50
};

//END copy from qtum

//internal syscall routine
long __qtum_syscall(long number, long p1, long p2, long p3, long p4, long p5, long p6);
void __qtum_terminate(long errorCode) __attribute__ ((noreturn));

//called internally to initialize libc, etc
void __init_qtum();


//structs
#define ADDRESS_DATA_SIZE 32

typedef struct{
    //Do not modify this struct's fields
    //This is consensus critical!
    uint32_t version;
    uint8_t data[ADDRESS_DATA_SIZE];
} __attribute__((__packed__)) UniversalAddressABI;

typedef struct {
    uint32_t size;
    uint32_t callDataSize;
    UniversalAddressABI sender;
} __attribute__((__packed__)) TxDataABI;

typedef uint8_t hash256_t[32];

static const TxDataABI* const transactionData = (const TxDataABI* const) TX_DATA_ADDRESS;

//metadata api
int isCreate();
int getSender(UniversalAddressABI* ua);

uint32_t getPrevBlockTime();
uint64_t getBlockGasLimit();
void getBlockCreator(UniversalAddressABI *address);
uint64_t getBlockDifficulty();
uint32_t getBlockHeight();

int getBlockHash(uint32_t height, hash256_t* hash);
int getSelfAddress(UniversalAddressABI *addr);

int qtumStore(const void* key, size_t keysize, const void* value, size_t valuesize); //returns value size
int qtumLoad(const void* key, size_t keysize, void* value, size_t maxvalueeize); //returns actual value size

//event functions
int qtumEvent(const void* key, size_t keysize, int keytype, const void* value, size_t valuesize, int valuetype);
int qtumEventStringString(const char* key, const char* value);
int qtumEventStringInt64(const char* key, int64_t value);
int qtumEventAddressInt64(const UniversalAddressABI* key, int64_t value);

//helper function that just does qtumEventStringString("error", msg); and then kills contract execution
void qtumError(const char* msg) __attribute__ ((noreturn));
void qtumErrorWithCode(uint32_t code, const char* msg) __attribute__ ((noreturn));

int qtumStackItemCount();
size_t qtumStackMemorySize();
size_t qtumStackItemSize();
void qtumStackPop(void* buffer, size_t size);
size_t qtumStackPopNoError(void* buffer, size_t maxSize);
void qtumStackPeek(void* buffer, size_t maxSize);
size_t qtumStackPeekNoError(void* buffer, size_t size);
void qtumStackPush(const void* buffer, size_t size);
int qtumStackDiscard();
int qtumStackClear();


//helper macros

#define QTUM_POP_VAL(b) do{qtumStackPop(&(b), sizeof(b));}while(0)
#define QTUM_PUSH_VAL(b) do{qtumStackPush(&(b), sizeof(b));}while(0)
#define QTUM_PEEK_VAL(b) do{qtumStackPeek(&(b), sizeof(b));}while(0)



#endif