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
 * The Original Code is Xface Core Library; TGA Loader.
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

#include <XEngine/TgaFile.h>
#include <XEngine/TgaImg.h>

namespace XEngine{

TgaFile::TgaFile(void)
{
}

TgaFile::~TgaFile(void)
{
	if (m_pData)
	{
		delete [] m_pData;
		m_pData = 0;
	}
}

bool TgaFile::checkSize( int x ) const 
{
    // Make sure its a power of 2.
    if( x == 2	 || x == 4   || 
        x == 8	 || x == 16  || 
        x == 32  || x == 64  ||
        x == 128 || x == 256 || 
        x == 512 )
        return true;

    return false;
}

/*!
	Loads the TGA file to memory (acces data using ITextureFile base class members).
*/
bool TgaFile::loadold(const std::string& filename)
{
	// Loads up a targa file. Supported types are 8,24 and 32 
    // uncompressed images.
    unsigned char type[4];
    unsigned char info[7];
    FILE *fp = NULL;
    int size = 0;
    
	if( !(fp = fopen( filename.c_str(), "rb" )) )
        return false;

    fread( &type, sizeof (char), 3, fp );   // Read in colormap info and image type, byte 0 ignored
    fseek( fp, 12, SEEK_SET);			   // Seek past the header and useless info
    fread( &info, sizeof (char), 6, fp );

    if( type[1] != 0 || (type[2] != 2 && type[2] != 3) )
	{
		fclose (fp);
		return false;
	}

    m_Width  = info[0] + info[1] * 256; 
    m_Height = info[2] + info[3] * 256;
    m_nBits  = info[4]; 

    size = m_Width * m_Height;

    // Make sure dimension is a power of 2  
    if( !checkSize(m_Width) || !checkSize(m_Height))
	{
		fclose(fp);
		return false;
	}

    // Make sure we are loading a supported type  
    if( m_nBits != 32 && m_nBits != 24 && m_nBits != 8 )
	{
		fclose(fp);
		return false;
	}
    
    if( m_nBits == 32 )
        m_pData = loadRGBA( fp, size );
    else if( m_nBits == 24 )
        m_pData = loadRGB( fp, size );	
    else if( m_nBits == 8 )
        m_pData = loadGray( fp, size );

    // No image data 
    if( m_pData == NULL )
    {
		fclose(fp);
		return false;
	}

    fclose( fp );

    return true;
}

unsigned char* TgaFile::loadRGBA(FILE* fp, int size)
{
	// Read in RGBA data for a 32bit image. 
    unsigned char *rgba;
    unsigned char temp;
    size_t bread;
    int i;

    rgba = new unsigned char[size * 4];

	// safety check
    if( rgba == NULL )
        return 0;

    bread = fread( rgba, sizeof (unsigned char), size * 4, fp ); 

    // safety check
    if( bread != size * 4 )
    {
        delete []rgba;
        return 0;
    }

    // TGA is stored in BGRA, make it RGBA  
	for( i = 0; i < size * 4; i += 4 )
    {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }

    return rgba;
}

unsigned char* TgaFile::loadRGB(FILE* fp, int size)
{
	// Read in RGB data for a 24bit image. 
    unsigned char *rgb;
    unsigned char temp;
    size_t bread;
    int i;

    rgb = new unsigned char [size * 3];

    if( rgb == NULL )
        return 0;

    bread = fread( rgb, sizeof (unsigned char), size * 3, fp );

    if(bread != size * 3)
    {
        delete []rgb;
        return 0;
    }

    // TGA is stored in BGR, make it RGB  
    for( i = 0; i < size * 3; i += 3 )
    {
        temp = rgb[i];
        rgb[i] = rgb[i + 2];
        rgb[i + 2] = temp;
    }

    return rgb;
}

unsigned char* TgaFile::loadGray(FILE* fp, int size)
{
	// Gets the grayscale image data.  Used as an alpha channel.
    unsigned char *grayData;
    size_t bread;

    grayData = new unsigned char[size];

    if( grayData == NULL )
        return 0;

    bread = fread( grayData, sizeof (unsigned char), size, fp );

    if( bread != size )
    {
        delete [] grayData;
        return 0;
    }

    return grayData;
}


/*!
Loads the TGA file to memory (acces data using ITextureFile base class members).
*/
bool TgaFile::load(const std::string& filename)
{
    // Loads up a targa file. Supported types are 24 and 32 
    // uncompressed images.

    TGAImg Img;        // Image loader
    // Load our Texture
    if(Img.Load((char*)filename.c_str())!=IMG_OK)
        return false;

    m_Width  = Img.GetWidth(); 
    m_Height = Img.GetHeight();
    m_nBits  = Img.GetBPP();

    // Make sure we are loading a supported type
    int nSize = m_Width*m_Height;
    if (m_nBits==32)
    {
        m_pData = new unsigned char[4*nSize];
        memcpy(m_pData, Img.GetImg(), 4*nSize);
        return true;
    }
    else if (m_nBits==24)
    {
        m_pData = new unsigned char[3*nSize];
        memcpy(m_pData, Img.GetImg(), 3*nSize);
        return true;
    }
    else
    {
        return false;
    }
}

}  // namespace XFace