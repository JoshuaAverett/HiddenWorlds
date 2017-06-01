#pragma once

#include "Master.h"

class HWDiffieHellman
{
public:
	static void Multiply(U8* q, U8* e, const U8* p);
	static void MultiplyBase(U8* q, U8* n);
};
