#pragma once

#include "Master.h"

#include <string>

#ifdef HW_OS_WINDOWS
#include <Windows.h>
#endif

enum class HWFileOperation
{
	Read,
	Write,
	Append,
};

class HWFile
{
public:
	HWFile(const std::string& path, HWFileOperation operation);
	HWFile(const HWFile&);
	~HWFile();

	BOOL Read(U8* out, S32 length);
	std::string Read();
	BOOL Write(const U8* in, S32 length);

	S64 GetLength();

private:
#ifdef HW_OS_WINDOWS
	HANDLE file;
#endif
};

