#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define EBC_MAGIC_NUM 'EC'

#pragma pack(push, 1)
struct ebc_header {
    uint32_t magic_number;
    uint32_t height;
    uint32_t width;
};
#pragma pack(pop)

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        fprintf(stderr, "Error: Failed to open input file %s\n", argv[1]);
        return 1;
    }

    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        fprintf(stderr, "Error: Failed to open output file %s\n", argv[2]);
        fclose(input_file);
        return 1;
    }

    // Read the EBU header
    uint32_t width, height;
    if (fread(&width, sizeof(width), 1, input_file) != 1 ||
        fread(&height, sizeof(height), 1, input_file) != 1) {
        fprintf(stderr, "Error: Failed to read EBU header\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    // Write the EBC header
    struct ebc_header ebc_header = {EBC_MAGIC_NUM, height, width};
    if (fwrite(&ebc_header, sizeof(ebc_header), 1, output_file) != 1) {
        fprintf(stderr, "Error: Failed to write EBC header\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    // Read and convert pixel data
    uint8_t r, g, b;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if (fread(&r, sizeof(r), 1, input_file) != 1 ||
                fread(&g, sizeof(g), 1, input_file) != 1 ||
                fread(&b, sizeof(b), 1, input_file) != 1) {
                fprintf(stderr, "Error: Failed to read pixel data\n");
                fclose(input_file);
                fclose(output_file);
                return 1;
            }
            uint32_t pixel = (r << 16) | (g << 8) | b;
            if (fwrite(&pixel, sizeof(pixel), 1, output_file) != 1) {
                fprintf(stderr, "Error: Failed to write pixel data\n");
                fclose(input_file);
                fclose(output_file);
                return 1;
            }
        }
    }

    // Cleanup and exit
    fclose(input_file);
    fclose(output_file);
    return 0;
}
