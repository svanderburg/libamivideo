/*
 * Copyright (c) 2012 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "bitplane.h"
#include <stdlib.h>
#include "viewportmode.h"

#define TRUE 1
#define FALSE 0

#define MAX_NUM_OF_BITPLANES 8

static unsigned int calculateScanLineSize(const unsigned int screenWidth)
{
    unsigned int scanLineSizeInWords = screenWidth / 16;
    
    if(screenWidth % 16 != 0)
	scanLineSizeInWords++;

    return (scanLineSizeInWords * 2);
}

amiVideo_UByte *amiVideo_bitplaneMemoryToChunky(amiVideo_UByte **bitplanePointers, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int screenPitch, const unsigned int bitplaneDepth)
{
    unsigned char *result = (amiVideo_UByte*)calloc(screenPitch * screenHeight, sizeof(amiVideo_UByte)); /* Fill with 0 bytes */
    unsigned int scanLineSize = calculateScanLineSize(screenWidth);
    unsigned int i;
    
    for(i = 0; i < bitplaneDepth; i++) /* Iterate over each bitplane */
    {
	unsigned int count = 0;
	amiVideo_UByte indexBit = 1 << i;
	amiVideo_UByte *bitplanes = bitplanePointers[i];
	unsigned int vOffset = 0;
	unsigned int j;
	
	for(j = 0; j < screenHeight; j++) /* Iterate over each scan line */
	{
	    unsigned int hOffset = vOffset;
	    unsigned int k;
	    unsigned int pixelCount = 0;
	    
	    for(k = 0; k < scanLineSize; k++) /* Iterate over each byte containing 8 pixels */
	    {
		amiVideo_UByte bitplane = bitplanes[hOffset];
		unsigned char bitmask = 0x80;
		unsigned int l;
		
		for(l = 0; l < 8; l++) /* Iterate over each bit representing a pixel */
		{
		    if(pixelCount < screenWidth) /* We must skip the padding bits. If we have already converted sufficient pixels on this scanline, ignore the rest */
		    {
			if(bitplane & bitmask)
			    result[count] |= indexBit;
			
			count++;
		    }
		    
		    pixelCount++;
		    bitmask >>= 1;
		}
		
		hOffset++;
	    }
	    
	    count += screenPitch - screenWidth; /* Skip the padding bytes in the output */
	    
	    vOffset += scanLineSize;
	}
    }
    
    return result;
}

amiVideo_UByte *amiVideo_bitplanesToChunky(amiVideo_UByte *bitplanes, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int screenPitch, const unsigned int bitplaneDepth)
{
    amiVideo_UByte *bitplanePointers[MAX_NUM_OF_BITPLANES];
    unsigned int i;
    unsigned int offset = 0;
    unsigned int bitplaneSize = calculateScanLineSize(screenWidth) * screenHeight;
    
    /* Set bitplane pointers */
    
    for(i = 0; i < bitplaneDepth; i++)
    {
	bitplanePointers[i] = bitplanes + offset;
	offset += bitplaneSize;
    }
    
    /* Convert bitplanes to chunky pixels */
    return amiVideo_bitplaneMemoryToChunky(bitplanePointers, screenWidth, screenHeight, screenPitch, bitplaneDepth);
}

