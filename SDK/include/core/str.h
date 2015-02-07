#pragma once

// Main idea behind API - every string is NULL terminated
// and repsented as pointer to storage and storage size.

#include <errno.h>

#define EOK             ( 0 )

//#define EINVAL          ( 22 )
//#define ERANGE          ( 34 )
//#define EILSEQ          ( 42 )
//#define STRUNCATE       ( 80 )

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

// TODO: add tests

typedef char* cstr;
typedef char  cstr256[256];
typedef char  cstr128[128];
typedef char  cstr1024[1024];

errno_t cstr_concat(char *dest, rsize_t dmax, const char *src);
errno_t cstr_concatn(char *dest, rsize_t dmax, const char *src, rsize_t slen);
errno_t cstr_copy(char *dest, rsize_t dmax, const char *src);
rsize_t cstr_len(const char *dest, rsize_t dmax);
errno_t cstr_substr(char *dest, rsize_t dmax, const char *src, rsize_t slen, char **substring);
errno_t cstr_compare(const char *str1, rsize_t s1max, const char *str2, rsize_t s2max, int *indicator);

// NOTE: "less safe" version of str_compare, user should ensure valid n;
errno_t cstr_comparen(const char *str1, const char *str2, rsize_t cmax, int *indicator);

errno_t cstr_toimax(const char * str, rsize_t size, int base, const char** end, intmax_t* res);
errno_t cstr_toumax(const char * str, rsize_t size, int base, const char** end, uintmax_t* res);
errno_t cstr_todouble(const char* str, rsize_t size, const char** end, double* res);

errno_t cstr_tokenize(
    const char* delims, rsize_t delim_size,
    const char** str,   rsize_t* size,
    const char** token, rsize_t* token_size
);

int cstr_scanf(const char *inbuf, rsize_t bsize, const char *fmt, ...);
int cstr_vscanf(const char *inbuf, rsize_t bsize, char const *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template<size_t N>
inline errno_t cstr_concat(char (&dest)[N], const char* src)
{
    return cstr_concat(dest, N, src);
}

template<size_t N>
inline errno_t cstr_copy(char (&dest)[N], const char* src)
{
    return cstr_copy(dest, N, src);
}

template<size_t N>
inline rsize_t cstr_len(const char (&dest)[N])
{
    return cstr_len(dest, N);
}

template<size_t N, size_t M>
errno_t cstr_substr(char (&dst)[N], char (&src)[M], char **substring)
{
    return cstr_substr(dst, N, src, M, substring);
}

template<size_t N>
errno_t cstr_compare(const char (&str1)[N], const char* str2, rsize_t M, int* indicator)
{
    return cstr_compare(str1, N, str2, M, indicator);
}

template<size_t M>
errno_t cstr_compare(const char* str1, rsize_t N, const char (&str2)[M], int* indicator)
{
    return cstr_compare(str1, N, str2, M, indicator);
}

template<size_t N, size_t M>
errno_t cstr_compare(const char (&str1)[N], const char (&str2)[M], int* indicator)
{
    return cstr_compare(str1, N, str2, M, indicator);
}

template<size_t N>
errno_t cstr_comparen(const char (&str1)[N], const char* str2, int* indicator)
{
    return cstr_comparen(str1, str2, N, indicator);
}

template<size_t N>
errno_t cstr_comparen(const char* str1, const char (&str2)[N], int* indicator)
{
    return cstr_comparen(str1, str2, N, indicator);
}

template<size_t N>
int cstr_scanf(const char (&inbuf)[N], const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = cstr_vscanf(inbuf, N, fmt, ap);
    va_end(ap);

    return ret;
}

template<size_t N>
int cstr_vscanf(const char (&inbuf)[N], const char *fmt, va_list ap)
{
    return cstr_vscanf(inbuf, N, fmt, ap);
}

template<size_t N>
errno_t cstr_tokenize(
    const char (&delims)[N],
    const char** str,   rsize_t* size,
    const char** token, rsize_t* token_size
)
{
    return cstr_tokenize(delims, N, str, size, token, token_size);
}

#endif