#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    unsigned int x = 10;
    int y = -20;
    bool test1 = (x + (unsigned int)y) == (x + y);
    bool test2 = ((int)x + y) != (x + y);

    int i = 4;
    unsigned int j = -12;
    unsigned int valabs_i = i;
    printf("Valeur de Valbs_i est %u\n", valabs_i);
    if (i <= 0)
    {
        valabs_i = 0 - i;
        printf("Valbs_i est %u\n", valabs_i);
        j = j - valabs_i;
        printf("j = %u\n", j);
    }
    else
    {
        j = (0-j) + valabs_i;
        printf("j2 = %u\n", j);
    }

    printf("(%u+ (unsigned int)%d) = %u\ntest1 = %d\n",x, y, (x + (unsigned int)y), test1);
    printf("%u + %d = %u\ntest2 = %d\n",x, y,  x + y, test2);
    printf("%u + %d)= %u\n\n",x, y, ((int)x + y));
    return 0;
}