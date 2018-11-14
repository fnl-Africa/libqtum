//Simple contract which will add 2 32-bit integers
//compile: i686-qtum-gcc adder.c -o adder -Wall
#include <qtum.h>

int onCreate(){
    return 0;
}

int main(){
    uint64_t balance = qtumGetBalance(&qtumExec->self);
    qtumEventStringInt64("sent value", qtumExec->valueSent);
    qtumEventStringInt64("current balance", balance);
    return 0;
}