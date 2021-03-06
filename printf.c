/*-
 * Copyright (c) 2005, Kohsuke Ohtani
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This file is modified from printf.c of prex's bootloader
 */

#include <stddef.h>
#include <stdarg.h>

#include "reg.h"

#define PL011_BASE  0x20201000
#define PL011_DR    (PL011_BASE + 0x00)
#define PL011_FR    (PL011_BASE + 0x18)

void _putc(unsigned char c)
{
    while( (_REG(PL011_FR) & 0x80) == 0);
    _REG(PL011_DR) = c;
}

void printf(const char *fmt, ...)
{
    static const char digits[] = "0123456789abcdef";
    va_list ap;
    char buf[10];
    char *s;
    unsigned u;
    int c, i, pad;

    va_start(ap, fmt);
    while ((c = *fmt++) != 0) {
        if (c == '%') {
            c = *fmt++;
            /* ignore long */
            if (c == 'l')
                c = *fmt++;
            switch (c) {
            case 'c':
                _putc(va_arg(ap, int));
                continue;
            case 's':
                s = va_arg(ap, char *);
                if (s == NULL)
                    s = "<NULL>";
                for (; *s; s++) {
                    _putc((int)*s);
                }
                continue;
            case 'd':
                c = 'u';
            case 'u':
            case 'x':
                u = va_arg(ap, unsigned);
                s = buf;
                if (c == 'u') {
                    do
                        *s++ = digits[u % 10U];
                    while (u /= 10U);
                } else {
                    pad = 0;
                    for (i = 0; i < 8; i++) {
                        if (pad)
                            *s++ = '0';
                        else {
                            *s++ = digits[u % 16U];
                            if ((u /= 16U) == 0)
                                pad = 1;
                        }
                    }
                }
                while (--s >= buf)
                    _putc((int)*s);
                continue;
            }
        }
        if (c == '\n')
            _putc('\r');
        _putc((int)c);
    }
    va_end(ap);
}
