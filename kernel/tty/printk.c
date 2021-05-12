/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * printk function
 */

#include "printk.h"

/* Don't let anyone see this... */
static char printk_buf[1024];

/*
 * Written by Lukas Chmela
 * Released under GPLv3
 */
static char *itoa(int value, char *result, int base) {
	// Check that base is valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp;

	do {
		tmp = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp < 0) *ptr++ = '-';
	*ptr-- = '\0';

	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

int printk(const char *fmt, ...)
{
	char *buf;
	/*
	 * Hack to make gcc point to the first parameter 
	 * instead of ignoring it
	 */
	va_list args = (va_list) &fmt;
	int err = vsprintf(&buf, args);
	tty_puts(buf, VGA_COLOR_WHITE);
	va_end(args);

	return err;
}

/*
 * Not too bad, right?
 */
int vsprintf(char **buf, va_list args)
{
	const char *fmt = va_arg(args, char *);
	char c;
	for (int i = 0; (c = *fmt++);) {
		printk_buf[i++] = c;
		if (c == '%') {
			c = *fmt++;
			char *str = "";
			switch (c) {
				/* Strings */
				case 's':
					str = va_arg(args, char *);
					break;
				/* Integers */
				case 'i':
					itoa(va_arg(args, int), str, 10);
					break;
				/* Unknown type */
				default:
					*buf = "";
					return SIGERR;
			}
			/* Skip '%' sign */
			i--;
			/* Copy string to buffer */
			for (; (c = *str++); i++) {
				printk_buf[i] = c;
			}
		}
	}

	*buf = printk_buf;
	return SIGOK;
}
