#include "BinaryNode.h"

std::string BinaryNode::ToString(int length, int interval)
{
	std::string str;
	for (int i = length; i >= 0; i--)
	{
		str += '0' + GetBit(i);
		if (i % interval == 0) str += ' ';
	}
	return str;
}

int BinaryNode::GetBit(int index)
{
	return (ID >> index) & 1;
}