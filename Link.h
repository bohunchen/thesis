#pragma once

#ifndef LINK_H
#define LINK_H

#include <vector>
#include "EdgeNode.h"

class Link
{
public:
	Link();
	Link(EdgeNode node1, EdgeNode node2, int d);
	
	~Link();
	int delay;
	std::vector<EdgeNode> nodeSet;
};


#endif // !LINK_H

