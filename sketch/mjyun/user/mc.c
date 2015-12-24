
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "gpio.h"

#include "noduino/esp8266_peri.h"
#include "driver/uart.h"
#include "mjyun.h"
#include "limits.h"

int ICACHE_FLASH_ATTR isalnum(int c)
{
	if (isalpha(c) || isdigit(c)) {
		return 1;
	}
	return 0;
}

int ICACHE_FLASH_ATTR islower(int c)
{
	if (c >= 'a' && c <= 'z') {
		return 1;
	}
	return 0;
}

int ICACHE_FLASH_ATTR isalpha(int c)
{
	if (islower(c) || isupper(c)) {
		return 1;
	}
	return 0;
}

int ICACHE_FLASH_ATTR isspace(int c)
{
	switch (c) {
	case 0x20:		// ' '
	case 0x09:		// '\t'
	case 0x0a:		// '\n'
	case 0x0b:		// '\v'
	case 0x0c:		// '\f'
	case 0x0d:		// '\r'
		return 1;
	}
	return 0;
}

int ICACHE_FLASH_ATTR isupper(int c)
{
	if (c >= 'A' && c <= 'Z') {
		return 1;
	}
	return 0;
}


long ICACHE_FLASH_ATTR strtol(const char *nptr, char **endptr, int base)
{
	const unsigned char *s = (const unsigned char *)nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
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
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *)(any ? (char *)s - 1 : nptr);
	return (acc);
}

long ICACHE_FLASH_ATTR atol(const char *s)
{
    char *tmp;
    return strtol(s, &tmp, 10);
}

int ICACHE_FLASH_ATTR atoi(const char *s)
{
        return (int)atol(s);
}


char *ICACHE_FLASH_ATTR strchr(const char *str, int character)
{
	while (1) {
		if (*str == 0x00) {
			return NULL;
		}
		if (*str == (char)character) {
			return (char *)str;
		}
		str++;
	}
}

char *ICACHE_FLASH_ATTR strncat(char *dest, const char *src, size_t n)
{
	size_t i;
	size_t offset = strlen(dest);
	for (i = 0; i < n && src[i]; i++) {
		dest[i + offset] = src[i];
	}
	dest[i + offset] = 0;
	return dest;
}

char *ICACHE_FLASH_ATTR strcat(char *dest, const char *src)
{
	return strncat(dest, src, strlen(src));
}
