#include "qtum.h"
#include <stdio.h>
#include <string.h>


void qtumAssert(int cond, char* msg){
    if(!cond){
        qtumError(msg);
    }
}

void __init_qtum(){
    //nothing here yet
}

size_t qtumStore(const void* key, size_t keysize, const void* value, size_t valuesize){
    return __qtum_syscall(QSC_WriteStorage, (uint32_t)key, keysize, (uint32_t) value, valuesize,0,0);
}
size_t qtumLoad(const void* key, size_t keysize, void* value, size_t maxvalueeize){
    return __qtum_syscall(QSC_ReadStorage, (uint32_t) key, keysize, (uint32_t) value, maxvalueeize, 0, 0);
}
void qtumLoadExact(const void* key, size_t keysize, void* value, size_t valuesize){
    size_t sz = qtumLoad(key, keysize, value, valuesize);
    qtumAssert(sz == valuesize, "Mismatched load size");
}

int qtumEvent(const void* key, size_t keysize, int keytype, const void* value, size_t valuesize, int valuetype){
    int type = (keytype << 4 | (0x0F & valuetype)) & 0xFF;
    return __qtum_syscall(QSC_AddEvent, (uint32_t)key, keysize, (uint32_t) value, valuesize, type,0);
}

int qtumEventStringString(const char* key, const char* value){
    return qtumEvent((uint8_t*) key, strlen(key), ABI_TYPE_STRING, (uint8_t*) value, strlen(value), ABI_TYPE_STRING);
}
int qtumEventStringInt64(const char* key, int64_t value){
    return qtumEvent((uint8_t*) key, strlen(key), ABI_TYPE_STRING, (uint8_t*) &value, sizeof(int64_t), ABI_TYPE_INT);
}
int qtumEventAddressInt64(const UniversalAddressABI* key, int64_t value){
    return qtumEvent((uint8_t*) key, sizeof(*key), ABI_TYPE_ADDRESS, (uint8_t*) &value, sizeof(value), ABI_TYPE_INT);
}


int qtumStackItemCount(){
     return __qtum_syscall(QSC_SCCSItemCount, 0, 0, 0, 0, 0, 0);
}
size_t qtumStackMemorySize(){
     return __qtum_syscall(QSC_SCCSSize, 0, 0, 0, 0, 0, 0);
}
size_t qtumPeekSize(){
     return __qtum_syscall(QSC_SCCSItemSize, 0, 0, 0, 0, 0, 0);
}
size_t qtumPop(void* buffer, size_t maxSize){
     return __qtum_syscall(QSC_SCCSPop, (uint32_t) buffer, maxSize, 0, 0, 0, 0);
}
size_t qtumPeek(void* buffer, size_t maxSize){
     return __qtum_syscall(QSC_SCCSPeek, (uint32_t) buffer, maxSize, 0, 0, 0, 0);
}
void qtumPopExact(void* buffer, size_t size){
    qtumAssert(buffer != NULL, "null buffer");
    size_t sz = qtumPop(buffer, size);
    qtumAssert(sz == size, "stack error");
}
void qtumPeekExact(void* buffer, size_t size){
    qtumAssert(buffer != NULL, "null buffer");
    size_t sz = qtumPeek(buffer, size);
    qtumAssert(sz == size, "stack error");
}
void qtumPush(const void* buffer, size_t size){
    qtumAssert(buffer != NULL, "null buffer");
    __qtum_syscall(QSC_SCCSPush, (uint32_t) buffer, size, 0, 0, 0, 0);
}
int qtumDiscard(){
     return __qtum_syscall(QSC_SCCSDiscard, 0, 0, 0, 0, 0, 0);
}
int qtumStackClear(){
     return __qtum_syscall(QSC_SCCSClear, 0, 0, 0, 0, 0, 0);
}

int qtumSHA256(void *data, size_t dataSize, void *output)
{
    qtumAssert(data != NULL, "null data input");
    qtumAssert(dataSize != 0, "data size should be greater than 0");
    qtumAssert(output != NULL, "null data output");
    return __qtum_syscall(QSC_SHA256, data, dataSize, &output, 0, 0, 0);
}

uint8_t qtumPop8(){
    uint8_t v;
    qtumPopExact(&v, sizeof(v));
    return v;
}
uint16_t qtumPop16(){
    uint16_t v;
    qtumPopExact(&v, sizeof(v));
    return v;
}
uint32_t qtumPop32(){
    uint32_t v;
    qtumPopExact(&v, sizeof(v));
    return v;
}
uint64_t qtumPop64(){
    uint64_t v;
    qtumPopExact(&v, sizeof(v));
    return v;
}
void qtumPush8(uint8_t v){
    qtumPush(&v, sizeof(v));
}
void qtumPush16(uint16_t v){
    qtumPush(&v, sizeof(v));
}
void qtumPush32(uint32_t v){
    qtumPush(&v, sizeof(v));
}
void qtumPush64(uint64_t v){
    qtumPush(&v, sizeof(v));
}

void qtumError(const char* msg){
    qtumStackClear();
    qtumEventStringString("error", msg);
    __qtum_terminate(QTUM_EXIT_ERROR | QTUM_EXIT_REVERT);
}

int qtumCallContract(const UniversalAddressABI *address, uint32_t gasLimit, uint64_t value, QtumCallResultABI* result){
    return __qtum_syscall(QSC_CallContract,
        (uint32_t) address,
         gasLimit,
         (uint32_t) result,
         sizeof(QtumCallResultABI),
         (uint32_t) (value  >> 32),
         (uint32_t) (value & 0xFFFFFFFF)
    );
}

int qtumParseAddress(const char* address, UniversalAddressABI *output){
    return __qtum_syscall(QSC_ParseAddress,
        (uint32_t) address,
        strlen(address),
        (uint32_t) output,
        0, 0 ,0);
}

uint64_t qtumGetBalance(const UniversalAddressABI* address){
    uint64_t ret = 0;
    __qtum_syscall(QSC_GetBalance,
        (uint32_t) address,
        (uint32_t) &ret,
        0, 0, 0, 0);
    return ret;
}

//this __end__ symbol is located at the end of the .data and .bss symbol and is effectively the end of
//the memory which is controlled by the compiler
//this symbol is created by the qtum linker script
extern char __end__;

char* __qtum_malloc_ptr = NULL;
void* __qtum_malloc(unsigned int sz){
    if(__qtum_malloc_ptr == NULL){
        __qtum_malloc_ptr = (char*) &__end__;
    }
    void* v = __qtum_malloc_ptr;
    __qtum_malloc_ptr = &__qtum_malloc_ptr[sz + 1];
    return v;
}
void* __qtum_calloc(unsigned int sz){
    void* p = __qtum_malloc(sz);
    if(p == NULL){
        return NULL;
    }
    memset(p, 0, sz);
    return p;
}
void __qtum_free(void* mem){
    //no-op for now
}
void* __qtum_realloc(void* mem, unsigned int newsz){
    return NULL; //not yet supported
}