amiVideo_OutputColor *amiVideo_bitplanesToRGB(amiVideo_UByte *bitplanes, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int screenPitch, const unsigned int bitplaneDepth, const amiVideo_Color *color, const unsigned int colorLength, const unsigned int numOfColorBits, const amiVideo_Long viewportMode)
{
    amiVideo_UByte *bytes = amiVideo_bitplanesToChunky(bitplanes, screenWidth, screenHeight, screenPitch, bitplaneDepth);
    unsigned int paletteSize;
    amiVideo_OutputColor *palette = amiVideo_computePalette(color, colorLength, numOfColorBits, viewportMode, &paletteSize);
    amiVideo_OutputColor *result = (amiVideo_OutputColor*)malloc(screenPitch * screenHeight * sizeof(amiVideo_OutputColor));
    unsigned int i;
    
    if(amiVideo_checkHoldAndModify(viewportMode))
    {
	unsigned int offset = 0;
	
	/* HAM mode */
	
	for(i = 0; i < screenHeight; i++)
	{
	    unsigned int j;
	    amiVideo_OutputColor previousResult = palette[0];
	    
	    for(j = 0; j < screenPitch; j++)
	    {
		amiVideo_UByte byte = bytes[offset + j];
		amiVideo_UByte mode = (byte & (0x3 << (bitplaneDepth - 2))) >> (bitplaneDepth - 2);
		amiVideo_UByte index = byte & ~(0x3 << (bitplaneDepth - 2));
	    
		if(mode == 0x0) /* Data bits are an index in the color palette */
		    result[offset + j] = palette[index];
		else if(mode == 0x1) /* Data bits are blue level */
		{
		    result[offset + j] = previousResult;
		    result[offset + j].b = index << (8 - bitplaneDepth + 2);
		}
		else if(mode == 0x2) /* Data bits are red level */
		{
		    result[offset + j] = previousResult;
		    result[offset + j].r = index << (8 - bitplaneDepth + 2);
		}
		else if(mode == 0x3) /* Data bits are green level */
		{
		    result[offset + j] = previousResult;
		    result[offset + j].g = index << (8 - bitplaneDepth + 2);
		}
		
		previousResult = result[offset + j];
	    }
	    
	    offset += screenPitch;
	}
    }
    else
    {
	/* Normal mode */
	for(i = 0; i < screenPitch * screenHeight; i++)
	    result[i] = palette[bytes[i]];
    }
    
    free(bytes);
    free(palette);
    
    return result;
}

void amiVideo_chunkyToBitplaneMemory(amiVideo_UByte **bitplanePointers, const amiVideo_UByte *pixels, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int bitplaneDepth)
{
    unsigned int i;
    unsigned int bitplaneIndex = 0;
    int bit = 7;
    
    for(i = 0; i < screenWidth * screenHeight; i++)
    {
	unsigned int j;
	amiVideo_UByte bitmask = 1 << bit;
	
	for(j = 0; j < bitplaneDepth; j++)
	{
	    if(pixels[i] & (1 << j)) /* Check if the current bit of the index value is set */
		bitplanePointers[j][bitplaneIndex] |= bitmask; /* Modify the current bit in the bitplane byte to be 1 and leave the others untouched */
	    else
		bitplanePointers[j][bitplaneIndex] &= ~bitmask; /* Modify the current bit in the bitplane byte to be 0 and leave the others untouched */
	}
	
	bit--;
	    
	if(bit < 0)
	{
	    bit = 7; /* Reset the bit counter */
	    bitplaneIndex++; /* Go to the next byte in the bitplane memory */
	}
    }
}

amiVideo_UByte *amiVideo_chunkyToBitplanes(const amiVideo_UByte *pixels, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int bitplaneDepth)
{
    unsigned int bitplaneSize = calculateScanLineSize(screenWidth) * screenHeight;
    amiVideo_UByte *result = (amiVideo_UByte*)calloc(bitplaneSize * bitplaneDepth, sizeof(amiVideo_UByte)); /* Fill with 0 bytes */
    amiVideo_UByte *bitplanePointers[MAX_NUM_OF_BITPLANES];
    unsigned int i;
    unsigned int offset = 0;
    
    /* Set bitplane pointers */
    
    for(i = 0; i < bitplaneDepth; i++)
    {
	bitplanePointers[i] = result + offset;
	offset += bitplaneSize;
    }
    
    /* Convert chunky pixels */
    amiVideo_chunkyToBitplaneMemory(bitplanePointers, pixels, screenWidth, screenHeight, bitplaneDepth);
    
    /* Return result */
    return result;
}
