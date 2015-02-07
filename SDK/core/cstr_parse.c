/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

static inline int char_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static inline int char_isdigit(char c)
{
    return '0'<=c && c<='9';
}

#define READ_NEXT ((s<e) ? *s++ : (++s, 0))
#define LOOKUP(N) (((s+(N))<e) ? *(s+N) : 0)

static int maxExponent = 511;   /* Largest possible base 10 exponent.  Any
                                 * exponent larger than this will already
                                 * produce underflow or overflow, so there's
                                 * no need to worry about additional digits.
                                 */
static double powersOf10[] = {  /* Table giving binary powers of 10.  Entry */
    10.,                        /* is 10^2^i.  Used to convert decimal */
    100.,                       /* exponents into floating-point numbers. */
    1.0e4,
    1.0e8,
    1.0e16,
    1.0e32,
    1.0e64,
    1.0e128,
    1.0e256
};

/*
 *----------------------------------------------------------------------
 *
 * cstr_todouble --
 *
 *  This procedure converts a floating-point number from an ASCII
 *  decimal representation to internal double-precision format.
 *
 * Results:
 *  The return value is the double-precision floating-point
 *  representation of the characters in string.  If endPtr isn't
 *  NULL, then *endPtr is filled in with the address of the
 *  next character after the last one that was part of the
 *  floating-point number.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

/* string - A decimal ASCII floating-point number,
*           optionally preceded by white space.
*           Must have form "-I.FE-X", where I is the
*           integer part of the mantissa, F is the
*           fractional part of the mantissa, and X
*           is the exponent.  Either of the signs
*           may be "+", "-", or omitted.  Either I
*           or F may be omitted, or both.  The decimal
*           point isn't necessary unless F is present.
*           The "E" may actually be an "e".  E and X
*           may both be omitted (but not just one).
*/
errno_t cstr_todouble(const char* str, rsize_t size, const char** end, double* res)
{
    //TODO: use better algorithm from FreeBSD vfscanf helper function parsefloat 
    int sign, expSign = FALSE, any = TRUE;
    double fraction, dblExp, *d;
    const char *s;
    const char *e;
    int c;
    int exp = 0;        /* Exponent read from "EX" field. */
    int fracExp = 0;    /* Exponent that derives from the fractional
                         * part.  Under normal circumstatnces, it is
                         * the negative of the number of digits in F.
                         * However, if I is very long, the last digits
                         * of I get dropped (otherwise a long I with a
                         * large negative exponent could cause an
                         * unnecessary overflow on I alone).  In this
                         * case, fracExp is incremented one for each
                         * dropped digit. */
    int mantSize;       /* Number of digits in mantissa. */
    int decPt;          /* Number of mantissa digits BEFORE decimal
                         * point. */
    const char *pExp;   /* Temporarily holds location of exponent
                         * in string. */
    errno_t err;

    /*
     * Strip off leading blanks and check for a sign.
     */

    s = str;
    e = str+size;

    do {
        c = READ_NEXT;
    } while (char_isspace(c));
    if (c == '-') {
        sign = TRUE;
        c = READ_NEXT;
    } else {
        if (c == '+') {
            c = READ_NEXT;
        }
        sign = FALSE;
    }

    /*
     * Count the number of digits in the mantissa (including the decimal
     * point), and also locate the decimal point.
     */

    decPt = -1;
    for (mantSize = 0;; mantSize += 1)
    {
        if (!char_isdigit(c)) {
            if ((c != '.') || (decPt >= 0)) {
                break;
            }
            decPt = mantSize;
        }
        c = READ_NEXT;
    }

    /*
     * Now suck up the digits in the mantissa.  Use two integers to
     * collect 9 digits each (this is faster than using floating-point).
     * If the mantissa has more than 18 digits, ignore the extras, since
     * they can't affect the value anyway.
     */

    --s;
    pExp = s;
    s -= mantSize;
    if (decPt < 0) {
        decPt = mantSize;
    } else {
        mantSize -= 1;          /* One of the digits was the point. */
    }
    if (mantSize > 18) {
        fracExp = decPt - 18;
        mantSize = 18;
    } else {
        fracExp = decPt - mantSize;
    }
    if (mantSize == 0) {
        fraction = 0.0;
        err = EINVAL;
        any = 0;
        goto done;
    } else {
        int frac1, frac2;
        frac1 = 0;
        for (; mantSize > 9; mantSize -= 1)
        {
            c = READ_NEXT;
            if (c == '.') {
                c = READ_NEXT;
            }
            frac1 = 10 * frac1 + (c - '0');
        }
        frac2 = 0;
        for (; mantSize > 0; mantSize -= 1)
        {
            c = READ_NEXT;
            if (c == '.') {
                c = READ_NEXT;
            }
            frac2 = 10 * frac2 + (c - '0');
        }
        fraction = (1.0e9 * frac1) + frac2;
    }

    /*
     * Skim off the exponent.
     */

    s = pExp;
    c = READ_NEXT;
    if ((c == 'E') || (c == 'e')) {
        c = READ_NEXT;
        if (c == '-') {
            expSign = TRUE;
            c = READ_NEXT;
        } else {
            if (c == '+') {
                c = READ_NEXT;
            }
            expSign = FALSE;
        }
        while (char_isdigit(c)) {
            exp = exp * 10 + (c - '0');
            c = READ_NEXT;
        }
    }
    if (expSign) {
        exp = fracExp - exp;
    } else {
        exp = fracExp + exp;
    }

    /*
     * Generate a floating-point number that represents the exponent.
     * Do this by processing the exponent one bit at a time to combine
     * many powers of 2 of 10. Then combine the exponent with the
     * fraction.
     */
    err = EOK;
    if (exp < 0) {
        expSign = TRUE;
        exp = -exp;
    } else {
        expSign = FALSE;
    }
    if (exp > maxExponent) {
        exp = maxExponent;
        err = ERANGE;
    }
    dblExp = 1.0;
    for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
        if (exp & 01) {
            dblExp *= *d;
        }
    }
    if (expSign) {
        fraction /= dblExp;
    } else {
        fraction *= dblExp;
    }
