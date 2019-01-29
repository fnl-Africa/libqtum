#ifndef QTUM_H
#define QTUM_H

#include <stdint.h>
#include <stddef.h>

#include "shared-x86.h"

#ifndef QTUM_MOCK
//Can't use raw syscalls with mocking

//internal syscall routine
long __qtum_syscall(long number, long p1, long p2, long p3, long p4, long p5, long p6);
void __qtum_terminate(long errorCode) __attribute__ ((noreturn));
//called internally to initialize libc, etc
void __init_qtum();
#endif


typedef uint8_t hash256_t[32];
#ifndef QTUM_MOCK
static const struct BlockDataABI* const qtumBlockchain = (const struct BlockDataABI* const) BLOCK_DATA_ADDRESS;
static const struct ExecDataABI* const qtumExec = (const struct ExecDataABI* const) EXEC_DATA_ADDRESS;
//static const TxDataABI* const transactionData = (const TxDataABI* const) TX_DATA_ADDRESS;
#else
//when using this as mocks, we need to be able to point this somewhere, but also keep it constant
static struct BlockDataABI* mock_qtumBlockchain = NULL;
static struct ExecDataABI* mock_qtumExec = NULL;

#define qtumBlockchain ((const struct BlockDataABI* const) mock_qtumBlockchain)
#define qtumExec ((const struct ExecDataABI* const) mock_qtumExec)

#endif
#define qtumSender (qtumExec->sender)

//metadata api

size_t qtumStore(const void* key, size_t keysize, const void* value, size_t valuesize); //returns value size
int qtumUpdateBytecode(const void* bytecode, size_t bytecodesize);
size_t qtumLoad(const void* key, size_t keysize, void* value, size_t maxvalueeize); //returns actual value size
void qtumLoadExact(const void* key, size_t keysize, void* value, size_t valueeize); //returns actual value size
void qtumLoadExternal(const UniversalAddressABI* addr, const void* key, size_t keysize, void* value, size_t maxvalueeize);
void qtumLoadExactExternal(const UniversalAddressABI* addr, const void* key, size_t keysize, void* value, size_t valueeize);

//event functions
int qtumEvent(const void* key, size_t keysize, int keytype, const void* value, size_t valuesize, int valuetype);
int qtumEventStringString(const char* key, const char* value);
int qtumEventStringInt64(const char* key, int64_t value);
int qtumEventAddressInt64(const UniversalAddressABI* key, int64_t value);

//helper function that just does qtumEventStringString("error", msg); and then kills contract execution
void qtumError(const char* msg) __attribute__ ((noreturn));
void qtumErrorWithCode(uint32_t code, const char* msg) __attribute__ ((noreturn));

//stack ops
int qtumStackCount();
size_t qtumStackMemorySize();
size_t qtumPeekSize();
void qtumPopExact(void* buffer, size_t size);
size_t qtumStackPop(void* buffer, size_t maxSize);
void qtumPeekExact(void* buffer, size_t maxSize);
size_t qtumPeek(void* buffer, size_t maxSize);
void qtumPush(const void* buffer, size_t size);
int qtumDiscard();
int qtumStackClear();
//helpers
uint8_t qtumPop8();
uint16_t qtumPop16();
uint32_t qtumPop32();
uint64_t qtumPop64();
void qtumPush8(uint8_t v);
void qtumPush16(uint16_t v);
void qtumPush32(uint32_t v);
void qtumPush64(uint64_t v);



int qtumSelfCalled();

int qtumCallContract(const UniversalAddressABI *address, uint32_t gasLimit, uint64_t value, QtumCallResultABI* result);
int qtumParseAddress(const char* address, UniversalAddressABI *output);

uint64_t qtumGetBalance(const UniversalAddressABI* address);


//internal libc helpers
void* __qtum_malloc(unsigned int sz);
void* __qtum_calloc(unsigned int sz);
void __qtum_free(void* mem);
void* __qtum_realloc(void* mem, unsigned int newsz);



//helper macros

#define QTUM_STORE_VAL(k, v) do{qtumStore(&(k), sizeof(k), &(v), sizeof(v));}while(0)
#define QTUM_LOAD_VAL(k, v) do{qtumLoadExact(&(k), sizeof(k), &(v), sizeof(v));}while(0)



#endif