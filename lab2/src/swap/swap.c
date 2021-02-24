#include "swap.h"

void Swap(char *left, char *right)
{
	// ваш код здесь
    char ptemp = *left; //a
	*left = *right;
	*right = ptemp;

}
