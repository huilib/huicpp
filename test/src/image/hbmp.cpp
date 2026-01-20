

#include "hbmp.h"
#include "../hfilehandle.h"
#include "../hfilename.h"
#include "../htodoholder.h"

namespace HUICPP {

namespace IMAGE {


HBmp::HBmp() noexcept
    : m_file_header(), m_info_header(), m_pData(nullptr) {
    ZERO_MEM(&m_file_header, sizeof(BMPFILEHEADER));
    ZERO_MEM(&m_info_header, sizeof(BMPINFOHEADER));
}


HBmp::HBmp(HCSTRR strFileName, bool bHeadOnly) noexcept 
    : m_file_header(), m_info_header(), m_pData(nullptr) {        
    ZERO_MEM(&m_file_header, sizeof(BMPFILEHEADER));
    ZERO_MEM(&m_info_header, sizeof(BMPINFOHEADER));
    HIGNORE_RETURN(ReadFromFile(strFileName, bHeadOnly));
}


HBmp::~HBmp() noexcept {
    clear();
}


bool HBmp::IsInvalid() const noexcept {
    return ((m_file_header.bfType >> 8 & 0xff) == 'M'  
        and (m_file_header.bfType & 0xff) == 'B' 
        and m_info_header.biBitCount == 24                      // MAKE SURE 24 BITS COLOR.
    and TotalSize() > HeaderSize()); 
}


HRET HBmp::ReadFromFile(HCSTRR strFileName, bool bHeadOnly) noexcept {

    HASSERT_RETURN(not strFileName.empty(), INVL_PARA);
    HASSERT_RETURN(HFileName(strFileName).Exists(), INVL_PARA);
    
    // clear first.
    clear();   
    HFileHandle fh;
    HNOTOK_RETURN(fh.Open(strFileName, O_RDONLY));

    HN ret = HTO_INT(fh.Read(&m_file_header, sizeof(BMPFILEHEADER)));
    // cannot read BMPFILEHEADER
    HASSERT_RETURN(ret == HTO_INT(sizeof(BMPFILEHEADER)), SRC_ERR);

    HASSERT_RETURN((m_file_header.bfType >> 8 & 0xff) == 'M'  
        and (m_file_header.bfType & 0xff) ==  'B', ERR_STATUS);

    ret = HTO_INT(fh.Read(&m_info_header, sizeof(BMPINFOHEADER)));
    // cannot read BMPINFOHEADER
    HASSERT_RETURN(ret == HTO_INT(sizeof(BMPINFOHEADER)), SRC_ERR);

    // make sure the bitmap is 24 bits color. RGB
    HASSERT_RETURN(m_info_header.biBitCount == 24, ERR_STATUS);

    // if the image size is 0, we calculate the image size.
    if (m_info_header.biSizeImage == 0) {
        m_info_header.biSizeImage = GetLineBytes() * m_info_header.biHeight;
    }

    if (bHeadOnly) {
        HRETURN_OK;
    }
 
    m_pData = static_cast<HUSZ>(malloc(m_info_header.biSizeImage));
    NULLPOINTER_CHECK(m_pData);

    ret = HTO_INT(fh.Read(m_pData, m_info_header.biSizeImage));
    HASSERT_RETURN(ret == HTO_INT(m_info_header.biSizeImage), SRC_ERR);
    
    HRETURN_OK;

}


HRET HBmp::Move(HN xoffset, HN yoffset) noexcept {
    HASSERT_RETURN(IsInvalid(), ERR_STATUS);
    HUN bytesPerLine = HBmp::BytesPerLine(Width(), BitCount());
    HN bc = HTO_INT(BitCount() / 8);
    HUSZ temp_buf = (HUSZ)malloc(bytesPerLine * Height());
    HSCOPE_EXIT { HFREE(temp_buf); };
    HN nWidth = HTO_INT(Width());
    HN nHeight = HTO_INT(Height());

    for (HN i = 0; i < nHeight; ++i) {
        for (HN j = 0; j < nWidth; ++j) {
            HUSZ dstpos = temp_buf + bytesPerLine * (nHeight - 1 - i) + j * bc;
            if ((j - xoffset >= 0 and j - xoffset < nWidth) and 
                (i - yoffset >= 0 and i - yoffset < nHeight )) {
                HUSZ srcpos = m_pData + bytesPerLine * (nHeight - 1 - (i - yoffset)) + 
                    (j - xoffset) * bc;
                for (HN k = 0; k < bc; ++k) {
                    *(dstpos + k) = *(srcpos + k);
                }
            } else {
                for (HN k = 0; k < bc; ++k) {
                    *(dstpos + k) = 255;
                }
            }
        }
    }

    memcpy(m_pData, temp_buf, ImageSize());
    HRETURN_OK;
}


HUN HBmp::GetLineBytes() const noexcept {
    return BytesPerLine(m_info_header.biWidth, m_info_header.biBitCount);
}


HRET HBmp::FromRgb(const HRgb& rga) {

    //4 bytes pack. must be 4 times per line。
    HUN bytesPerLine = HBmp::BytesPerLine(rga.Width(), COLOR_BIT_COUNT);
    HUN pixcelBytes  = bytesPerLine * rga.Height();

    // fill file header
    m_file_header.bfType        = 0x4D42;
    m_file_header.bfReserved1   = 0;
    m_file_header.bfReserved2   = 0;
    m_file_header.bfOffBits     = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER);
    m_file_header.bfSize        = m_file_header.bfOffBits + pixcelBytes;

