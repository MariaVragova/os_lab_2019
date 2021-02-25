#include "revert_string.h"
#include <stdio.h>
#include <string.h>

void RevertString(char *str)
{
    // your code here
    if (str == 0 || *str == 0)
    {
        return;
    }

    char* start = str;
    char* end = start + strlen(str) - 1;
    char temp;

    while (end > start)
    {
        temp = *start;
        *start = *end;
        *end = temp;

        ++start;
        --end;
    }
}

