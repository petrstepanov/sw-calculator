#include <TROOT.h>
#include <TApplication.h>
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "model/Constants.h"

int main(int argc, char **argv) {
    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
    TGMainFrame* mainFrame = new TGMainFrame(gClient->GetRoot(), Constants::windowWidth, Constants::windowHeight);
    mainFrame->SetIconPixmap("icon.png");
    mainFrame->SetWindowName(Constants::applicationName);    
    SWCalculatorView* swCalculatorView = new SWCalculatorView(mainFrame);
    mainFrame->AddFrame(swCalculatorView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    app->Run();
    return 0;
}