done:
    if (sign) {
        fraction = -fraction;
    }

    if (res) *res = fraction;
    if (end) *end = any ? s - 1 : str;

    return err;
}

/*
 * Convert a string to an intmax_t integer.
 *
 * Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
errno_t cstr_toimax(const char * str, rsize_t size, int base, const char** end, intmax_t* res)
{
    //TODO: use better algorithm from FreeBSD vfscanf helper function parseint 
    const char* s;
    const char* e;
    uintmax_t acc;
    char c, cs, cs1;
    uintmax_t cutoff;
    int neg, any, cutlim;
    errno_t err;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    s = str;
    e = s + size;
    do {
        c = READ_NEXT;
    } while (char_isspace(c));
    if (c == '-') {
        neg = 1;
        c = READ_NEXT;
    } else {
        neg = 0;
        if (c == '+')
            c = READ_NEXT;
    }

    cs = LOOKUP(0);
    cs1 = LOOKUP(1);
    if ((base == 0 || base == 16) &&
        c == '0' && (cs == 'x' || cs == 'X') &&
        ((cs1 >= '0' && cs1 <= '9') ||
        (cs1 >= 'A' && cs1 <= 'F') ||
        (cs1 >= 'a' && cs1 <= 'f'))) {
        c = cs1;
        s += 2;
        base = 16;
    } else if ((base == 0 || base == 2) &&
        c == '0' && (cs == 'b' || cs == 'B') &&
        (cs1 == '0' || cs1 == '1')) {
        c = cs1;
        s += 2;
        base = 2;
    } else if (base == 0)
        base = c == '0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for intmax_t is
     * [-9223372036854775808..9223372036854775807] and the input base
     * is 10, cutoff will be set to 922337203685477580 and cutlim to
     * either 7 (neg==0) or 8 (neg==1), meaning that if we have
     * accumulated a value > 922337203685477580, or equal but the
     * next digit is > 7 (or 8), the number is too big, and we will
     * return a range error.
     *
     * Set 'any' if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    cutoff = neg ? (uintmax_t)-(INTMAX_MIN + INTMAX_MAX) + INTMAX_MAX : INTMAX_MAX;
    cutlim = cutoff % base;
    cutoff /= base;
    for (;; c = READ_NEXT) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }

    err = EOK;
    if (any < 0) {
        acc = neg ? INTMAX_MIN : INTMAX_MAX;
        err = ERANGE;
    } else if (!any) {
    noconv:
        err = EINVAL;
    } else if (neg)
        acc = (uintmax_t)(-(intmax_t)acc);

    if (res) *res = acc;
    if (end) *end = (char *)(any ? s - 1 : str);

    return (err);
}

/*
 * Convert a string to a uintmax_t integer.
 *
 * Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
errno_t cstr_toumax(const char * str, rsize_t size, int base, const char** end, uintmax_t* res)
{
    //TODO: use better algorithm from FreeBSD vfscanf helper function parseint 
    const char* s;
    const char* e;
    uintmax_t acc;
    char c, cs, cs1;
    uintmax_t cutoff;
    int neg, any, cutlim;
    errno_t err;

    /*
     * See strtoimax for comments as to the logic used.
     */
    s = str;
    e = s + size;
    do {
        c = READ_NEXT;
    } while (char_isspace(c));
    if (c == '-') {
        neg = 1;
        c = READ_NEXT;
    } else {
        neg = 0;
        if (c == '+')
            c = READ_NEXT;
    }

    cs = LOOKUP(0);
    cs1 = LOOKUP(1);
    if ((base == 0 || base == 16) &&
        c == '0' && (cs == 'x' || cs == 'X') &&
        ((cs1 >= '0' && cs1 <= '9') ||
        (cs1 >= 'A' && cs1 <= 'F') ||
        (cs1 >= 'a' && cs1 <= 'f'))) {
        c = cs1;
        s += 2;
        base = 16;
    } else if ((base == 0 || base == 2) &&
        c == '0' && (cs == 'b' || cs == 'B') &&
        (cs1 == '0' || cs1 == '1')) {
        c = cs1;
        s += 2;
        base = 2;
    } else if (base == 0)
        base = c == '0' ? 8 : 10;
    acc = any = 0;
    if (base < 2 || base > 36)
        goto noconv;

    cutoff = UINTMAX_MAX / base;
    cutlim = UINTMAX_MAX % base;
    for (;; c = READ_NEXT) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }

    err = EOK;
    if (any < 0) {
        acc = UINTMAX_MAX;
        err = ERANGE;
    } else if (!any) {
    noconv:
        err = EINVAL;
    } else if (neg)
        acc = (uintmax_t)(-(intmax_t)acc);

    if (res) *res = acc;
    if (end) *end = (char *)(any ? s - 1 : str);

    return (err);
}

