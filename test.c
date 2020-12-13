#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *join1(char *c, char *b) {
    
    
    printf("Work in join1\n");
    char *m = malloc(sizeof(char)*(strlen(c) + strlen(b)));
    printf("a\n");
    printf("%s\n",c);
    printf("b\n");
    printf("%s\n",b);
    /*
    if (c == NULL) exit (1);
    char *tempc = c;
    for(int j = 0; j < strlen(a);j++){
        *c++ = *a++;
    }
    for(int j = 0 ; j < strlen(b);j++){
        *c++ = *b++;
    }
    */
    return m;
}

static char *AppendFunc( char *src, char match[], char Append[],int size)
{
    /* Define the pointers */
    char *srcp = src;
    //char *matchp = match;
    char *Appendp = Append;
    // If we can not find match within the string then return NULL//
    
    if(srcp == NULL){
        printf("Source Input is NULL\n");
        return NULL;
    }
    if(Appendp == NULL){
        printf("Match Input is NULL\n");
        return NULL;
    }
    
    char *mid = strstr(src,match);
    //printf("src\n");
   // printf("%s\n",srcp);
   // printf("mid\n");
  //  printf("%s\n",mid);
   
    if(mid == NULL){
   //     printf("No more matching \n");
        /* Return the src to append in the futuer */
        return src;
    }
    //printf("GET\n");
    //printf("%s\n",mid);
    //printf("%s\n",srcp);
    char *temarray =malloc(sizeof(char)*(mid -srcp)+1);
    
    //printf("new array\n");
    for(int j = 0; j < mid-src; j++){
        //printf("%c",*(srcp+j));
        *(temarray+j) = *(srcp+j);
    }
    //printf("\n");
    //printf("tem\n");
    //printf("%s\n",temarray);
    strncat(temarray,Appendp,12);
    //printf("tem0\n");
    //printf("%s\n",temarray);
    strncat(temarray,match,4);
    //printf("GET\n");
    //printf("tem1\n");
    //printf("%s\n",temarray);
    mid = strstr(src,match);
    //printf("%c\n",*(mid+4));
    if(*(mid+4) == '\0'){
        printf("Work here!\n");
        printf("Return %s \n",temarray);
        return temarray;
    }else{
        mid = mid +4;
        printf("Input for joint1!\n");
        printf("%s\n",temarray);
        src = join1(temarray,AppendFunc(mid,match,Append,size+12));
    }
    printf("Return src\n");
    printf("%s\n",src);
    return src;
}

int main(void)
{
    char src[] ="http://www.letv.com/ptv/vplay/24607840.jpg-http://www.letv.com/ptv/vplay/24607840.jpg";
    char ingredients[] = "-126.128.138";
    char match[] = ".jpg";
    //char match0[] = ".png";
    char *srcp = src;
    int size = sizeof(src);
    /* Recursively change the URL within the HTML file */
    char *result = AppendFunc(srcp,match,ingredients,size);
    //char *result0 =AppendFunc(result,match,ingredients,size);
    printf("result\n");
    printf("%s\n",result);
    return 0;    
}



