

#include "testapp.h"
#include <iostream>

int main(int argc, const char* argv[]) {

    TestApp app(argc, argv);

    HIGNORE_RETURN(app.Init());

    std::cout << app.GetArgvByIndex(1) << std::endl;

    app.Run();

    return 0;
}