/**
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

AMI_UByte *amiVideo_bitplanesToChunky(const AMI_UByte *bitplanes, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int bitplaneDepth)
{
    unsigned int i;
    unsigned char *result = (AMI_UByte*)calloc(screenWidth * screenHeight, sizeof(AMI_UByte)); /* Empty with 0 bytes */
    unsigned int offset = 0;
    unsigned int bitplaneSize = screenWidth * screenHeight / 8;
    
    for(i = 0; i < bitplaneDepth; i++)
    {
	unsigned int j;
	unsigned int count = 0;
	AMI_UByte value = 1 << i;
	
	for(j = 0; j < bitplaneSize; j++)
	{
	    AMI_UByte bitplane = bitplanes[offset + j];
	    
	    if(bitplane & 0x80)
		result[count] |= value;
	
	    if(bitplane & 0x40)
		result[count + 1] |= value;
	
	    if(bitplane & 0x20)
		result[count + 2] |= value;
		
	    if(bitplane & 0x10)
		result[count + 3] |= value;
	
	    if(bitplane & 0x08)
		result[count + 4] |= value;
	
	    if(bitplane & 0x04)
		result[count + 5] |= value;
	
	    if(bitplane & 0x02)
		result[count + 6] |= value;
	
	    if(bitplane & 0x01)
		result[count + 7] |= value;
	
	    count += 8;
	}
	
	offset += bitplaneSize;
    }
    
    return result;
}

AMI_OutputColor *amiVideo_bitplanesToRGB(const AMI_UByte *bitplanes, const unsigned int screenWidth, const unsigned int screenHeight, const unsigned int bitplaneDepth, const AMI_Color *color, const unsigned int colorLength, const AMI_Long viewportMode)
{
    AMI_UByte *bytes = amiVideo_bitplanesToChunky(bitplanes, screenWidth, screenHeight, bitplaneDepth);
    unsigned int paletteSize;
    AMI_OutputColor *palette = amiVideo_computePalette(color, colorLength, viewportMode, &paletteSize);
    AMI_OutputColor *result = (AMI_OutputColor*)malloc(screenWidth * screenHeight * sizeof(AMI_OutputColor));
    unsigned int i;
    
    if(amiVideo_checkHoldAndModify(viewportMode))
    {
	unsigned int offset = 0;
	
	/* HAM mode */
	
	for(i = 0; i < screenHeight; i++)
	{
	    unsigned int j;
	    AMI_OutputColor previousResult = palette[0];
	    
	    for(j = 0; j < screenWidth; j++)
	    {
		AMI_UByte byte = bytes[offset + j];
		AMI_UByte mode = (byte & (0x3 << (bitplaneDepth - 2))) >> (bitplaneDepth - 2);
		AMI_UByte index = byte & ~(0x3 << (bitplaneDepth - 2));
	    
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
	    
	    offset += screenWidth;
	}
    }
    else
    {
	/* Normal mode */
	for(i = 0; i < screenWidth * screenHeight; i++)
	    result[i] = palette[bytes[i]];
    }
    
    free(bytes);
    free(palette);
    
    return result;
}
