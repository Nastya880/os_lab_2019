#include "find_min_max.h"

#include <limits.h>
#include "find_min_max.h"
//#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
{
    struct MinMax min_max;
    int i;
    //int begin;
    //int end;
    {
    if ((begin) == (end)) // Если есть только один элемент, вернуть как min, так и max одновременно 
        {
        min_max.max = array[0];
        min_max.min = array[0];
        return min_max;
        }
    if  (array[0] > array[1])//Если имеется более одного элемента, инициализировать min и макс 
        {
        min_max.max = array[0];
        min_max.min = array[1];
        }
    else
        {
        min_max.max = array[1];
        min_max.min = array[0];
        }
    
    for (i = begin; i<end; i++)
    {
        if (array[i] > min_max.max)
            min_max.max = array[i];
        else if (array[i] < min_max.min)
            min_max.min = array[i];
    }
return min_max;
    }
}
}
