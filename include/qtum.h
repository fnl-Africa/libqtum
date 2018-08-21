#ifndef QTUM_H
#define QTUM_H

#include <stdint.h>

//internal addresses

#define CODE_ADDRESS 0x1000
#define MAX_CODE_SIZE 0x10000
#define DATA_ADDRESS 0x100000
#define MAX_DATA_SIZE 0x10000
#define STACK_ADDRESS 0x200000
#define MAX_STACK_SIZE 1024 * 8

#define TX_CALL_DATA_ADDRESS 0x210000
static const uint8_t * const __tx_call_data = (uint8_t*) 0x210000;

#define TX_DATA_ADDRESS 0xD0000000
#define TX_DATA_ADDRESS_END 0xF0000000

//system calls:

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


    //storage commands, 0x1000
#define QSC_ReadStorage             0x1000
#define QSC_WriteStorage            0x1001

    //value commands, 0x2000
#define QSC_SendValue               0x2000 //send coins somewhere
#define QSC_GetBalance              0x2001
#define QSC_SendValueAndCall        0x2002

    //caller commands, 0x3000
#define QSC_GasProvided             0x3000
#define QSC_CallerTransaction       0x3001 //provides output scripts, etc
#define QSC_ValueProvided           0x3002
#define QSC_OriginAddress           0x3003
#define QSC_SenderAddress           0x3004
#define QSC_CallStackSize           0x3005

    //call commands, 0x4000
#define QSC_CallContract            0x4000
#define QSC_CallLibrary             0x4001




//debug only system calls
#define SYSCALL_INTERNAL_PRINT 0xFFFF0001

//internal syscall routine
long __qtum_syscall(long number, long p1, long p2, long p3, long p4, long p5, long p6);

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
const void* getCallData();
int isCreate();
int getSender(UniversalAddressABI* ua);

uint32_t getPrevBlockTime();
uint64_t getBlockGasLimit();
void getBlockCreator(UniversalAddressABI *address);
uint64_t getBlockDifficulty();
uint32_t getBlockHeight();

int getBlockHash(uint32_t height, hash256_t* hash);
int getSelfAddress(UniversalAddressABI *addr);




#endif
