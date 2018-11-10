//Simple contract which will add 2 32-bit integers
//compile: i686-qtum-gcc adder.c -o adder -Wall
#include <qtum.h>

int onCreate(){
    return 0;
}

//format: a:uint32 b:uint32 ADD -> result:uint32
#define ADDER_CONTRACTFN_ADD 1

int main(){
    UniversalAddressABI a;
    if(qtumParseAddress("xCREYTgvdk3XzFNpdXkj35fZX4hJK2Ckpd", &a)){
        qtumError("Error parsing address");
    }
    uint32_t add1 = 100, add2 = 908934;
    QTUM_PUSH_VAL(add1);
    QTUM_PUSH_VAL(add2);
    uint8_t function = 1;
    QTUM_PUSH_VAL(function); //add function

    struct QtumCallResultABI result;
    qtumCallContract(&a, 100000, 0, &result);
    if(result.errorCode != 0){
        qtumError("Adder contract failed");
    }
    QTUM_POP_VAL(add1);
    qtumEventStringInt64("add result", add1);
    return 0;
}