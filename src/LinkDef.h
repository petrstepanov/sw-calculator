#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

// Main GUI class inherited from TGMainFrame
#pragma link C++ class MyMainFrame;

// PDFs inherited from RooAbsPdf
#pragma link C++ class BackgroundPdf;
#pragma link C++ class DampLorentzPdf;
#pragma link C++ class GaussianPdf;
#pragma link C++ class LorentzianPdf;
#pragma link C++ class OrthogonalPdf;
#pragma link C++ class ParabolaPdf;
#endif
