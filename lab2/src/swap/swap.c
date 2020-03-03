#include "swap.h"

void swap(char *left, char *right)
{char temp = *left;
*left=*right;
*right=temp;
}
