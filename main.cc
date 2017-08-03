#include <TROOT.h>
#include <TApplication.h>
#include "SWCalculatorFrame.h"

int main(int argc, char **argv) {
    TApplication theApp("App", &argc, argv);
    new SWCalculatorFrame(gClient->GetRoot(), 1000, 800);
    theApp.Run();
    return 0;
}