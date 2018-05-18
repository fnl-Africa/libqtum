#include "qtum.h"
#include <stdio.h>


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
    return __qtum_syscall(SYSCALL_IS_CREATE, 0, 0, 0, 0, 0, 0);
}

void* getCallData(){
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

















