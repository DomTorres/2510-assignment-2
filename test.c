#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *pmessage = "now is the time"; /* a pointer */

    *(pmessage + 3) = 'A';

    printf("%s\n", pmessage);
}