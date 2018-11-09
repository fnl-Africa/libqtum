//Simple contract which will add 2 32-bit integers
//compile: i686-qtum-gcc adder.c -o adder -Wall
#include <qtum.h>

int onCreate(){
    return 0;
}

//format: a:uint32 b:uint32 ADD -> result:uint32
#define ADDER_CONTRACTFN_ADD 1

int main(){
    if(qtumExec->isCreate){
        return 0;
    }
    if(qtumStackItemCount() < 3){
        qtumError("Need 3 items on stack");
    }
    uint8_t func = 0;
    QTUM_POP_VAL(func);
    if(func != ADDER_CONTRACTFN_ADD){
        qtumError("Invalid function");
    }
    uint32_t a = 0, b = 0;
    QTUM_POP_VAL(b);
    QTUM_POP_VAL(a);
    a+=b;
    QTUM_PUSH_VAL(a);
    return 0;
}