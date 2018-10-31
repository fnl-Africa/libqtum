#ifndef QTUM_H
#define QTUM_H

#include <stdint.h>
#include <stddef.h>

#include <shared-x86.h>

//internal syscall routine
long __qtum_syscall(long number, long p1, long p2, long p3, long p4, long p5, long p6);
void __qtum_terminate(long errorCode) __attribute__ ((noreturn));

//called internally to initialize libc, etc
void __init_qtum();


typedef uint8_t hash256_t[32];

static const struct BlockDataABI* const qtumBlockchain = (const struct BlockDataABI* const) BLOCK_DATA_ADDRESS;
static const struct ExecDataABI* const qtumExec = (const struct ExecDataABI* const) EXEC_DATA_ADDRESS;
//static const TxDataABI* const transactionData = (const TxDataABI* const) TX_DATA_ADDRESS;
#define qtumSender (qtumExec->sender)

//metadata api

size_t qtumStore(const void* key, size_t keysize, const void* value, size_t valuesize); //returns value size
size_t qtumLoad(const void* key, size_t keysize, void* value, size_t maxvalueeize); //returns actual value size
void qtumLoadExact(const void* key, size_t keysize, void* value, size_t valueeize); //returns actual value size

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
size_t qtumStackPeekNoError(void* buffer, size_t maxSize);
void qtumStackPush(const void* buffer, size_t size);
int qtumStackDiscard();
int qtumStackClear();


//helper macros

#define QTUM_POP_VAL(b) do{qtumStackPop(&(b), sizeof(b));}while(0)
#define QTUM_PUSH_VAL(b) do{qtumStackPush(&(b), sizeof(b));}while(0)
#define QTUM_PEEK_VAL(b) do{qtumStackPeek(&(b), sizeof(b));}while(0)

#define QTUM_STORE_VAL(k, v) do{qtumStore(&(k), sizeof(k), &(v), sizeof(v));}while(0)
#define QTUM_LOAD_VAL(k, v) do{qtumLoadExact(&(k), sizeof(k), &(v), sizeof(v));}while(0)



#endif