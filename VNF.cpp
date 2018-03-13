#include "VNF.h"
#include <string>

using namespace std;

VNF::VNF(){}

VNF::~VNF(){}

VNF::VNF(string type, int value){
    typeID = type;
    maxRequest = value;
}

VNF::VNF(string type, int capacity, int maxReq){
    this->typeID = type;
    this->capacity = capacity;
    this->maxRequest = maxReq;
}

