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
uint8_t * __tx_call_data = (uint8_t*) 0x210000;

#define TX_DATA_ADDRESS 0xD0000000
#define TX_DATA_ADDRESS_END 0xF0000000



//system calls:

#define SYSCALL_IS_CREATE 6

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

const TxDataABI* const transactionData = (const TxDataABI* const) TX_DATA_ADDRESS;

//metadata api
void* getCallData();
int isCreate();
int getSender(UniversalAddressABI* ua);






#endif
