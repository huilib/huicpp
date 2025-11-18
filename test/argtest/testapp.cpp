

#include "testapp.h"
#include <iostream>

TestApp::TestApp(HN argc, HCSZ argv[])
    : base_class(argc, argv) {

}


TestApp::~TestApp() {

}


HRET TestApp::Init () {

    HNOTOK_RETURN(base_class::Init());

    HRETURN_OK;

}


bool TestApp::Run () {

    std::cout << "hello fin" << std::endl;

    return true;

}

