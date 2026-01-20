/*
 * Created: Joseph Hui Tue Dec 27 2016
 * Description: 
  		Wrapper for huicpp api factory.
*/


#ifndef __H_HUICPP_API_REPO_H__
#define __H_HUICPP_API_REPO_H__


#include "huicpp.h"


namespace HUICPP {

template<class _Ty>
class HApiFactory {
public:
	using api_type = _Ty;
	using api_pointer = api_type*;
	typedef api_pointer (* api_maker)();
	using api_factory_t = std::map<HSTR, api_maker>;

	HApiFactory() {}

	~ HApiFactory () { }

	void RegisterApi (HCSTRR name, api_maker maker) {

		m_factory[name] = maker;

	}

	api_pointer NewApi (HCSTRR name) {
		
		if (name.empty()) {
			return nullptr;
		}
		
		typename api_factory_t::iterator fit = m_factory.find(name);
		if (fit == m_factory.end()) {
			return nullptr;
		}

		auto pfun = fit->second;
		if (pfun != nullptr) {
			return pfun();
		}

		return nullptr;
	}

	private:
		api_factory_t m_factory;
    };

}


#endif // matched __H_HUICPP_API_REPO_H__











