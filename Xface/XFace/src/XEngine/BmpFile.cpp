/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Xface Core Library; Bitmap Loader.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * For info, contact: xface-info@itc.it or http://xface.itc.it
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */


#include <XEngine/BmpFile.h>
#include <stdio.h>

#if defined(USE_EGL)
#   include <GLES/gl.h>
#else
#   include <direct.h> // not sure if os independent (not think so)
#   include <GL/glaux.h>
#   ifdef WIN32
#       pragma message("		Adding GLaux lib...")
#       pragma comment( lib, "glaux.lib"	 )
#   endif
#endif

namespace XEngine{

BmpFile::~BmpFile(void)
{
	if (m_pData)
	{
		free(m_pData); // unfortunately it was allocated using malloc in AUX lib.
		m_pData = 0;
	}
}

/*!
	Loads the BMP file to memory (acces data using ITextureFile base class members).
	\note Uses glaux library for the moment. It doesn't come with devcpp/gcc so do not
	forget to add it as a dependency.
*/
bool BmpFile::load(const std::string& filename)
{
#if defined(USE_EGL)
    // load from file directly
    return loadBitmap(filename);
#else
	// Safety check
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp)
		return false;
	else
		fclose(fp);

	AUX_RGBImageRec *pBitmap = auxDIBImageLoad(filename.c_str());	// Load the bitmap and store the data
	
	if(!pBitmap)		// If we can't load the file, quit!
		return false;

	m_Width = pBitmap->sizeX;
	m_Height = pBitmap->sizeY;
	
	m_pData = pBitmap->data;
	m_nBits = 24; // Aux lib loads only these I guess..
	return true;
#endif
}

#if defined(USE_EGL)

#define BI_RGB 0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long           LONG;

#pragma pack(2)
typedef struct {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;
#pragma pack()

typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

#endif//USE_EGL

bool BmpFile::loadBitmap(const std::string &filename)
{
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp)
        return false;

    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bmiHeader;
    if (   fread(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, fp) != 1
        || fread(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1
        || bmfHeader.bfType != (((WORD)'M'<<8)|'B')  // must be 'BM'
        || bmiHeader.biBitCount != 24 )  // only support 24bit bmp file currently
    {
        fclose(fp);
        return false;
    }

    int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : (1<<bmiHeader.biBitCount);
    unsigned long data_size = bmiHeader.biSizeImage;
    if (data_size == 0)
        data_size = bmiHeader.biHeight * bmiHeader.biWidth * sizeof(DWORD);

    // to be consistent with auxDIBImageLoad using malloc
    unsigned char *bmpData = (unsigned char*)malloc(data_size * sizeof(unsigned char));
    unsigned char *tmpData = (unsigned char*)malloc(data_size * sizeof(unsigned char));
    if (bmpData == NULL || tmpData == NULL)
    {
        fclose(fp);
        return false;
    }

    unsigned long off = 0;
    if (bmiHeader.biBitCount > 8)
        off = (bmiHeader.biClrUsed + ((bmiHeader.biCompression==BI_BITFIELDS)?3:0)) * sizeof(RGBQUAD);
    else
        off = nColors * sizeof(RGBQUAD);

    fseek(fp, off, SEEK_CUR);
    fread(tmpData, 1, data_size, fp);

    // convert the bmp data if necessary
    // bmp file might be top-down (bmiHeader.biHeight<0) or bottom-up (bmiHeader.biHeight>0)
    // each line of bmp data must end on a DWORD(4 byte) boundary
    {
        // byteWidth is the width of the actual image in bytes
        // padWidth is the width of the image plus the extra padding
        LONG byteWidth, padWidth, offset;
        byteWidth = padWidth = bmiHeader.biWidth * bmiHeader.biBitCount / 8;
        while (padWidth % 4 != 0)
        {
            // add any extra space to bring each line to a DWORD boundary
            padWidth ++;
        }

        if (bmiHeader.biHeight > 0)
        {
            // bitmap is bottom-up
            // just remove the padding
            // convert from BGR to RGB
            for (int i=0; i<bmiHeader.biHeight; i++)
            {
                unsigned char *dstLine = bmpData + (i*byteWidth);
                unsigned char *srcLine = tmpData + (i*padWidth);
                for (int j=0; j<byteWidth; j+=3)
                {
                    *(dstLine+j)   = *(srcLine+j+2); // R
                    *(dstLine+j+1) = *(srcLine+j+1); // G
                    *(dstLine+j+2) = *(srcLine+j);   // B
                }
            }
        }
        else
        {
            // bitmap is top-down
            // flip the bitmap
            // remove the padding
            // convert from BGR to RGB
            int i, j;
            bmiHeader.biHeight *= -1;
            for (i=0, j=bmiHeader.biHeight-1; i<bmiHeader.biHeight; i++, j--)
            {
                unsigned char *dstLine = bmpData + (i*byteWidth);
                unsigned char *srcLine = tmpData + (j*padWidth);
                for (int j=0; j<byteWidth; j+=3)
                {
                    *(dstLine+j)   = *(srcLine+j+2); // R
                    *(dstLine+j+1) = *(srcLine+j+1); // G
                    *(dstLine+j+2) = *(srcLine+j);   // B
                }
            }
        }
    }

    free(tmpData);

    m_Width = bmiHeader.biWidth;
    m_Height= bmiHeader.biHeight;
    m_nBits = bmiHeader.biBitCount;
    m_pData = bmpData;

    fclose(fp);
    return true;
}

} // namespace XFace

