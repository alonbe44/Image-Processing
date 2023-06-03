#include <stdio.h>
#include <stdlib.h>

#define EBC_MAGIC_NUMBER "EC"
#define EBC_HEADER_SIZE 6

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file.ebc> <output_file.ebu>\n", argv[0]);
        return 1;
    }

    char* input_file = argv[1];
    char* output_file = argv[2];

    // Open the input file for reading
    FILE* input_fp = fopen(input_file, "rb");
    if (input_fp == NULL) {
        printf("Error: Failed to open input file: %s\n", input_file);
        return 1;
    }

    // Check the magic number
    char magic_number[3];
    fread(magic_number, 1, 2, input_fp);
    magic_number[2] = '\0';
    if (strcmp(magic_number, EBC_MAGIC_NUMBER) != 0) {
        printf("Error: Invalid magic number\n");
        fclose(input_fp);
        return 1;
    }

    // Read the height and width of the image
    int height, width;
    fread(&height, 4, 1, input_fp);
    fread(&width, 4, 1, input_fp);

    // Calculate the size of the compressed image data
    int compressed_data_size = (height * width) / 8;

    // Allocate memory for the compressed image data
    unsigned char* compressed_data = (unsigned char*)malloc(compressed_data_size);
    if (compressed_data == NULL) {
        printf("Error: Failed to allocate memory\n");
        fclose(input_fp);
        return 1;
    }

    // Read the compressed image data
    fread(compressed_data, 1, compressed_data_size, input_fp);

    // Close the input file
    fclose(input_fp);

    // Open the output file for writing
    FILE* output_fp = fopen(output_file, "wb");
    if (output_fp == NULL) {
        printf("Error: Failed to open output file: %s\n", output_file);
        free(compressed_data);
        return 1;
    }

    // Write the EBU header
    fprintf(output_fp, "EBU\n%d %d\n", height, width);

    // Allocate memory for the uncompressed image data
    unsigned char* uncompressed_data = (unsigned char*)malloc(height * width);
    if (uncompressed_data == NULL) {
        printf("Error: Failed to allocate memory\n");
        fclose(output_fp);
        free(compressed_data);
        return 1;
    }

    // Decompress the image data
    int i, j, k;
    k = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            unsigned char value = (compressed_data[k / 8] >> (7 - k % 8)) & 1;
            uncompressed_data[i * width + j] = value * 255;
            k++;
        }
    }

    // Write the uncompressed image data to the output file
    fwrite(uncompressed_data, 1, height * width, output_fp);

    // Close the output file
    fclose(output_fp);

    // Free the memory
    free(compressed_data);
    free(uncompressed_data);

    printf("Success: Converted %s to %s\n", input_file, output_file);

    return 0;
}
