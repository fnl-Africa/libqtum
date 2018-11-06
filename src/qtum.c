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
size_t qtumStackItemSize(){
     return __qtum_syscall(QSC_SCCSItemSize, 0, 0, 0, 0, 0, 0);
}
size_t qtumStackPopNoError(void* buffer, size_t maxSize){
     return __qtum_syscall(QSC_SCCSPop, (uint32_t) buffer, maxSize, 0, 0, 0, 0);
}
size_t qtumStackPeekNoError(void* buffer, size_t maxSize){
     return __qtum_syscall(QSC_SCCSPeek, (uint32_t) buffer, maxSize, 0, 0, 0, 0);
}
void qtumStackPop(void* buffer, size_t size){
    qtumAssert(size != 0, "size must be > 0");
    size_t sz = qtumStackPopNoError(buffer, size);
    qtumAssert(sz == size, "stack error");
}
void qtumStackPeek(void* buffer, size_t size){
    qtumAssert(size != 0, "size must be > 0");
    size_t sz = qtumStackPeekNoError(buffer, size);
    qtumAssert(sz == size, "stack error");
}
void qtumStackPush(const void* buffer, size_t size){
    qtumAssert(size != 0, "size must be > 0");
    __qtum_syscall(QSC_SCCSPush, (uint32_t) buffer, size, 0, 0, 0, 0);
}
int qtumStackDiscard(){
     return __qtum_syscall(QSC_SCCSDiscard, 0, 0, 0, 0, 0, 0);
}
int qtumStackClear(){
     return __qtum_syscall(QSC_SCCSClear, 0, 0, 0, 0, 0, 0);
}

void qtumError(const char* msg){
    qtumStackClear();
    qtumEventStringString("error", msg);
    __qtum_terminate(QTUM_EXIT_ERROR | QTUM_EXIT_REVERT);
}
void qtumErrorWithCode(uint32_t code, const char* msg){
    qtumStackClear();
    qtumStackPush(&code, sizeof(code));
    qtumEventStringString("error", msg);
    __qtum_terminate(QTUM_EXIT_ERROR | QTUM_EXIT_REVERT | QTUM_EXIT_HAS_DATA);
}

int qtumCallContract(const UniversalAddressABI *address, uint32_t gasLimit, uint64_t value, struct QtumCallResultABI* result){
    return __qtum_syscall(QSC_CallContract,
        (uint32_t) address,
         gasLimit,
         (uint32_t) result,
         sizeof(struct QtumCallResultABI),
         (uint32_t) (value  >> 32),
         (uint32_t) (value & 0xFFFFFFFF)
    );
}








