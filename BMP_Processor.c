/*
 *
 * @author Anne Landrum, ASU
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "DefineBMP.h"
#include "DefinePixel.h"

void readBMPHeader(FILE* file, struct BMP_Header* header) {
    // read bitmap file header(14 bytes)
    fread(&header->signature, sizeof(char)*2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(&header->signature, sizeof(char)*2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

void readDIBHeader(FILE* file, struct DIB_Header* header) {
    // read bitmap DIB header
    fread(&header->headersize, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->planes, sizeof(short), 1, file);
    fread(&header->bpp, sizeof(short), 1, file);
    fread(&header->compression, sizeof(int), 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->xres, sizeof(int), 1, file);
    fread(&header->yres, sizeof(int), 1, file);
    fread(&header->colors, sizeof(int), 1, file);
    fread(&header->importantcolors, sizeof(int), 1, file);
}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->headersize, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bpp, sizeof(short), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->xres, sizeof(int), 1, file);
    fwrite(&header->yres, sizeof(int), 1, file);
    fwrite(&header->colors, sizeof(int), 1, file);
    fwrite(&header->importantcolors, sizeof(int), 1, file);
}

void makeBMPHeader(struct BMP_Header* header, int width, int height) {
        header->size = width*height;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
        header->height = height;
        header->width = width;
}


void headerTest(struct Pixel **pixel, struct BMP_Header* header, struct DIB_Header* DIBheader) {
    printf("signature: %c%c\n", header->signature[0], header->signature[1]);
    printf("size: %d\n", header->size);
    printf("reserved1: %d\n", header->reserved1);
    printf("reserved2: %d\n", header->reserved2);
    printf("offset_pixel_array: %d\n", header->offset_pixel_array);
    
    printf("headersize: %d\n", DIBheader->headersize);
    printf("width: %d\n", DIBheader->width);
    printf("height: %d\n", DIBheader->height);
    printf("planes: %d\n", DIBheader->planes);
    printf("bpp: %d\n", DIBheader->bpp);
    printf("compression: %d\n", DIBheader->compression);
    printf("size: %d\n", DIBheader->size);
    printf("xres: %d\n", DIBheader->xres);
    printf("yres: %d\n", DIBheader->yres);
    printf("colors: %d\n", DIBheader->colors);
    printf("impocolors: %d\n", DIBheader->importantcolors);
    
    for(int i = 0; i < DIBheader->height; i++){
        for(int j = 0; j < DIBheader->width; j++) {
            printf("blue%u\n", pixel[i][j].blue);
            printf("green%u\n", pixel[i][j].green);
            printf("red%u\n", pixel[i][j].red);
        }
    }
}
// END BMP PROCESSOR

//BEGIN PIXELPROCESSOR
/**
 * Shift color of Pixel array. The dimension of the array is width * height. The shift value of RGB is
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct Pixel** pArr, int width, int height, unsigned char rShift, unsigned char gShift, unsigned char bShift) {
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++) {
            pArr[i][j].red = pArr[i][j].red+rShift;
            pArr[i][j].green = pArr[i][j].green+gShift;
            pArr[i][j].blue = pArr[i][j].blue+bShift;
        }
    }
}

int main(int argc, const char * argv[]) {
    const char* input = argv[1];
    int r = atoi(argv[2]);
    int g = atoi(argv[3]);
    int b = atoi(argv[4]);
    FILE* file = fopen(input, "rb");
    FILE* filewrite = fopen("/Users/annelandrum/Documents/Code/Pr_Img/Pr_Img/output.bmp", "wb");
    struct BMP_Header* header = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    struct DIB_Header* DIBheader = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
    readBMPHeader(file, header);
    readDIBHeader(file, DIBheader);
    
    printf("%d\n", DIBheader->width);
    printf("%d", DIBheader->height);
    
    int rowsize = (ceil((DIBheader->bpp*DIBheader->width)/32))*4;
        printf("%d rowsize", rowsize);
    
    struct Pixel **pixel = (struct Pixel**)malloc(sizeof(struct Pixel*)* DIBheader->height);
    for(int i = 0; i < DIBheader->height; i++) {
        pixel[i] = malloc(sizeof(struct Pixel)*(rowsize));
    }
    
    ///headerTest(pixel, header, DIBheader);
    
    fseek(file, 54, SEEK_SET);

    for(int i = 0; i < DIBheader->height; i++){
        for(int j = 0; j < rowsize; j++) {
            fread(&pixel[i][j].red, sizeof(unsigned char), 1, file);
            fread(&pixel[i][j].green, sizeof(unsigned char), 1, file);
            fread(&pixel[i][j].blue, sizeof(unsigned char), 1, file);
        }
    }
    printf("%c\n%c\n%c\n", r, g, b);

    colorShiftPixels(pixel, rowsize, DIBheader->height, r, g, b);
    
    fseek(file, 0, SEEK_SET);
    writeBMPHeader(filewrite, header);
    writeDIBHeader(filewrite, DIBheader);
    fseek(file, 54, SEEK_SET);
    for(int i = 0; i < DIBheader->height; i++){
        for(int j = 0; j < rowsize; j++) {
            fwrite(&pixel[i][j].red, sizeof(unsigned char), 1, filewrite);
            fwrite(&pixel[i][j].green, sizeof(unsigned char), 1, filewrite);
            fwrite(&pixel[i][j].blue, sizeof(unsigned char), 1, filewrite);
        }
    }
    
    
    fclose(file);
    fclose(filewrite);
    
    return 0;
}

 
 



