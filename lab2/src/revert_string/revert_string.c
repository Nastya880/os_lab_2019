#include "revert_string.h"
#include<stdio.h>
#include<string.h>

void RevertString(char *str){
    register int i, len = 0;
    //Определяем длину строки
    len = strlen(str);
    //Выполняем реверс символов в строке
    for (i = 0; i <= len / 2; i++){
        *(str + len - i) = *(str + i);  
        *(str + i) = *(str + len - i - 1);
    } 

    //Сдвигаем вторую половину символов влево на 1 символ
    for (i = len / 2; i <= len; i++)
        *(str + i) = *(str + i + 1);
        
    //Устанавливаем символ завершения строки
    *(str + len) = '\0';
}
