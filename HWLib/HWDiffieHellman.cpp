#include "HWDiffieHellman.h"
#include <string>

void HWDiffieHellman::MultiplyBase(U8* q, U8* n)
{
	static const U8 base[] = { 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	crypto_scalarmult(q, n, base);
}

void HWDiffieHellman::Multiply(U8* q, U8* e, const U8* p)
{
}