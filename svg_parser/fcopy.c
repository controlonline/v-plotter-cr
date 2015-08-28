#include <stdio.h>

int main (int argc, char **argv) {

          FILE* sourceFile;
          FILE* destFile;
          char buf[100];
          int numBytes;

          if(argc!=3)
          {
            printf("fcopy origen desti\n");
            return 1;
         }

sourceFile = fopen(argv[1], "rb");
destFile= fopen(argv[2], "wb");

if(sourceFile==NULL)
{
   printf("No existeix el fitxer d'origen\n");
   return  2;
   }

if(destFile==NULL)
{
   printf("No existeix el fitxer desti\n");
   return  3;
   }

//while(numBytes=fread(buf, 1, 50, sourceFile))
//   {
       //fwrite(buf, 1, numBytes, destFile);
//   }

while (feof(sourceFile) == 0)
  {
      fgets(buf,100,sourceFile);
      printf("%s",buf);
            }
   fclose(sourceFile);
   fclose(destFile);
   return 0;
}