int cstr_scanf(const char *inbuf, rsize_t bsize, const char *fmt, ...)
{
    va_list ap;
    int ret;
    
    va_start(ap, fmt);
    ret = cstr_vscanf(inbuf, bsize, fmt, ap);
    va_end(ap);
    return(ret);
}

/*
 * Flags used during conversion.
 */
#define LONG        0x01    /* l: long or double */
#define SHORT       0x04    /* h: short */
#define SUPPRESS    0x08    /* *: suppress assignment */
#define POINTER     0x10    /* p: void * (as hex) */
#define NOSKIP      0x20    /* [ or c: do not skip blanks */
#define LONGLONG    0x400   /* ll: long long (+ deprecated q: quad) */
#define INTMAXT     0x800   /* j: intmax_t */
#define PTRDIFFT    0x1000  /* t: ptrdiff_t */
#define SIZET       0x2000  /* z: size_t */
#define SHORTSHORT  0x4000  /* hh: char */
#define UNSIGNED    0x8000  /* %[oupxX] conversions */

/*
 * The following are used in integral conversions only:
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS
 */
#define SIGNOK      0x40    /* +/- is (still) legal */
#define NDIGITS     0x80    /* no digits detected */
#define PFXOK       0x100   /* 0x prefix is (still) legal */
#define NZDIGITS    0x200   /* no zero digits detected */
#define HAVESIGN    0x10000 /* sign detected */

/*
 * Conversion types.
 */
