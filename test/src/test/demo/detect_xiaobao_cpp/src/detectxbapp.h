
#ifndef __H_DETECTXB_APP_H__
#define __H_DETECTXB_APP_H__

#include <happ.h>

using namespace HUICPP;


class DetectXbApp final : public HApp {
public:
    using base_class = HApp;    
public:
    DetectXbApp(HN argc, HCSZ argv[]);

    ~DetectXbApp();

public:
    HRET Init () override;

    bool Run () override;
};

#endif //__H_DETECTXB_APP_H__
