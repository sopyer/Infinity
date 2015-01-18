#pragma once

#define EOK             ( 0 )
#define ESNULLP         ( 400 )       /* null ptr                    */
#define ESZEROL         ( 401 )       /* length is zero              */
#define ESLEMAX         ( 403 )       /* length exceeds max          */
#define ESOVRLP         ( 404 )       /* overlap undefined           */
#define ESNOSPC         ( 406 )       /* not enough space for s2     */
#define ESUNTERM        ( 407 )       /* unterminated string         */
#define ESNOTFND        ( 409 )       /* not found                   */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

#if !defined(_WIN64) && !defined(__APPLE__)
typedef unsigned int rsize_t;
#endif

// TODO: add tests

typedef char* cstr;
typedef char  cstr256[256];
typedef char  cstr128[128];
typedef char  cstr1024[1024];

// TODO: Remove later
#define strnlen_s  strnlen_s_core_c
#define strcat_s   strcat_s_core_c
#define strncat_s  strncat_s_core_c
#define strstr_s   strstr_s_core_c
#define strcpy_s   strcpy_s_core_c


errno_t strcat_s(char *dest, rsize_t dmax, const char *src);

errno_t strcpy_s(char *dest, rsize_t dmax, const char *src);

rsize_t strnlen_s(const char *dest, rsize_t dmax);

errno_t strstr_s(char *dest, rsize_t dmax, const char *src, rsize_t slen, char **substring);

#ifdef __cplusplus
}

template<size_t N>
inline errno_t strcat_s(char (&dest)[N], const char* src)
{
    return strcat_s(dest, N, src);
}

template<size_t N>
inline errno_t strcpy_s(char (&dest)[N], const char* src)
{
    return strcpy_s(dest, N, src);
}

template<size_t N>
inline rsize_t strnlen_s(const char (&dest)[N])
{
    return strnlen_s(dest, N);
}

template<size_t N, size_t M>
errno_t strstr_s(char (&dest)[N], char (&src)[M], char **substring)
{
    return strstr_s(dest, N, src, M, substring);
}

#endif