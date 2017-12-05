// bmptoc.c
// Name: Kiran Rao

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Your program will only support BMP files that
// have a max size that is equivalent to the size
// of the GBA screen.
#define MAX_BMP_FILE_SIZE (0x36 + 240 * 160 * 4)
unsigned char data_arr[MAX_BMP_FILE_SIZE];

//Function prototypes
unsigned int getWidth(unsigned char*);
unsigned int getHeight(unsigned char*);

int main(int argc, char *argv[]) {
    // The implementation of the program is broken down into steps below.

    //TODO
    // 1. Make sure the user passed in the correct number of arguments
    // Hint: You'll want to see what value argc has.
    if (argc != 2) {
        printf("Incorrect number of arguments passed.");
        return 0;
    }



    FILE *infile;
    //TODO
    // 2. Open the file. If it doesn't exist, tell the user and then exit
    // Hint: read the man page for fopen by typing `man fopen`
    //       either in your terminal or google
    infile = fopen(argv[1], "r");
    if (infile == NULL) {
        printf("The file does not exist.");
        return 0;
    }

    // We've implemented this part for you. It reads in the bmp file into
    // a buffer (assuming you've open the file properly), then closes the file.
    fread(data_arr, 1, MAX_BMP_FILE_SIZE, infile);
    if(ferror(infile)) {
       printf("An error occurred when reading the file. Quitting.");
       fclose(infile);
       exit(1);
    }
    fclose(infile);

    // 3. Get the width and height of the image. Read the BMP file format
    // to determine what you should do for this one. Then implement the
    // functions at the end of the file.
    unsigned int width = getWidth(data_arr);
    unsigned int height = getHeight(data_arr);

    // We've implemented outputing the header file correctly assuming your
    // getWidth and getHeight work properly. Read this code carefully and
    // read the man pages because you'll have to do something similar
    // for generating the C file.
    FILE *outheader;
    char filename[100];

    strcpy(filename, argv[1]);
    int fname_len = strlen(filename);
    filename[fname_len - 3] = 'h';
    filename[fname_len - 2] = '\0'; // file.bmp -> file.h
    if(!(outheader = fopen(filename, "w"))) {
        printf("Error creating file %s: %s\n", filename, strerror(errno));
    }
    filename[fname_len - 4] = '\0';
    char *ptr = filename;
    while(*ptr) {
        *ptr = toupper(*ptr); // file.h -> FILE
        ++ptr;
    }

    /* File format:
     *
     *  #define FILE_WIDTH 100
     *  #define FILE_HEIGHT 150
     *  const unsigned short file_data[15000];
     */

    fprintf(outheader, "#define %s_WIDTH %d\n", filename, width);
    fprintf(outheader, "#define %s_HEIGHT %d\n", filename, height);

    strcpy(filename, argv[1]);
    filename[fname_len - 4] = '\0';
    fprintf(outheader, "const unsigned short %s_data[%d];\n", filename, width * height);

    fclose(outheader);


    //TODO
    // 4. Now you are responsible for creating a c file with the same basename
    // as the bmp file that was passed in file.bmp -> file.c
    // Hint: Use the code above as a guide for how to
    //       initialize the filename and  open the file.
    FILE *outcfile;
    char cfilename[100];

    strcpy(cfilename, argv[1]);
    int cfname_len = strlen(cfilename);
    cfilename[cfname_len - 3] = 'c';
    cfilename[cfname_len - 2] = '\0'; // file.bmp -> file.c
    if(!(outcfile = fopen(cfilename, "w"))) {
        printf("Error creating file %s: %s\n", cfilename, strerror(errno));
    }


    /* File format:
     *
     *  const unsigned short file_data[15000] = {
     *    0x4521, 0x5B2F, 0x1F69, ...
     *  };
     */

    // 5. We've implemented the first part of the C file which will look like the
    // line above. Note that instead of 15000 we compute width * height.
    //

    fprintf(outcfile, "const unsigned short %s_data[%d] = {\n", filename, width * height);

    //TODO
    // 6. Iterate over the pixel data convert it to hex and write the output to the C file.
    // Hint: Pay careful attention to the format of the BMP file. The rows are stored
    // in reverse order. We've structured your loop so that if you print the pixels in
    // the order they are iterated over it will be correct.

    // Useful variables:
    // data_arr - array of bytes that contains all the data in the bmp file
    // height/width - self explanatory, just so you're aware they're there

    for (int r = height - 1; r >= 0; r--) {
        for (unsigned int c = 0; c < width; c++) {
            // CODE GOES HERE
            unsigned int gbaAll;
            unsigned short gbaValRed;
            unsigned short gbaValGreen;
            unsigned short gbaValBlue;
            unsigned short storeVal;
            gbaAll = *(unsigned int*)&data_arr[0x36 + (r*width*4) + (c*4)]; // 0xAArrggbb -> little endianness
            gbaValRed = (unsigned short)(((gbaAll & 0x00FF0000)>>16)>>3); // 0x00rr
            gbaValGreen = (unsigned short)(((gbaAll & 0x0000FF00)>>8)>>3); // 0x00gg
            gbaValBlue = (unsigned short)((gbaAll & 0x000000FF)>>3); // 0x00bb
            storeVal = (unsigned short)(gbaValRed + (gbaValGreen << 5) + (gbaValBlue << 10));
            fprintf(outcfile, "0x%04x, ", storeVal);
        }
        // SOME OTHER CODE GOES HERE
        fprintf(outcfile, "\n");
    }
    // Print the closing brace in the array
    fprintf(outcfile, "};");
    fclose(outcfile);


    // You're done with this part, but don't forget there are other files
    // you need to implement code in.

    return 0;
}

/*
 *
 * Format of a bmp header (data_arr)
 * file offset  |   length   |     field
 * 0x12         |  4 bytes   |  image width
 * 0x16         |  4 bytes   |  image height
 *
 */

unsigned int getWidth(unsigned char *data_arr) {
    //TODO FIX THIS
    //The '&' gets the address of the value of data_arr[0x12], 
    //which is then turned into an int pointer to group multiple addresses. 
    //It is then converted to a value again using the last dereference
    unsigned int width = *(int*)&data_arr[0x12];
    //printf("%u\n", width);
    return width;
}

unsigned int getHeight(unsigned char *data_arr) {
    //TODO FIX THIS
    unsigned int height = *(int*)&data_arr[0x16];
    //printf("%u\n", height);
    return height;
}