#define CT_CHAR     0   /* %c conversion */
#define CT_CCL      1   /* %[...] conversion */
#define CT_STRING   2   /* %s conversion */
#define CT_INT      3   /* %[dioupxX] conversion */
#define CT_FLOAT    4   /* %[efgEFG] conversion */

static const char* __sccl(char *tab, const char* fmt, rsize_t inr);

int cstr_vscanf(const char *inp, rsize_t size, char const *fmt0, va_list ap)
{
    rsize_t inr;
    const char *fmt = fmt0;
    int c;              /* character from format, or conversion */
    rsize_t width;       /* field width, or 0 */
    int flags;          /* flags as defined above */
    int nassigned;      /* number of fields assigned */
    int nconversions;   /* number of conversions */
    int nr;             /* characters read by the current conversion */
    int nread;          /* number of characters consumed from fp */
    int base;           /* base argument to conversion function */
    char ccltab[256];   /* character class table for %[...] */



    inr = cstr_len(inp, size);

    nassigned = 0;
    nconversions = 0;
    nread = 0;
    base = 0;       /* XXX just to keep gcc happy */
    for (;;) {
        c = *fmt++;
        if (c == 0)
            return (nassigned);
        if (char_isspace(c)) {
            while (inr > 0 && char_isspace(*inp))
                nread++, inr--, inp++;
            continue;
        }
        if (c != '%')
            goto literal;
        width = 0;
        flags = 0;
        /*
         * switch on the format.  continue if done;
         * break once format type is derived.
         */
    again:
        c = *fmt++;
        switch (c) {
        case '%':
        literal :
            if (inr <= 0)
                goto input_failure;
            if (*inp != c)
                goto match_failure;
            inr--, inp++;
            nread++;
            continue;

        case '*':
            flags |= SUPPRESS;
            goto again;
        case 'j':
            flags |= INTMAXT;
            goto again;
        case 'l':
            if (flags & LONG) {
                flags &= ~LONG;
                flags |= LONGLONG;
            } else
                flags |= LONG;
            goto again;
        case 'q':
            flags |= LONGLONG;  /* not quite */
            goto again;
        case 't':
            flags |= PTRDIFFT;
            goto again;
        case 'z':
            flags |= SIZET;
            goto again;
        case 'h':
            if (flags & SHORT) {
                flags &= ~SHORT;
                flags |= SHORTSHORT;
            } else
                flags |= SHORT;
            goto again;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            width = width * 10 + c - '0';
            goto again;

            /*
             * Conversions.
             */
        case 'd':
            c = CT_INT;
            base = 10;
            break;

        case 'i':
            c = CT_INT;
            base = 0;
            break;

        case 'o':
            c = CT_INT;
            flags |= UNSIGNED;
            base = 8;
            break;

        case 'u':
            c = CT_INT;
            flags |= UNSIGNED;
            base = 10;
            break;

        case 'X':
        case 'x':
            flags |= PFXOK; /* enable 0x prefixing */
            c = CT_INT;
            flags |= UNSIGNED;
            base = 16;
            break;

#ifndef NO_FLOATING_POINT
        case 'A': case 'E': case 'F': case 'G':
        case 'a': case 'e': case 'f': case 'g':
            c = CT_FLOAT;
            break;
#endif

        case 's':
            c = CT_STRING;
            break;

        case '[':
            fmt = __sccl(ccltab, fmt, inr);
            flags |= NOSKIP;
            c = CT_CCL;
            break;

        case 'c':
            flags |= NOSKIP;
            c = CT_CHAR;
            break;

        case 'p':   /* pointer format is like hex */
            flags |= POINTER | PFXOK;
            c = CT_INT;         /* assumes sizeof(uintmax_t) */
            flags |= UNSIGNED;  /*      >= sizeof(uintptr_t) */
            base = 16;
            break;

        case 'n':
            if (flags & SUPPRESS)   /* ??? */
                continue;
            if (flags & SHORTSHORT)
                *va_arg(ap, char *) = nread;
            else if (flags & SHORT)
                *va_arg(ap, short *) = nread;
            else if (flags & LONG)
                *va_arg(ap, long *) = nread;
            else if (flags & LONGLONG)
                *va_arg(ap, long long *) = nread;
            else if (flags & INTMAXT)
                *va_arg(ap, intmax_t *) = nread;
            else if (flags & SIZET)
                *va_arg(ap, size_t *) = nread;
            else if (flags & PTRDIFFT)
                *va_arg(ap, ptrdiff_t *) = nread;
            else
                *va_arg(ap, int *) = nread;
            continue;

        default:
            goto match_failure;
        }

        /*
         * We have a conversion that requires input.
         */
        if (inr <= 0)
            goto input_failure;

        /*
         * Consume leading white space, except for formats
         * that suppress this.
         */
        if ((flags & NOSKIP) == 0) {
            while (char_isspace(*inp)) {
                nread++;
                if (--inr > 0)
                    inp++;
                else
                    goto input_failure;
            }
            /*
             * Note that there is at least one character in
             * the buffer, so conversions that do not set NOSKIP
             * can no longer result in an input failure.
             */
        }

        /*
         * Do the conversion.
         */
        switch (c) {

        case CT_CHAR:
            /* scan arbitrary characters (sets NOSKIP) */
            if (width == 0)
                width = 1;
            else
                width = width < inr ? width : inr;
            if (width == 0)
                goto input_failure;
            if ((flags & SUPPRESS) == 0) {
                mem_copy(va_arg(ap, char *), inp, width);
            }
            inr -= width;
            inp += width;
            nr   = width;
            break;

        case CT_CCL:
            /* scan a (nonempty) character class (sets NOSKIP) */
            if (width == 0)
                width = (size_t)~0;	/* `infinity' */
            /* take only those things in the class */
            nr = 0;
            if (flags & SUPPRESS) {
                while (ccltab[*inp]) {
                    nr++, inr--, inp++;
                    if (--width == 0)
                        break;
                    if (inr <= 0)
                        break;
                }
            }
            else {
                char* p = va_arg(ap, char *);
                rsize_t sz = va_arg(ap, rsize_t);
                while (ccltab[*inp]) {
                    if (sz-- > 1)
                        *p++ = *inp;
                    inr--;
                    inp++;
                    nr++;
                    if (--width == 0)
                        break;
                    if (inr <= 0)
                        break;
                }
                if (sz) *p = 0;
            }
            if (nr == 0)
                goto match_failure;
            break;

        case CT_STRING:
            if (width == 0)
                width = ~(size_t)0; /* `infinity' */
            /* like CCL, but zero-length string OK, & no NOSKIP */
            nr = 0;
            if (flags & SUPPRESS) {
                while (!char_isspace(*inp)) {
                    nr++, inr--, inp++;
                    if (--width == 0)
                        break;
                    if (inr <= 0)
                        break;
                }
            } else {
                char* p = va_arg(ap, char *);
                rsize_t sz = va_arg(ap, rsize_t);
                while (!char_isspace(*inp)) {
                    if (sz-- > 1)
                        *p++ = *inp;
                    inr--;
                    inp++;
                    nr++;
                    if (--width == 0)
                        break;
                    if (inr <= 0)
                        break;
                }
                if (sz) *p = 0;
            }
            break;

        case CT_INT: {
            uintmax_t res;
            const char* end;
            if (width == 0)
                width = inr;
            else
                width = width < inr ? width : inr;
            if ((flags & UNSIGNED) == 0)
                cstr_toimax(inp, width, base, &end, (intmax_t*)&res);
            else
                cstr_toumax(inp, width, base, &end, &res);
            nr = end - inp;
            inp = end; inr -= nr;
            if (nr == 0)
                goto match_failure;
            if ((flags & SUPPRESS) == 0) {
                if (flags & POINTER)
                    *va_arg(ap, void **) = (void *)(uintptr_t)res;
                else if (flags & SHORTSHORT)
                    *va_arg(ap, char *) = (char)res;
                else if (flags & SHORT)
                    *va_arg(ap, short *) = (short)res;
                else if (flags & LONG)
                    *va_arg(ap, long *) = (long)res;
                else if (flags & LONGLONG)
                    *va_arg(ap, long long *) = (long long)res;
                else if (flags & INTMAXT)
                    *va_arg(ap, intmax_t *) = (intmax_t)res;
                else if (flags & PTRDIFFT)
                    *va_arg(ap, ptrdiff_t *) = (ptrdiff_t)res;
                else if (flags & SIZET)
                    *va_arg(ap, size_t *) = (size_t)res;
                else
                    *va_arg(ap, int *) = (int)res;
            }
        } break;

#ifndef NO_FLOATING_POINT
        case CT_FLOAT: {
            double res;
            const char* end;
            if (width == 0)
                width = inr;
            else
                width = width < inr ? width : inr;
            cstr_todouble(inp, width, &end, &res);
            nr = end - inp;
            inp = end; inr -= nr;
            if (nr == 0)
                goto match_failure;
            if ((flags & SUPPRESS) == 0) {
                if (flags & LONG) {
                    *va_arg(ap, double *) = res;
                } else {
                    *va_arg(ap, float *) = (float)res;
                }
            }
        } break;
#endif /* !NO_FLOATING_POINT */
        }
        if (!(flags & SUPPRESS))
            nassigned++;
        nread += nr;
        nconversions++;
    }
input_failure:
    return (nconversions != 0 ? nassigned : -1);
match_failure:
    return (nassigned);
}

