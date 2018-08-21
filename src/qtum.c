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
    if(isCreate()){
        return NULL;
    }
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














