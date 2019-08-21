#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

// GUI classes
#pragma link C++ class MyMainFrame;
#pragma link C++ class ModalDialogFrame;
#pragma link C++ class RooRealVarFrame;
#pragma link C++ class RooRealVarListFrame;
#pragma link C++ class SWCalculatorView;
#pragma link C++ class MyTGNumberEntry;

// For signals and slots
#pragma link C++ class Model;
#pragma link C++ class SWCalculatorPresenter;

// PDFs inherited from RooAbsPdf
#pragma link C++ class BackgroundPdf;
#pragma link C++ class DampLorentzPdf;
#pragma link C++ class GaussianPdf;
#pragma link C++ class LorentzianPdf;
#pragma link C++ class OrthogonalPdf;
#pragma link C++ class ParabolaPdf;

// Why this?
#pragma link C++ class DoublePair;


#endif
