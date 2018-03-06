#include "Binary.h"

Binary::Binary()
{
	Bin = 0;
}

Binary::Binary(int value)
{
	Bin = value;
}

Binary::Binary(const Binary &obj)
{
	Bin = obj.Bin;
}

std::string Binary::ToString(int length, int interval) const
{
	std::string str;
	for (int i = length; i >= 0; i--)
	{
		str += '0' + ((Bin >> i) & 1);
		if (i % interval == 0) str += ' ';
	}
	return str;
}

Binary::BitProxy::BitProxy(int index)
{
	Index = index;
}

Binary::BitProxy::BitProxy(Binary* binary, int index)
{
	Bin = binary;
	Index = index;
}

int Binary::BitProxy::operator=(int value)
{
	if (value == 0)
	{
		Bin->Bin &= ~0 - (1 << Index);
	}
	else
	{
		Bin->Bin |= 1 << Index;
	}
	return value;
}

Binary::BitProxy::operator int() const
{
	return (Bin->Bin >> Index) & 1;
}

Binary::BitProxy Binary::operator[](int index)
{
	return Binary::BitProxy(this, index);
}

const Binary::BitProxy Binary::operator[](int index) const
{
	return Binary::BitProxy(index);
}