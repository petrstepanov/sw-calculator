#include <TROOT.h>
#include <TApplication.h>
#include <TGFrame.h>
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/MyMainFrame.h"
#include "model/Constants.h"
#include "model/Model.h"

int main(int argc, char **argv) {
    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
    MyMainFrame* myMainFrame = new MyMainFrame(gClient->GetRoot());
    SWCalculatorView* swCalculatorView = new SWCalculatorView(myMainFrame);

    myMainFrame->AddFrame(swCalculatorView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    myMainFrame->mapAndResize();
    swCalculatorView->setSourceContributionFrameVisible(kFALSE);

    app->Run();
    return 0;
}
