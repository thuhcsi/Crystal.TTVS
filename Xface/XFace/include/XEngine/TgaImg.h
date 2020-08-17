
#pragma once

#ifndef _TGA_IMAGE_INCLUDED_
#define _TGA_IMAGE_INCLUDED_

#include <fstream>

namespace XEngine
{
    // TGA Loader - 16/11/04 Codehead
#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT  0x8
#define IMG_ERR_UNSUPPORTED 0x40

    class TGAImg
    {
    public:
        TGAImg();
        ~TGAImg();
        int Load(char* szFilename);
        int GetBPP() const {return iBPP;}
        int GetWidth() const {return iWidth;}
        int GetHeight() const {return iHeight;}
        unsigned char* GetImg() const {return pImage;}          // Return a pointer to image data
        unsigned char* GetPalette() const {return pPalette;}    // Return a pointer to VGA palette

    private:
        short int iWidth,iHeight,iBPP;
        unsigned long lImageSize;
        char bEnc;
        unsigned char *pImage, *pPalette, *pData;

        // Internal workers
        int ReadHeader();
        int LoadRawData();
        int LoadTgaRLEData();
        int LoadTgaPalette();
        void BGRtoRGB();
        void FlipImg();
    };
}

#endif//_TGA_IMAGE_INCLUDED_
