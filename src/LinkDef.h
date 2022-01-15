#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

// Generally speaking we list all the classes that take advantage of the ClassDef() and ClassImp() here

// Main frames
#pragma link C++ class MyMainFrame+;
#pragma link C++ class ModalDialogFrame+;
#pragma link C++ class RooRealVarFrame+;
#pragma link C++ class RooRealVarListFrame+;

// GUI class overrides
#pragma link C++ class MyTGNumberEntry+;

// For signals in model
#pragma link C++ class Model+;

// SWCalculator widget
#pragma link C++ class SWCalculatorView<SWCalculatorPresenter>+;
#pragma link C++ class SWCalculatorView+;
#pragma link C++ class SWCalculatorPresenter<Model, SWCalculatorView>+;
#pragma link C++ class SWCalculatorPresenter+;

// RooRealVar widget
#pragma link C++ class RooRealVarView<RooRealVarPresenter>+;
#pragma link C++ class RooRealVarView+;
#pragma link C++ class RooRealVarPresenter<RooRealVar, RooRealVarView>+;
#pragma link C++ class RooRealVarPresenter+;

// AbstractImportSpectrum widget (no need because abstract?)
#pragma link C++ class AbstractImportSpectrumView<AbstractImportSpectrumPresenter>+;
#pragma link C++ class AbstractImportSpectrumView+;
#pragma link C++ class AbstractImportSpectrumPresenter<Model, AbstractImportSpectrumView>+;
#pragma link C++ class AbstractImportSpectrumPresenter+;

// ImportSpectrum widget
#pragma link C++ class ImportSpectrumView<ImportSpectrumPresenter>+;
#pragma link C++ class ImportSpectrumView+;
#pragma link C++ class ImportSpectrumPresenter<Model, ImportSpectrumView>+;
#pragma link C++ class ImportSpectrumPresenter+;

// ImportSourceSpectrum widget
#pragma link C++ class ImportSourceSpectrumView<ImportSourceSpectrumPresenter>+;
#pragma link C++ class ImportSourceSpectrumView+;
#pragma link C++ class ImportSourceSpectrumPresenter<Model, ImportSourceSpectrumView>+;
#pragma link C++ class ImportSourceSpectrumPresenter+;

// ImportComponent widget
#pragma link C++ class ImportComponentView<ImportComponentPresenter>+;
#pragma link C++ class ImportComponentView+;
#pragma link C++ class ImportComponentPresenter<Model, ImportComponentView>+;
#pragma link C++ class ImportComponentPresenter+;

// PDFs inherited from RooAbsPdf
#pragma link C++ class StepPdf+;
#pragma link C++ class BackgroundPdf+;
#pragma link C++ class DampLorentzPdf+;
#pragma link C++ class GaussianPdf+;
#pragma link C++ class LorentzianPdf+;
#pragma link C++ class OrthogonalPdf+;
#pragma link C++ class ParabolaPdf+;

// Why this?
#pragma link C++ class DoublePair+;


#endif
