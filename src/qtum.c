#include "qtum.h"
#include <stdio.h>
#include <string.h>


static int screen_putc(int c, FILE *stream)
{
    // put character on the screen
    return 1;
}

static int keyboard_getc(FILE *stream)
{
    // read and return a character from keyboard
    return 0;
}

static void screen_pre_output(FILE *stream)
{
    // read cursor position before output
}

static void screen_post_output(FILE *stream)
{
    // set cursor position after output
}

FILE scr_term;

void setup_terminal_io()
{
    // required if output is used
    scr_term.putc = screen_putc;

    // required if input is used
    scr_term.getc = keyboard_getc;

    // optional
    scr_term.pre_output = screen_pre_output;
    scr_term.post_output = screen_post_output;

    // required initial state if input
    // is used
    scr_term.ungetc_buf = -1;

    // pointer to custom data
    scr_term.user_ptr = NULL;

    // initialize needed streams
    //stdout = &scr_term;
}

void __init_qtum(){
    setup_terminal_io();
}


int isCreate(){
    return __qtum_syscall(QSC_IsCreate, 0, 0, 0, 0, 0, 0);
}

const void* getCallData(){
    return __tx_call_data;
}
int getCallDataSize(){
    if(isCreate()){
        return 0;
    }
    return transactionData->callDataSize;
}


uint32_t getPrevBlockTime(){
    return __qtum_syscall(QSC_PreviousBlockTime, 0, 0, 0, 0, 0, 0);
}
uint64_t getBlockGasLimit(){
    uint64_t res = 0;
    __qtum_syscall(QSC_BlockGasLimit, (uint32_t) &res, 0, 0, 0, 0, 0);
    return res;
}
void getBlockCreator(UniversalAddressABI *address){
    __qtum_syscall(QSC_BlockCreator, (uint32_t) address, 0, 0, 0, 0, 0);
}
uint64_t getBlockDifficulty(){
    uint64_t res = 0;
    __qtum_syscall(QSC_BlockDifficulty, (uint32_t) &res, 0, 0, 0, 0, 0);
    return res;
}
uint32_t getBlockHeight(){
    return __qtum_syscall(QSC_BlockHeight, 0, 0, 0, 0, 0, 0);
}

int getBlockHash(uint32_t height, hash256_t* hash){
    return __qtum_syscall(QSC_GetBlockHash, height, (uint32_t) hash, 0, 0, 0, 0);
}

int getSelfAddress(UniversalAddressABI *address){
    return __qtum_syscall(QSC_SelfAddress, (uint32_t) address, 0, 0, 0, 0, 0);
}

int qtumStore(const void* key, size_t keysize, const void* value, size_t valuesize){
    return __qtum_syscall(QSC_WriteStorage, (uint32_t)key, keysize, (uint32_t) value, valuesize,0,0);
}
int qtumLoad(const void* key, size_t keysize, void* value, size_t maxvalueeize){
    return __qtum_syscall(QSC_ReadStorage, (uint32_t) key, keysize, (uint32_t) value, maxvalueeize, 0, 0);
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

int getSender(UniversalAddressABI *address){
    return __qtum_syscall(QSC_SenderAddress, (uint32_t) address, 0, 0, 0, 0, 0);
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
size_t qtumStackPop(void* buffer, size_t maxSize){
     return __qtum_syscall(QSC_SCCSPop, (uint32_t) buffer, maxSize, 0, 0, 0, 0);
}
size_t qtumStackPeek(void* buffer, size_t maxSize){
     return __qtum_syscall(QSC_SCCSPeek, (uint32_t) buffer, maxSize, 0, 0, 0, 0);
}
int qtumStackPush(const void* buffer, size_t size){
     return __qtum_syscall(QSC_SCCSPush, (uint32_t) buffer, size, 0, 0, 0, 0);
}
int qtumStackDiscard(){
     return __qtum_syscall(QSC_SCCSDiscard, 0, 0, 0, 0, 0, 0);
}
int qtumStackClear(){
     return __qtum_syscall(QSC_SCCSClear, 0, 0, 0, 0, 0, 0);
}











