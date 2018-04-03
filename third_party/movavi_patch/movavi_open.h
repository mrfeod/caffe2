#ifndef MOVAVI_OPEN
#define MOVAVI_OPEN

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include <stddef.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#pragma warning( push )
#pragma warning( disable : 4996 )
#else
#include <unistd.h>
#include <fcntl.h>
#endif

static int ConvertUtf8ToWide(const char * const str, wchar_t ** dest)
{
	if (!str)
		return 1;

	int srcLen = strlen(str) + 1;
	*dest = (wchar_t *)malloc(srcLen * sizeof(wchar_t));
	if (!*dest) return 0;
	if (!MultiByteToWideChar(CP_UTF8, 0, str, -1, *dest, srcLen))
		return 0;

	return 1;
}

static int ConvertWideToUtf8(const wchar_t * const str, char ** dest)
{
	if (!str)
		return 1;

	int srcLen = wcslen(str) + 1;
	*dest = (char *)malloc(srcLen * sizeof(char));
	if (!*dest)
		return 0;

	if (!WideCharToMultiByte(CP_UTF8, 0, str, -1, *dest, srcLen, 0, 0))
		return 0;

	return 1;
}

static FILE* movavi_fopen(const char *filename, const char *mode)
{
	wchar_t *wfilename, *wmode;
	if (!ConvertUtf8ToWide(filename, &wfilename))
		return 0;
	if (!ConvertUtf8ToWide(mode, &wmode))
		return 0;

	FILE* res = _wfopen(wfilename, wmode);
	free(wfilename);
	free(wmode);
	return res;

}

static int movavi_open(const char *filename, int flag, ...)
{
	int mode = 0;

	if ((flag & O_CREAT)) {
		va_list ap;
		va_start(ap, flag);
		mode = va_arg(ap, int);
		va_end(ap);
	}

	wchar_t *wfilename;
	if (!ConvertUtf8ToWide(filename, &wfilename))
		return 0;

	int res =  _wopen(wfilename, flag, mode);
	free(wfilename);
	return res;
}

#define open movavi_open
#define fopen movavi_fopen

#endif /* MOVAVI_OPEN */
