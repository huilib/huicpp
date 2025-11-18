/**********************************************************
 * Created: Joseph Mar-29-2021
 * Description: 
 *      Wrapper for fixed-length sequence.
 * 
 **********************************************************/

#ifndef __H_HUICPP_BITS_H__
#define __H_HUICPP_BITS_H__

#include "huicpp.h"
#include "utils/base_utils.h"

namespace HUICPP {

    template <HUN _BITS>
    class basic_bits {
    protected:
        using byte_t = HUCH;
        static constexpr const HUN BITS = _BITS;
        static constexpr const HUN BYTE_WIDTH = sizeof(byte_t);
        static constexpr const HUN WIDTH = utils::uper_divied(BITS, BYTE_WIDTH);
        static constexpr const HULN KB_LENGTH = 1024;
        static constexpr const HULN MB_LENGTH = 1024 * KB_LENGTH;
        static constexpr const HULN GB_LENGTH = 1024 * MB_LENGTH;

    public:
        /* construct 0 value by default */
        constexpr basic_bits(): m_data{0} { }

        /* construct for constant between 0 and 255 */
        constexpr explicit basic_bits(byte_t v): m_data{v} { }

    public:
        static HULN KB_length(HN kbs) {
            return KB_LENGTH * kbs;
        }

        static HULN MB_length(HN mbs) {
            return MB_LENGTH * mbs;            
        }

        static HULN GB_length(HN gbs) {
            return GB_LENGTH * gbs;
        }

    private:
        byte_t m_data[WIDTH];
    };


#include "inl/hbits_inl.h"

using HByte = basic_bits<1>;

}

#endif //__H_HUICPP_BITS_H__

