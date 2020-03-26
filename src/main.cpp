#include <TROOT.h>
#include <TApplication.h>
#include <TGFrame.h>
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/MyMainFrame.h"
#include "model/Constants.h"

int main(int argc, char **argv) {
    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);

    // Main frame sets icon, title, resizes and maps the window
    MyMainFrame* myMainFrame = new MyMainFrame();
    SWCalculatorView* swCalculatorView = new SWCalculatorView(myMainFrame);
    myMainFrame->addChildFrame(swCalculatorView);

    // Hiding frames only possible after Window was mapped.
    // swCalculatorView->onUiReady();

    app->Run();
    return 0;
}
