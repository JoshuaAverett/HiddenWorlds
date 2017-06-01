#include "HWFile.h"

HWFile::HWFile(const std::string& path, HWFileOperation operation)
{
#ifdef HW_OS_WINDOWS
	std::wstring wide_path(path.length(), L' ');
	std::copy(path.begin(), path.end(), wide_path.begin());

	DWORD access, share, disposition;
	switch(operation)
	{
	case HWFileOperation::Read:
		access = FILE_GENERIC_READ;
		share = FILE_SHARE_READ;
		disposition = OPEN_EXISTING;
		break;
	case HWFileOperation::Append:
		access = FILE_GENERIC_WRITE;
		share = 0;
		disposition = OPEN_ALWAYS;
		break;
	case HWFileOperation::Write:
		access = FILE_GENERIC_WRITE;
		share = 0;
		disposition = OPEN_ALWAYS;
		break;
	default:
		return;
	}

	// Move this to another method
	file = CreateFile(wide_path.c_str(), access, share, nullptr, disposition, FILE_ATTRIBUTE_NORMAL, nullptr);
#endif
}

HWFile::~HWFile()
{
#ifdef HW_OS_WINDOWS
	CloseHandle(file);
#endif
}

BOOL HWFile::Read(U8* out, S32 length)
{
#ifdef HW_OS_WINDOWS
	return ReadFile(file, out, length, nullptr, nullptr);
#endif
}

std::string HWFile::Read()
{
#ifdef HW_OS_WINDOWS
	S64 length = GetLength();
	U8* data = new U8[length];
	Read(data, length);

	return std::string(reinterpret_cast<char*>(data), length);
#endif
}

BOOL HWFile::Write(const U8* in, S32 length)
{
#ifdef HW_OS_WINDOWS
	return false;
#endif
}

S64 HWFile::GetLength()
{
#ifdef HW_OS_WINDOWS
	S64 length = 0;
	GetFileSizeEx(file, (LARGE_INTEGER*)&length);
	return length;
#endif
}
