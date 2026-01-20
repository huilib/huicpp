/*
 * Created: Joseph Hui Mon Dec 19 2022
 * Description: 
  		Wraps BMP.
        transfer RGA to BMP.
*/


#ifndef __H_HUICPP_IMAGE_BMP_H__
#define __H_HUICPP_IMAGE_BMP_H__


#include "hrgb.h"


namespace HUICPP {

namespace IMAGE {

/*
    !!!ONLY 24 COLORS.!!!
*/
class HBmp {
public:

#pragma pack(1)
    // BMP file header.
    typedef struct tagBMPFILEHEADER {
        HUSN    bfType;
        HUN     bfSize;
        HUSN    bfReserved1;
        HUSN    bfReserved2;
        HUN     bfOffBits;
    } BMPFILEHEADER;

    // BMP info header.
    typedef struct tagBMPINFOHEADER {
        HUN     biSize;
        HN      biWidth;
        HN      biHeight;
        HUSN    biPlanes;
        HUSN    biBitCount;
        HUN     biCompression;
        HUN     biSizeImage;
        HN      biXPelsPerMeter;
        HN      biYPelsPerMeter;
        HUN     biClrUsed;
        HUN     biClrImportant;
    } BMPINFOHEADER;    
#pragma pack()

public:
    static constexpr const HUN COLOR_BIT_COUNT = 24;

public:
    HBmp() noexcept;

    explicit HBmp(HCSTRR strFileName, bool bHeadOnly = false) noexcept;

    ~HBmp() noexcept;

public:
    bool IsInvalid() const noexcept;

    HRET ReadFromFile(HCSTRR strFileName, bool bHeadOnly = false) noexcept;    

    HRET Move(HN xoffset, HN yoffset) noexcept;

public:
    HUN Width() const noexcept { return m_info_header.biWidth; }

    HUN Height () const noexcept { return m_info_header.biHeight; }

    HUN BitCount () const noexcept { return m_info_header.biBitCount; }

    HUN ImageSize() const noexcept { return m_info_header.biSizeImage; }

    HUN TotalSize() const noexcept { return m_file_header.bfSize; }

    HUN HeaderSize() const noexcept { return (sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER)); }

    HCUSZ GetDataPtr() const noexcept { return m_pData; } 

    HUN GetLineBytes() const noexcept;

public:
    HRET FromRgb(const HRgb& rga);
    
    HRET SaveAs(HCSTRR strFileName) const;

    HRET ExportBmpBuffer(HUSZ buffer, HUNR len) const;

private:
    void clear();

public:
    static HUN BytesPerLine(HUN nWidth, HUN nBitCount) noexcept;

private:
    BMPFILEHEADER m_file_header;
    BMPINFOHEADER m_info_header;
    HUSZ m_pData;
};

}

}


#endif //__H_HUICPP_IMAGE_BMP_H__