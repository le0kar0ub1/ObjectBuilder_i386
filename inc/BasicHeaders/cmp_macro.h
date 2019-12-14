#ifndef CMP_H_
#define CMP_H_

#include "stdbool.h"
#include "lib.h"

static inline char *TOLOWER(char *s)
{
    for (uint i = 0x0; s[i]; i++)
        if (s[i] <= 0x5A && s[i] >= 0x41)
            s[i] = s[i] + 0x20;
    return (s);
}

static inline bool is_num(char c)
{
    if (c <= 0x39 && c >= 0x30)
        return (true);
    return (false);
}

static inline bool sis_num(char *s)
{
    for (; *s && is_num(*s); s++);
    if (*s == 0x0)
        return (true);
    return (false);
}

static inline bool is_alpha(char c)
{
    if ((( c <= 0x5A && c >= 0x41) ||
(c <= 0x7A && c >= 0x61))
|| c == 0x5f || is_num(c))
        return (true);
    return (false);
}

static inline bool sis_alpha(char *s)
{
    for (; *s && is_alpha(*s); s++);
    if (*s == 0x0)
        return (true);
    return (false);
}

static inline bool cmp(int x, int y)
{
    if (x != y)
        return (false);
    return (true);
}

static inline long absl(long x)
{
    return (x *= (x < 0x0) ? -0x1 : 0x1);
}

static inline void pexit(char const *s)
{
    if (s)
        write(2, s, len(s));
    exit(84);
}

# define sprint(x) my_printf("%s\n", x)

# define nprint(x) my_printf("%d\n", x)

# define tab(x) my_put_tab(x)

# define retl my_putchar(0xA)

# define space my_putchar(0x20)

# define endl(x) (x != 0xA && x != 0x0)

# define loop(x) for (int i = 0x0; x[i]; i++)

# define sloop(x, y) for (int i = 0x0; x[i] && x[i] != y; i++);

#endif
