#include <cstdio>
#include <cstdlib>
#include "program.h"

int main(int argc, char* argv[])
{
   if (argc < 4)
   {
      fprintf(stderr, "USAGE: %s <inc file> <c file> <executable>\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   FILE* f = fopen(argv[1], "rt");
   if (f == NULL)
   {
      fprintf(stderr, "File \"%s\" not found\n", argv[1]);
      exit(EXIT_FAILURE);
   }
   TokenStream ts(f);
   fclose(f);
   Program p(ts);
   f = fopen(argv[2], "wt");
   if (f == NULL)
   {
      fprintf(stderr, "Unable to open file \"%s\"\n", argv[2]);
   }
   p.write(f);
   fclose(f);

   char buffer[1024];
   sprintf(buffer, "gcc --ansi --pedantic -Wall -Werror -g -lpthread -o %s %s",
           argv[3], argv[2]);
   system(buffer);

   return 0;
}
