#pragma once

#ifndef VNFTABLE_H

#define VNFTABLE_H
#include <map>

using namespace std;

#include "VNF.h"

class VNFTable
{
    map<VNF, int > mytable;
};

#endif