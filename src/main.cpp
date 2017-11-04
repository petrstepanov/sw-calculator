#include <TROOT.h>
#include <TApplication.h>
#include <TGFrame.h>
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/MainView.h"
#include "model/Constants.h"
#include "model/Model.h"

int main(int argc, char **argv) {
    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
    MainView* mainView = new MainView(gClient->GetRoot());
    SWCalculatorView* swCalculatorView = new SWCalculatorView(mainView);
    mainView->AddFrame(swCalculatorView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    mainView->mapAndResize();
    swCalculatorView->setSourceContributionFrameVisible(kFALSE);
    app->Run();
    return 0;
}