#include <qtum.h>
#include <stdlib.h>
#include <string.h>

//dumb ABI
//sends specified amount from msg.sender balance to address
//format: address:address value:uint64 SEND -> sendervalue:uint64 recvvalue:uint64
#define CONTRACT_SEND 1
//returns the balance held by the specified address
//format: address:address BALANCE -> balance:uint64
#define CONTRACT_BALANCE 0
//returns balance of getSender()
//format: SELFBALANCE -> balance:uint64
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



struct simpletoken_Send_Params{
    UniversalAddressABI* address;
    uint64_t value;
};

struct simpletoken_Send_Returns{
    uint64_t recvvalue;
    uint64_t sendervalue;
};

void decodeABI(){
    //format: address:address value:uint64 SEND -> sendervalue:uint64 recvvalue:uint64
    //format: address:address BALANCE -> balance:uint6
    //format: SELFBALANCE -> balance:uint64
    uint32_t function = 0;
    if(qtumStackItemCount() == 0){
        //fallback function... 
    }
    QTUM_POP_VAL(function);
    switch(function){
        case CONTRACT_SELFBALANCE:
        {
            uint64_t resBalance;
            selfBalance(&resBalance);
            QTUM_PUSH_VAL(resBalance);
            return;
        }
        case CONTRACT_BALANCE:
        {
            UniversalAddressABI address;
            QTUM_POP_VAL(address);

            uint64_t resBalance;
            balance(&address, &resBalance);
            QTUM_PUSH_VAL(resBalance);
            return;
        }
        case CONTRACT_SEND:
        {
            struct simpletoken_Send_Params params;
            UniversalAddressABI __tmp1;
            params.address = &__tmp1;


            QTUM_POP_VAL(params.value);
            QTUM_POP_VAL(__tmp1);
            struct simpletoken_Send_Returns returns;

            send(&params, &returns);
            QTUM_PUSH_VAL(returns.sendervalue);
            QTUM_PUSH_VAL(returns.recvvalue);
            return;
        }
        default:
            qtumError("Invalid function");
            return;
    }
}

//format for this:
//address:address value:uint64 SEND -> sendervalue:uint64 recvvalue:uint64
struct QtumCallResultABI simpletoken_Send(const UniversalAddressABI* __contract, uint64_t __gasLimit,
    const struct simpletoken_Send_Params* params,
    struct simpletoken_Send_Returns* returns
    )
{
    if(__gasLimit == 0){
        __gasLimit = QTUM_CALL_GASLIMIT;
    }
    qtumStackClear();
    QTUM_PUSH_VAL(*params->address);
    QTUM_PUSH_VAL(params->value);
    uint32_t f = CONTRACT_SEND;
    QTUM_PUSH_VAL(f);
    struct QtumCallResultABI result;
    qtumCallContract(__contract, __gasLimit, 0, &result);
    if(result.errorCode != 0){
        return result;
    }
    QTUM_POP_VAL(returns->recvvalue);
    QTUM_POP_VAL(returns->sendervalue);
    return result;
}