/*
 * Fill in the given table from the scanset at the given format
 * (just after `[').  Return a pointer to the character past the
 * closing `]'.  The table has a 1 wherever characters should be
 * considered part of the scanset.
 */
static const char* __sccl(char *tab, const char* fmt, rsize_t size)
{
    int c, n, v;
    const char* s;
    const char* e;

    s = fmt;
    e = s + size;
    /* first `clear' the whole table */
    c = READ_NEXT;     /* first char hat => negated scanset */
    if (c == '^') {
        v = 1;      /* default => accept */
        c = READ_NEXT; /* get new first char */
    } else
        v = 0;      /* default => reject */

    /* XXX: Will not work if sizeof(tab*) > sizeof(char) */
    (void)mem_set(tab, 256, v);

    if (c == 0)
        return (fmt - 1);/* format ended before closing ] */

    /*
     * Now set the entries corresponding to the actual scanset
     * to the opposite of the above.
     *
     * The first character may be ']' (or '-') without being special;
     * the last character may be '-'.
     */
    v = 1 - v;
    for (;;) {
        tab[c] = v;     /* take character c */
    doswitch:
        n = READ_NEXT;     /* and examine the next */
        switch (n) {

        case 0:         /* format ended too soon */
            return (fmt - 1);

        case '-':
            /*
             * A scanset of the form
             *  [01+-]
             * is defined as `the digit 0, the digit 1,
             * the character +, the character -', but
             * the effect of a scanset such as
             *  [a-zA-Z0-9]
             * is implementation defined.  The V7 Unix
             * scanf treats `a-z' as `the letters a through
             * z', but treats `a-a' as `the letter a, the
             * character -, and the letter a'.
             *
             * For compatibility, the `-' is not considerd
             * to define a range if the character following
             * it is either a close bracket (required by ANSI)
             * or is not numerically greater than the character
             * we just stored in the table (c).
             */
            n = LOOKUP(0);
            if (n == ']' || n < c) {
                c = '-';
                break;  /* resume the for(;;) */
            }
            fmt++;
            /* fill in the range */
            do {
                tab[++c] = v;
            } while (c < n);
            c = n;
            /*
             * Alas, the V7 Unix scanf also treats formats
             * such as [a-c-e] as `the letters a through e'.
             * This too is permitted by the standard....
             */
            goto doswitch;
            break;

        case ']':       /* end of scanset */
            return (fmt);

        default:        /* just another character */
            c = n;
            break;
        }
    }
    /* NOTREACHED */
}

#undef READ_NEXT
#undef LOOKUP
#undef LONG
#undef SHORT
#undef SUPPRESS
#undef POINTER
#undef NOSKIP
#undef LONGLONG
#undef INTMAXT
#undef PTRDIFFT
#undef SIZET
#undef SHORTSHORT
#undef UNSIGNED
#undef SIGNOK
#undef NDIGITS
#undef PFXOK
#undef NZDIGITS
#undef HAVESIGN
#undef CT_CHAR
#undef CT_CCL
#undef CT_STRING
#undef CT_INT
#undef CT_FLOAT
