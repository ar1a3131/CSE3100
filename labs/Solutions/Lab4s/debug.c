#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct TokenArray{
  char** tokens;
  size_t numTokens;
} TokenArray;

char* dupString(char* s, size_t len){
  char* duplicate = malloc((len + 1)*sizeof(char));
  duplicate[len] = 0;
  for(size_t i = 0; i < len; i++){
    duplicate[i] = s[i];
  }
  return duplicate;
}

int countTokens(char* s,char delim)
{
   int cnt = 0;
   int inToken = *s != delim;
   while (*s) {
      if (*s == delim)
         cnt += inToken;
      inToken = *s != delim;
      s++;
   }
   return cnt + inToken;
}

char* tokenEnd(char* from,char delim)
{
   while (*from && *from != delim) from++;
   return from - 1;
}

char* skipDelim(char* from,char delim)
{
   while(*from && *from == delim) from++;
   return from;
}

TokenArray tokenize(char* string, char delim)
{
  size_t numTok = countTokens(string,delim);
  char** tokens = malloc(sizeof(char*)*numTok);  
  size_t tokenIndex = 0;
  char* ts = string;
  while(tokenIndex < numTok) {
     char* te = tokenEnd(ts,delim);
     if (te - ts + 1 > 0)
        tokens[tokenIndex++] = dupString(ts,te-ts+1);
     ts = skipDelim(te+1,delim);
  }
  TokenArray ta;
  ta.tokens = tokens;
  ta.numTokens = numTok;
  return ta;
}

int main(int argc, char* argv[])
{
  printf("Enter string:\n");
  char* string = NULL;
  size_t n = 0;
  ssize_t lineSize = getline(&string, &n, stdin);
  string[lineSize - 1] = '\0';
  printf("Enter delimiter: ");
  char delim;
  scanf("%c", &delim);

  TokenArray ta = tokenize(string, delim);
  printf("Num tokens: %lu\n", ta.numTokens);
  for(size_t i = 0; i < ta.numTokens; i++){
    printf("token: %s\n", ta.tokens[i]);
    free(ta.tokens[i]);
  }
  free(ta.tokens);
  free(string);
  
  return 0;
}
 
