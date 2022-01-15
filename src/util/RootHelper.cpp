/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RootHelper.cpp
 * Author: petrstepanov
 *
 * Created on August 24, 2017, 8:16 PM
 */

#include "RootHelper.h"
#include "Debug.h"
#include "MathUtil.h"
#include "StringUtils.h"
#include "../model/Constants.h"

#include <TUnixSystem.h>
#include <TGFrame.h>
#include <TObject.h>
#include <TMatrixD.h>
#include <TMatrixDUtils.h>
#include <TString.h>
#include <TList.h>
#include <TObjString.h>

#include <RooHist.h>
#include <RooPlot.h>

#include <iostream>
#include <utility>

void RootHelper::deleteObject(const char* name){
    TObject* obj = gROOT->FindObject(name);
    deleteObject(obj);
}

void RootHelper::deleteObject(TObject* obj){
    if (obj){
        obj->Delete();
    }
}

Int_t RootHelper::getNumCpu(){
    SysInfo_t sysInfo;
    gSystem->GetSysInfo(&sysInfo);
    std::cout << "RootHelper::getNumCpu: sysInfo.fCpus = " << sysInfo.fCpus << std::endl;
    return (sysInfo.fCpus >= 0) ? sysInfo.fCpus : 1;
}

TStopwatch* RootHelper::watch = new TStopwatch();
TRandom* RootHelper::random = new TRandom(0);

void RootHelper::startTimer(){
    watch->Start();
}

void RootHelper::stopAndPrintTimer(){
    watch->Stop();
    watch->Print();
}

TGNumberFormat::EStyle RootHelper::getNumberFormatStyle(Double_t value){
	Int_t numberOfDecimals = MathUtil::numberOfDecimals(value);
	switch (numberOfDecimals){
		case 0:
			return TGNumberFormat::EStyle::kNESInteger;
		case 1:
			return TGNumberFormat::EStyle::kNESRealOne;
		case 2:
			return TGNumberFormat::EStyle::kNESRealTwo;
		case 3:
			return TGNumberFormat::EStyle::kNESRealThree;
		case 4:
			return TGNumberFormat::EStyle::kNESRealFour;
		default:
			return TGNumberFormat::EStyle::kNESReal;
	}
}


void RootHelper::setRooRealVarValueLimits(RooRealVar* var, Double_t value, Double_t min, Double_t max){
	// Tip: roofit can't set min larger than current max and vice versa
	var->setMin(std::numeric_limits<double>::min());
	var->setMax(std::numeric_limits<double>::max());
	var->setMin(min);
	var->setMax(max);
	var->setVal(value);
}

// Deprecated, use getParameterByNameCommonOrLocal()
RooRealVar* RootHelper::getParameterNameContains(RooAbsPdf* pdf, const char* nameSubstring){
	RooArgSet* parameters = pdf->getParameters(RooArgSet());
	TIterator* it = parameters->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooRealVar* rrv = dynamic_cast<RooRealVar*>(temp)) {
			if (StringUtils::isSubstring(rrv->GetName(), nameSubstring)){
				return rrv;
			}
		}
	}
	return nullptr;
}

// Finds RooRealVar in list by a name substring
RooAbsArg* RootHelper::findArgNameSubstring(RooAbsCollection* list, const char* nameSubstring) {
	TIterator* it = list->createIterator();
	while (TObject* temp = it->Next()) {
		if (RooAbsArg* rooAbsArg = dynamic_cast<RooAbsArg*>(temp)) {
			const char* name = rooAbsArg->GetName();
			if (StringUtils::isSubstring(name, nameSubstring)) {
				return rooAbsArg;
			}
		}
	}
	return NULL;
}

