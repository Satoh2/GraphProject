#pragma once

class ANode
{
protected:
	int ID;

public:
	ANode() { ID = 0; }
	ANode(int id) { ID = id; };
	ANode(const ANode &obj) { ID = obj.ID; };

	// Getter for ID
	virtual int GetID() const = 0;

	// Setter for ID
	virtual void SetID(int id) = 0;
};