//Simple contract which will add 2 32-bit integers
//compile: i686-qtum-gcc adder.c -o adder -Wall
#include <qtum.h>

int onCreate(){
    return 0;
}

//#define QTUM_GET_ARG(type, name) do{type name; qtumStackPopNoError(&)}while(0)



int main(){
    if(qtumExec->isCreate){
        return 0;
    }
    if(qtumStackItemCount() < 2){
        qtumError("Need 2 items on stack");
    }
    uint32_t a = 0, b = 0;
    //Use no error here because we don't care about size, we're assuming 32 bit either way
    qtumStackPopNoError(&a, sizeof(uint32_t));
    qtumStackPopNoError(&b, sizeof(uint32_t));
    a+=b;
    qtumStackPush(&a, sizeof(uint32_t));
    return 0;
}