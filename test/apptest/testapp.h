
#ifndef __H_TESTAPP_H__
#define __H_TESTAPP_H__

#include <happ.h>

using namespace HUICPP;


class TestApp final : public HApp {
public:
    using base_class = HApp;    
public:
    TestApp(HN argc, HCSZ argv[]);

    ~TestApp();

public:
    HRET Init () override;

    bool Run () override;
};

#endif //__H_TESTAPP_H__
