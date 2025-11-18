

#include "huicpp.h" 
#include <assert.h>

namespace HUICPP {

    // huicpp's global error variable.
    RET_T g_hErr = ERR_NO(OK);


#define BASE_ERR_STR_GEN(n,s) s,

    static constexpr HCSZ s_base_err_map[] = {

		HUICPP_ERR_MAP(BASE_ERR_STR_GEN)

    };

#undef BASE_ERR_STR_GEN


    HCSZ hbase_err (RET_T err) {

		if (err < 0 || err >=  ERR_NO(ERR_END)){

			g_hErr = ERR_NO(UNKOWN_ERR);
			return s_base_err_map[g_hErr];
			
		}

		g_hErr = err;

		assert(g_hErr >= 0 && g_hErr < ERR_NO(ERR_END));

		return s_base_err_map[g_hErr];

    }

	RET_T hlast_err ()  {

		return g_hErr;
	
	}

}









