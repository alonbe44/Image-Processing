#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 100
#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAGIC_NUMBER 0x6265
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1


int is_magic(int n){
int sum=0;
while(n>0){sum+=n%10;n/=10;}

return sum%10==0;
    
}
int main(int argc, char *argv[]) {
    if(argc==BAD_DIM){
   printf("ERROR: Bad Arguments\n");
        return BAD_ARGS;
    }
     if (argc == BAD_ARGS)
        { // check arg count
        printf("Usage: ebcEcho file1 file2\n");
        return SUCCESS;
        } // check arg count


    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
      printf("ERROR: Bad File Name (%s)\n",argv[1]);
        return BAD_FILE;
    }

//int magic;
    int width = 0, height = 0;
    // and capture fscanfs return to ensure we got 2 values.
 char str[256];
     unsigned char magicNumber[2];
    unsigned short *magicNumberValue = (unsigned short *)magicNumber;

    // get first 2 characters which should be magic number
    magicNumber[0] = getc(fp);
    magicNumber[1] = getc(fp);
    if (!is_magic(*magicNumberValue))
        { // check magic number
        printf("ERROR: Bad Magic Number (%s)\n",argv[1]);
        return BAD_MAGIC_NUMBER;
        } //check magic number


  
                    fgets(str,256,fp);
                    fscanf(fp,"%d",&height);
                    fscanf(fp,"%d",&width);





    if ( height < MIN_DIMENSION || width < MIN_DIMENSION || height > MAX_DIMENSION || width > MAX_DIMENSION)
        { // check dimensions
 fclose(fp);
        printf("ERROR: Bad Dimensions (%s)\n",argv[1]);
        return BAD_DIM;
        } // check dimensions

    long numBytes = height * width;
    unsigned int * imageData = (unsigned int *)malloc(numBytes * sizeof(unsigned int));

    // if malloc is unsuccessful
    if (imageData == NULL)
        { // check malloc
        fclose(fp);
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
        } // check malloc


        fgets(str,256,fp);
        fgets(str,256,fp);


   // read in each value from the file
    for (int current = 0; current < numBytes; current++)
        { // reading in
        int check = fscanf(fp, "%u", &imageData[current]);
        // validate that we have captured 1 pixel value
        if (check != 1)
            { // check inputted data
            // ensure that allocated data is freed before exit.
            free(imageData);
            fclose(fp);
            printf("ERROR: Bad Data (%s)\n",argv[1]);
            return BAD_DATA;
            } // check inputted data

        }


// now we have finished using the inputFile we should close it
    fclose(fp);

    // open the output file in write mode   
          // FILE *outputFile1 = 
          // fopen(argv[2], "w");

    int result = access(argv[2], W_OK); // W_OK is a constant representing write permission

    if (result != 0) {
      printf("ERROR: Bad Output(%s)\n",argv[2]);
        return 8;
    } 

//  if(outputFile1==NULL){
//   free(imageData);
//         printf("ERROR: Bad File Name (%s)\n",argv[2]);
//         return BAD_FILE;
//     }

   // Clean up
    free(imageData);

printf("ECHOED\n");
    return SUCCESS;




    
        
  

}