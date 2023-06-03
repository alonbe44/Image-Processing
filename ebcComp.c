#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Open the input file for reading
    FILE *input_fp = fopen(argv[1], "rb");
    if (input_fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read the header
    char magic[2] = "EB";
    int height, width;
    fread(&height, sizeof(int), 1, input_fp);
    fread(&width, sizeof(int), 1, input_fp);

    // Open the output file for writing
    FILE *output_fp = fopen(argv[2], "wb");
    if (output_fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write the header to the output file
    fwrite(&magic, sizeof(char), 2, output_fp);
    fwrite(&height, sizeof(int), 1, output_fp);
    fwrite(&width, sizeof(int), 1, output_fp);

    // Compress and write the image data to the output file
    int num_pixels = height * width;
    char *data = malloc(num_pixels * sizeof(char));
    fread(data, sizeof(char), num_pixels, input_fp);
    for (int i = 0; i < num_pixels; i += 9) {
        int sum = 0;
        for (int j = 0; j < 9 && i+j < num_pixels; j++) {
            sum += data[i+j];
        }
        char avg = (char) (sum / 9);
        fwrite(&avg, sizeof(char), 1, output_fp);
    }

    // Close the input and output files and free the memory
    fclose(input_fp);
    fclose(output_fp);
    free(data);

    return 0;
}
