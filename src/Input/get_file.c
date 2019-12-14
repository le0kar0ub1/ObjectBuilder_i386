#include "builtin.h"
#include "ElfHelper.h"
#include "sys/stat.h"

uint8 UNBREAKER = 0x0; // used to unbreak a line when special character occured

#define UNBREAKER_SMP 0x1
#define UNBREAKER_DBL 0x2

#define COMMENT_CHAR ';'

bool IS_A_SEPARATOR(char c)
{
    if (c == 0x20 || c == 0x9 || c == ',')
        return (true);
    return (false);
}

bool IS_A_UNBREAKER(char c)
{
    if (c == '\'' && UNBREAKER != UNBREAKER_DBL) {
        if (UNBREAKER)
            UNBREAKER = 0x0;
        else
            UNBREAKER = UNBREAKER_SMP;
        return (true);
    }
    if (c == '\"' && UNBREAKER != UNBREAKER_SMP) {
        if (UNBREAKER)
            UNBREAKER = 0x0;
        else
            UNBREAKER = UNBREAKER_DBL;
        return (true);
    }
    return (false);
}

char SpecialCase(char c)
{
    /* get special char from string */
    switch (c) {
        case '0':
            return (0x7F);
        case 'a':
            return (0x7);
        case 'b':
            return (0x8);
        case 't':
            return (0x9);
        case 'n':
            return (0xA);
        case 'v':
            return (0xB);
        case 'f':
            return (0xC);
        case 'r':
            return (0xD);
        case '\'':
            return ('\'');
        case '\"':
            return ('\"');
        case '\\':
            return ('\\');
        default:
            return (c);
    }
}

uint next_case(char *buf)
{
    uint i = 0x0;

    for (; buf[i] && buf[i] != 0xA && IS_A_SEPARATOR(buf[i]); i++);
    for (; buf[i] && buf[i] != 0xA && !IS_A_SEPARATOR(buf[i]); i++) {
        if (buf[i] == '\'')
            for (i++; buf[i] && buf[i] != '\'' && buf[i] != 0xA; i++);
        else if (buf[i] == '\"')
            for (i++; buf[i] && buf[i] != '\"' && buf[i] != 0xA; i++);
    }
    return (i);
}

uint next_line_space(char *buf)
{
    uint sp = 0x0;

    for (; *buf && *buf != 0xA; buf++)
        if (IS_A_SEPARATOR(*buf)) {
            for (; *buf && *buf != 0xA && IS_A_SEPARATOR(*buf); buf++);
            sp++;
        }
    return (sp);
}

bool CommentAndSpace(char *s)
{
    if (*s == COMMENT_CHAR)
        return (true);
    for (;*s && (*s == 0x9 || *s == 0x20); s++);
    if (*s == 0x0 || *s == 0xA)
        return (true);
    return (false);
}

bool LineAsComment(char *s)
{
    for (; *s && *s != 0xA; s++)
        if (*s == COMMENT_CHAR)
            return (true);
    return (false);
}

char **line_by_line(char **buff)
{
    uint idx = 0x0, i = 0x0;

    if (LineAsComment(*buff)) {
        for (; **buff && **buff != 0xA; *(buff) += 1);
        return (NULL);
    }
    char **ret = malloc((next_line_space(*buff) + 0x6) * 0x8);
    if (ret == NULL || (ret[idx] = malloc(next_case(*buff) + 0x5)) == NULL)
        pexit("Malloc Failed\n");
    for (; **buff && **buff != 0xA; *(buff) += 1, i++) {
        if (CommentAndSpace(*buff)) {
            for (; **buff && **buff != 0xA; *(buff) += 1);
            break;
        }
        if (IS_A_SEPARATOR(**buff) && UNBREAKER == 0x0) {
            for (; **buff && IS_A_SEPARATOR(**buff); *(buff) += 1);
            ret[idx][i] = 0x0;
            idx++;
            i = -1;
            if ((ret[idx] = malloc(next_case(*buff) + 0x5)) == NULL)
                pexit("Malloc Failed\n");
            *buff -= 0x1;
        } else {
            IS_A_UNBREAKER(**buff);
            if (UNBREAKER != 0x0 && **buff == '\\') {
                *buff += 0x1;
                ret[idx][i] = SpecialCase(**buff);
            } else
                ret[idx][i] = **buff;
        }
    }
    if (**buff)
        *buff += 1;
    ret[idx][i] = 0x0;
    ret[idx + 1] = NULL;
    return (ret);
}

char ***get_file(int rd, char *file)
{
    struct stat s;

    if (stat(file, &s) == -1)
        pexit("Stat error\n");
    char *f = malloc(s.st_size + 0x10);
    if (f == NULL)
        pexit("Malloc Failed\n");
    char *fs = f;
    for (; read(rd, f, 0x1); f++);
    *f = 0x0;
    close(rd);

    char ***ret = malloc((nbr_char_str(fs, 0xA) * 0x8) + 0x20);
    if (ret == NULL)
        pexit("Malloc Failed\n");
    uint idx = 0x0;
    for (; *fs && (*fs == 0x20 || *fs == 0x9 || *fs == 0xA); fs++);
    for (; *fs; idx++) {
        ret[idx] = line_by_line(&fs);
        if (ret[idx] == NULL)
            idx--;
        for (; *fs && (*fs == 0x20 || *fs == 0x9 || *fs == 0xA); fs++);
    }
    ret[idx] = NULL;
    return (ret);
}
