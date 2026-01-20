

#include "detectxbapp.h"
#include <iostream>

DetectXbApp::DetectXbApp(HN argc, HCSZ argv[])
    : base_class(argc, argv) {

}


DetectXbApp::~DetectXbApp() {

}


HRET DetectXbApp::Init () {

    HNOTOK_RETURN(base_class::Init());

    HRETURN_OK;

}


bool DetectXbApp::Run () {

    std::cout << "hello fin" << std::endl;

    return true;

}

