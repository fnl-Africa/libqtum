#include <qtum.h>
#include <stdlib.h>
#include <string.h>

//dumb ABI
//sends specified amount from msg.sender balance to address
#define CONTRACT_SEND 1
//returns the balance held by the specified address
#define CONTRACT_BALANCE 0
//returns balance of getSender()
#define CONTRACT_SELFBALANCE 3

#define COINSUPPLY 100000

uint64_t getBalance(const UniversalAddressABI *a){
    //don't use QTUM_LOAD_VAL because it will error upon key not existing or size not exactly matching
    uint64_t value = 0;
    qtumLoad(a, sizeof(*a), &value, sizeof(value));
    return value;
}
void setBalance(const UniversalAddressABI *a, uint64_t value){
    QTUM_STORE_VAL(*a, value);
}

int onCreate(){
    uint64_t tmp = COINSUPPLY; //can't point to a #define
    setBalance(&qtumSender, COINSUPPLY);
    qtumEventAddressInt64(&qtumSender, tmp);
	return 0;
}

//returns sender_balance, receiver_balance
int send(const UniversalAddressABI* recv, uint64_t value){
    uint64_t sendervalue = 0;
    sendervalue = getBalance(&qtumSender);
    if(sendervalue < value){
        qtumError("Not enough tokens");
    }
    uint64_t recvvalue = 0;
    recvvalue = getBalance(recv);
    recvvalue += value; //todo, need safemath equivalent
    sendervalue -= value;

    //now commit to state
    setBalance(recv, recvvalue);
    setBalance(&qtumSender, sendervalue);
    //report useful things
    qtumEventAddressInt64(&qtumSender, sendervalue);
    qtumEventAddressInt64(recv, recvvalue);
    //return useful things
    qtumStackPush(&sendervalue, sizeof(sendervalue));
    qtumStackPush(&recvvalue, sizeof(recvvalue));
    return 0;
}

int balance(const UniversalAddressABI *address){
    uint64_t value = getBalance(address);
    qtumEventAddressInt64(address, value);
    QTUM_PUSH_VAL(value);
    return 0;
}

int selfBalance(){
    return balance(&qtumSender);
}

int main(){
    if(qtumExec->isCreate){
        return 0;
    }
    uint8_t function = 0;
    QTUM_POP_VAL(function);
    switch(function){
        case CONTRACT_BALANCE:
        {
            UniversalAddressABI a;
            QTUM_POP_VAL(a);
            return balance(&a);
        }
        case CONTRACT_SELFBALANCE:
            return selfBalance();
        case CONTRACT_SEND:
        {
            uint64_t value;
            UniversalAddressABI address;
            QTUM_POP_VAL(value);
            QTUM_POP_VAL(address);
            return send(&address, value);
        }
        default:
            qtumError("unknown call");
    }
}
