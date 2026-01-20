/*
 * Created: Joseph Hui Mon Dec 18 2022
 * Description: 
  		Wraps RGA.
        transfer YUV 21/420 to RGA.
*/


#ifndef __H_HUICPP_IMAGE_RGB_H__
#define __H_HUICPP_IMAGE_RGB_H__


#include "../huicpp.h"


namespace HUICPP {

namespace IMAGE {


class HRgb {
public:
    HRgb() noexcept;

    HRgb(HUN nWidth, HUN nHeight, HUSZ ptr = nullptr);

    HRgb(HRgb&& oth) noexcept;

    ~HRgb() noexcept;

    HRgb& operator=(HRgb&& oth) noexcept;

public:
    void Release() noexcept;    

public:
    HUN Width() const noexcept { return m_nWidth; }

    HUN Height() const noexcept { return m_nHeight; }

    HUN Size() const noexcept { return Width() * Height() * 3; }   

    HCUSZ GetDataPtr() const noexcept { return m_pData; } 

    bool IsInvalid() const noexcept { return Width() > 0 and Height() > 0 and GetDataPtr() != nullptr; }
    
public:
    static HRgb FromNV21(HCUSZ data, HUN nWidth, HUN nHeight);

    static HRgb FromYuv420(HCUSZ data, HUN nWidth, HUN nHeight);

    static HRgb FromYuv420sp(HCUSZ data, HUN nWidth, HUN nHeight);

private:
    HUN m_nWidth;
    HUN m_nHeight;
    HUSZ m_pData;
};

}

}



#endif //__H_HUICPP_IMAGE_RGB_H__
