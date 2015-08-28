#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main () {

    char buffer[5120], ch;
    
      int fIn, fOut, i;
      ssize_t bytes;
          FILE *fp = NULL;
                
       //open a file
             fIn = open ("linea_svg.svg", O_RDONLY);
                    if (fIn == -1) {
                        printf("\nfailed to open file.");
                       return 1;
                              }
                                            
       //read from file
        bytes =  read (fIn, buffer, sizeof(buffer));
         //and close it
      close (fIn);
return 0;
}