// Makes data points - a pair of Matrix (ascii columns) with column captions TList (TString)
std::pair<TMatrixD,TList*> RootHelper::rooPlotToMatrix(RooRealVar* axis, RooPlot* plot){
	Int_t numberOfColumns = 1; // originally we have one column just for channels

	// Calculate number of RooHists and RooCurves in RooPlot
	for (int i=0; i < plot->numItems(); i++){
		TObject* object = plot->getObject(i);
		if (RooHist* hist = dynamic_cast<RooHist*>(object)){
			numberOfColumns+=2; // RooHist has value and error column
		}
		if (RooCurve* curve = dynamic_cast<RooCurve*>(object)){
			numberOfColumns++; // RooCurve has only value column
		}
	}
	// Number of columns in data matrix is 1 (channels) + numberOfRooHists*2 (value, error) + numberOfRooCurves (value)
	Int_t currentColumn = 0;

	// Number of rows is just number of axis bins
	const RooAbsBinning* binning = &(axis->getBinning());
	Int_t numberOfRows = binning->numBins();

	// Matrix with data columns
	TMatrixD matrix(numberOfRows, numberOfColumns);
	TList* columnNames = new TList();

	// Fill first column with channel center bin coordinates
	TObjString* channelObjString = new TObjString("channel");
	columnNames->Add(channelObjString);
	TMatrixDColumn column(matrix, currentColumn); // get first column
	for (int i = 0; i < numberOfRows; i++){
		column(i) = (Double_t)binning->binCenter(i);
	}
	currentColumn++;

	// Iterate through RooHists and RooCurves in the list of RooPlots
	for (int p=0; p < plot->numItems(); p++){
		TObject* object = plot->getObject(p);
		if (RooHist* hist = dynamic_cast<RooHist*>(object)){
			TString columnValueName = TString::Format("%s_value", hist->GetName());
			TObjString* columnValueObjString = new TObjString(columnValueName.Data());
			columnNames->Add(columnValueObjString);
			TString columnErrorName = TString::Format("%s_error", hist->GetName());
			TObjString* columnErrorObjString = new TObjString(columnErrorName.Data());
			columnNames->Add(columnErrorObjString);
			TMatrixDColumn columnValue(matrix, currentColumn);
			TMatrixDColumn columnError(matrix, currentColumn+1);
			for (int i = 0; i < numberOfRows; i++){
				Double_t binCenter = binning->binCenter(i);
				columnValue(i) = hist->Eval(binCenter);
				columnError(i) = hist->GetErrorY(i);
			}
			currentColumn+=2;
		}
		if (RooCurve* curve = dynamic_cast<RooCurve*>(object)){
			columnNames->Add(new TObjString(curve->GetName()));
			TMatrixDColumn column(matrix, currentColumn);
			for (int i = 0; i < numberOfRows; i++){
				Double_t binCenter = binning->binCenter(i);
				column(i) = curve->Eval(binCenter);
			}
			currentColumn++;
		}
	}

	// Print matrix
	Debug("RootHelper::rooPlotToMatrix");

	#ifdef USEDEBUG
//		columnNames->Print();
//		matrix.Print();
	#endif

	return std::make_pair(matrix,columnNames);
}

RooArgList* RootHelper::getLinearIntensities(RooArgList* recursiveIntensities) {
	RooArgList* intensities = new RooArgList();

	// Add material pdfs intensities (for recursive sum)
	Double_t previousCoeff = 1;   // previous coefficient
	Double_t previousCoeffError = 0;  // previous coefficient error
	for (unsigned i=0; i < recursiveIntensities->getSize(); i++){
		TString nameString = TString::Format("%s_", recursiveIntensities->at(i)->GetName());
		const char* name = nameString.Data();
		const char* title = recursiveIntensities->at(i)->GetTitle();
		RooRealVar* linearIntensity = new RooRealVar(name, title, 0, "%");

		if (RooRealVar* coeff = dynamic_cast<RooRealVar*>(recursiveIntensities->at(i))){
			Double_t recursiveIntensity = coeff->getValV();
			Double_t recursiveIntensityError = coeff->getError();
			linearIntensity->setVal(previousCoeff*recursiveIntensity*100);

			Double_t error = sqrt(pow(recursiveIntensity*previousCoeffError,2) + pow(previousCoeff*recursiveIntensityError,2));
			linearIntensity->setError(error*100);

			previousCoeff *= 1.-recursiveIntensity;
			previousCoeffError = error;
		}
		intensities->add(*linearIntensity);
	}
	return intensities;
}

void RootHelper::setSigDigits(RooAbsReal* rooAbsReal, Int_t sigDigits){
	rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_0, kFALSE);
	rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_1, kFALSE);
	rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_2, kFALSE);
	rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_3, kFALSE);
	rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_4, kFALSE);
	rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_5, kFALSE);

	switch (sigDigits){
		case 0:
			rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_0, kTRUE);
			break;
		case 1:
			rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_1, kTRUE);
			break;
		case 2:
			rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_2, kTRUE);
			break;
		case 3:
			rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_3, kTRUE);
			break;
		case 4:
			rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_4, kTRUE);
			break;
		case 5:
			rooAbsReal->setAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_5, kTRUE);
			break;
	}
}

Int_t RootHelper::getSigDigits(RooAbsReal* rooAbsReal){
	if (rooAbsReal->getAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_0)) return 0;
	if (rooAbsReal->getAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_1)) return 1;
	if (rooAbsReal->getAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_2)) return 2;
	if (rooAbsReal->getAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_3)) return 3;
	if (rooAbsReal->getAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_4)) return 4;
	if (rooAbsReal->getAttribute(Constants::ATTR_FORMAT_SIG_DIGITS_5)) return 5;
	return 3;
}

TString* RootHelper::getUUID(){
	UInt_t randomInt = random->Rndm()*1E8;
	TString uuidString = TString::Format("%u", randomInt);
	return new TString(uuidString);
}
