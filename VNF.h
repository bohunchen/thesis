#pragma once

#ifndef VNF_H
#define VNF_H

#include <string>

using namespace std;

class VNF
{
public:
    VNF();
    VNF(string type , int value);
    VNF(string type, int capacity, int maxReq);

    bool operator <(const VNF& obj) const
    {
        return typeID < obj.typeID;
    }

    ~VNF();
    string typeID;
    int capacity;
    int maxRequest;
};

#endif