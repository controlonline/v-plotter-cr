#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <math.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    int c;
    char *input = argv[1];
    FILE *input_file;

    char TextLine[10000];
    long xMin = 1000000, xMax = -1000000;
    long yMin = 1000000, yMax = -1000000;
    long coordinateCount = 0;
    char a;
    int ReadState = 0;
    long xNow = 0, yNow = 0;
    long xNow1 = 0, yNow1 = 0;
    long xNow2 = 0, yNow2 = 0;
    char *pEnd;
    long i;

    input_file = fopen(input, "r");

    if (input_file == 0)
    {
        //fopen returns 0, the NULL pointer, on failure
        perror("Canot open input file\n");
        exit(-1);
    }

while(!(feof(input_file)) ){

fread(&a, 1, 1, input_file);
long i=0;
TextLine[0] = '\0';
while(!(feof(input_file)) && a !=' ' && a != '<' && a != '>' && a != '\"' && a != '=' && a != ',' && a != ':' && a != 10){
TextLine[i] = a;
TextLine[i+1] = '\0';
i++;
fread(&a, 1, 1, input_file);
}
if(a == '<'){//Init
ReadState = 0;
//printf("ReadState=0");
}
if(strcmp(TextLine, "path") == 0){
ReadState = 1;//path found
//printf("ReadState=1==path found");
}
if(ReadState == 1 && strcmp(TextLine, "fill") == 0){
//ReadState = 2;//fill found
ReadState = 3;//paths without line are also considered when calculating max values
//printf("ReadState=1->3=fill found");
}
if(ReadState == 2 && strcmp(TextLine, "none") == 0){
ReadState = 3;//none found
}
if(ReadState == 2 && strcmp(TextLine, "stroke") == 0){
ReadState = 0;//stroke found, fill isn't "none"
}
if(ReadState == 3 && strcmp(TextLine, "d") == 0 && a == '='){
//printf("ReadState=1(path)->3(fill)->4(d) found");
ReadState = 4;//d= found
}
if(ReadState == 4 && strcmp(TextLine, "M") == 0 && a == ' '){
printf("ReadState=1(path)->3(fill)->4(d)->5(M) found \n");
ReadState = 5;//M found
}

if(ReadState == 5 && strcmp(TextLine, "C") == 0 && a == ' '){
ReadState = 5;//C found
//printf("ReadState=1(path)->3(fill)->4(d)->5(M)->5(C) found");
}

if(ReadState == 6){//Y value
yNow = strtol(TextLine, &pEnd, 10);
//printf("String='%s' y=%ld\n", TextLine, yNow);
if(yNow > yMax){
yMax = yNow;
}
if(yNow < yMin){
yMin = yNow;
}
ReadState = 7;
coordinateCount++;
}
if(ReadState == 5 && a == ','){//X value
xNow = strtol(TextLine, &pEnd, 10);
if(xNow > xMax){
xMax = xNow;
}
if(xNow < xMin){
xMin = xNow;
}
ReadState = 6;
}
if(ReadState == 7){
printf("Found coordinates %ld, %ld\n", xNow, yNow);
ReadState = 5;
}
//printf("ReadState=% 3d, xNow=% 10ld, xMin=% 10ld, xMax=% 10ld, yMin=% 10ld, yMax=% 10ld   ", ReadState, xNow, xMin, xMax, yMin, yMax);

}//while(!(feof(input_file)) && stopPlot == 0){
fclose(input_file);

}