    // fill info header
    m_info_header.biSize          = sizeof(BMPINFOHEADER);
    m_info_header.biWidth         = HTO_INT(rga.Width());
    /** 这样图片才不会倒置 */
    m_info_header.biHeight        = - HTO_INT(rga.Height()); 
    m_info_header.biPlanes        = 1;
    m_info_header.biBitCount      = COLOR_BIT_COUNT;
    m_info_header.biCompression   = 0;
    m_info_header.biSizeImage     = pixcelBytes;
    m_info_header.biXPelsPerMeter = 100;
    m_info_header.biYPelsPerMeter = 100;
    m_info_header.biClrUsed       = 0;
    m_info_header.biClrImportant  = 0;

    HFREE(m_pData);
    m_pData = static_cast<HUSZ>(malloc(pixcelBytes));
    CHECK_NEWPOINT(m_pData);
    HUSZ pBmpData = m_pData;    
    HCUSZ pRgaData = rga.GetDataPtr();
  
    HUN nWidth = rga.Width(), nHeight = rga.Height();
    for (HUN i = 0; i < nHeight; i++) {
        for (HUN j=0; j < nWidth; j++) {
           pBmpData[0] = pRgaData[2];
           pBmpData[1] = pRgaData[1];
           pBmpData[2] = pRgaData[0];
           pRgaData += 3;
           pBmpData += 3;
        }
        //pack for 4 bytes
        pBmpData += (bytesPerLine - nWidth * 3);
    }

    HRETURN_OK;
}


HRET HBmp::SaveAs(HCSTRR strFileName) const {
    HASSERT_RETURN(IsInvalid(), ERR_STATUS);
    HASSERT_RETURN(not strFileName.empty(), INVL_PARA);
    HFileHandle fh;
    HNOTOK_RETURN(fh.Open(strFileName, O_WRONLY | O_CREAT));
    fh.Write(&m_file_header, sizeof(m_file_header));
    fh.Write(&m_info_header, sizeof(m_info_header));
    fh.Write(m_pData, ImageSize());
    HRETURN_OK;
}


HRET HBmp::ExportBmpBuffer(HUSZ buffer, HUNR len) const {
    HASSERT_RETURN(IsInvalid(), ERR_STATUS);
    HASSERT_RETURN(buffer != nullptr, INVL_PARA);
    HASSERT_RETURN(len >= TotalSize(), INVL_PARA);
    HUSZ pos = buffer;
    memcpy(pos, &m_file_header, sizeof(m_file_header));
    pos += sizeof(m_file_header);
    memcpy(pos, &m_info_header, sizeof(m_info_header));
    pos += sizeof(m_info_header);
    memcpy(pos, m_pData, ImageSize());
    len = static_cast<HUN>(TotalSize());
    HRETURN_OK;
}


void HBmp::clear() {

    HFREE(m_pData);
    ZERO_MEM(&m_file_header, sizeof(BMPFILEHEADER));
    ZERO_MEM(&m_info_header, sizeof(BMPINFOHEADER));    
    
}


HUN HBmp::BytesPerLine(HUN nWidth, HUN nBitCount) noexcept {
    return (nWidth * nBitCount + 31) / 32 * 4;    
}


}

}