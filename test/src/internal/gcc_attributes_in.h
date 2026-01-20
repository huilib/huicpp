/**********************************************************
 * Created: Joseph Aug-02-2022
 * Description: 
 *      GCC compiler attribute optimizer defines.
 * 
 **********************************************************/

#ifndef __H_HUICPP_GCC_ATTRIBUTES_IN_H_
#define __H_HUICPP_GCC_ATTRIBUTES_IN_H_

namespace HUICPP {

#ifndef HPACKED_ATTRIBUTE 
#define HPACKED_ATTRIBUTE __attribute__((__packed__))
#endif //HPACKED_ATTRIBUTE

#ifndef HNONNULL_INPUT_POINTER 
#define HNONNULL_INPUT_POINTER(params) __attribute__ ((__nonnull__ params))
#endif //HNONNULL_INPUT_POINTER

#ifndef HALWAYS_INLINE
#define HALWAYS_INLINE inline __attribute__((__always_inline__))
#endif //HALWAYS_INLINE

#ifndef HCOLD_FUNCTION
#define HCOLD_FUNCTION __attribute__((__cold__))
#endif //HCOLD_FUNCTION

#ifndef HHOT_FUNCTION
#define HHOT_FUNCTION __attribute__((__hot__))
#endif //HHOT_FUNCTION

}

#endif //__H_HUICPP_GCC_ATTRIBUTES_IN_H_
