#include <TROOT.h>
#include <TApplication.h>
#include <TGFrame.h>
#include "widgets/swCalculatorWidget/SWCalculatorView.h"
#include "widgets/MainView.h"
#include "model/Constants.h"

//int main(int argc, char **argv) {
//    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
//    TTripleSliderDemo* tt = new TTripleSliderDemo(gClient->GetRoot());
//    app->Run();
//    return 0;
//}

//int main(int argc, char **argv) {
//    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
//    SWCalculatorView* swCalculatorView = new SWCalculatorView(gClient->GetRoot());
//    app->Run();
//    return 0;
//}

int main(int argc, char **argv) {
    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
    MainView* mainView = new MainView(gClient->GetRoot());
    SWCalculatorView* swCalculatorView = new SWCalculatorView(mainView);
    mainView->AddFrame(swCalculatorView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    mainView->mapAndResize();
    app->Run();
    return 0;
}

//int main(int argc, char **argv) {
//    TApplication* app = new TApplication(Constants::applicationName, &argc, argv);
//    TGMainFrame* mainFrame = new TGMainFrame(gClient->GetRoot(), Constants::windowWidth, Constants::windowHeight, kMainFrame);
//    SWCalculatorView* swCalculatorView = new SWCalculatorView(mainFrame);
//    mainFrame->AddFrame(swCalculatorView, new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY));
//    //    mainFrame->SetIconPixmap("./icon.png");
////    mainFrame->SetWindowName(Constants::applicationName);    
//    mainFrame->MapSubwindows();
//    mainFrame->Resize(mainFrame->GetDefaultSize());
//    mainFrame->MapWindow();
//    mainFrame->Resize(Constants::windowWidth, Constants::windowHeight);
//    app->Run();
//
//    return 0;
//}