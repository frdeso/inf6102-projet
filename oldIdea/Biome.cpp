#include "Biome.h"


Biome::Biome(int type, unsigned int length)
{
	this->SetType(type);
	this->SetLength(length);
}

int Biome::GetType() const
{
	return this->type_;
}

void Biome::SetType(int type)
{
	this->type_ = type;
}

unsigned Biome::GetLength() const
{
	return length_;
}

void Biome::SetLength(unsigned int length)
{
	this->length_= length;
}
