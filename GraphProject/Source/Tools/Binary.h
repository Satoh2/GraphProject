#pragma once

#include <string>
#include <iostream>

class Binary
{
private:
	int Bin;

public:
	//
	// Constructors
	//

	Binary();
	Binary(int value);
	Binary(const Binary &obj);


	//
	// Basic
	//

	// Returns address in binary string
	std::string ToString(int length, int interval) const;


	//
	// Proxy
	//

	friend class BitProxy;

	class BitProxy
	{
	private:
		Binary * Bin;
		int Index;

	public:
		BitProxy(int index);

		BitProxy(Binary* binary, int index);

		int operator=(int value);

		operator int() const;
	};

	BitProxy operator[](int index);

	const BitProxy operator[](int index) const;
};

