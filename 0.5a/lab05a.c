#include <stdio.h>
#include <stdlib.h>
#include<string.h>


char* input(){
    char *p = (char*)malloc(1);//выделяем память под \0
    *p = '\0';
    char buf[81];//80 тк длина окна 80 + \0
    int n, len = 0;
    do{
        n = scanf("%80[^\n]", buf);//80 символов до переноса строки
        if(n<0){//если конец ввода (Crtl+D) или ошибка ввода
            free(p);
            p = NULL;
            continue;
            }
        if(n==0)//если не считан ни один символ (в строке только \n)
            scanf("%*c");
        else{
            len += strlen(buf);
            p = (char*)realloc(p, len+1);
            strcat(p, buf);//копируем buf в p
        }
    }while(n>0);
    return p;
}


char* copyWord(char* from, char* to){
    int k = strcspn(from, " \t");// длина слова(до первого пробела)
    strncat(to, from, k); //копирует к элементов в конец to
    to += k;//сдвигаем указатель на конец слова
    *(to++) = ' ';//сначала ставим пробел потом двигаем указатель на один символ вперед
    *to = '\0';//добавляем нуль терминатор
    return to;//возвращяет слово + пробел
}


char* skipSpace(char* p){
    int k = strspn(p, " \t");//возвращяет количество пробелов/табуляций(количестов элементов совпадающих с " \t")
    return p+k;//перемещает указатель на k пробелов вперед
}


char* skipWord(char* p){
    int k = strcspn(p, " \t");//количество символов до пробела
    return p+k;//перемещает указатель на к символов вперед до первого пробела
}


void newStr(char *s, char** p){
    *p = (char*)malloc(strlen(s)+1);
    char* tmp = *p;
    *tmp = '\0';
    int fl=0, i=0;
    while(*(s = skipSpace(s))){//пока s не равно \0
        i++;

        if(i==1){
          tmp = copyWord(s, tmp);
          fl = strcspn(*p," \t");//длина первого слова

        }

        if (strcspn(s, " \t")==fl && i>1){
          tmp = copyWord(s, tmp);
        }

        s = skipWord(s);
    }

    if (*p!=tmp)//удаляем пробел после последнего слова
      *--tmp = '\0';//смещаемся на символ влево, а затем удаляем пробел, заменяя его на \0

    *p = (char*)realloc(*p, strlen(*p)+1);//очищаем лишнюю память

}



int main() {
    char* p=NULL;
    char* s=NULL;

    while (puts("Enter string:"), p = input()){
      printf("\nsource string: %s\n", p);
      newStr(p,&s);
      printf("new string: %s\n\n", s);
      free(p);
      free(s);
  }
    return 0;
}
