/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileUtils.cpp
 * Author: petrstepanov
 * 
 * Created on August 2, 2017, 5:46 AM
 */

#include "FileUtils.h"
#include <TDatime.h>
#include <TString.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <RooPlot.h>
#include "../model/Constants.h"
#include "TMath.h"
#include "TMatrixD.h"

FileUtils::FileUtils(){};

FileUtils::FileUtils(FileUtils const& fileUtil){};

FileUtils* FileUtils::instance = NULL;

FileUtils* FileUtils::getInstance(){
	if (!instance){
		instance = new FileUtils;
	}
	return instance;
}

TH1F* FileUtils::importTH1(const char* fileName, int eColumn, int cColumn){
	std::ifstream ifs;
	std::string str;

	ifs.open(fileName, std::ifstream::in);

	if (!ifs.is_open()){
		std::cout << "importTH1: file \"" << fileName << "\" not found." << std::endl;
		return NULL;
	}

	if (eColumn == cColumn){
		std::cout << "importTH1: columns should have different numbers." << std::endl;
		return NULL;
	}

	std::map<double, double> spectrum;
	Int_t bins = 0;
	Double_t energyMin, energyMax;
	std::cout << "importTH1: energy column - " << eColumn << ", counts column - " << cColumn << std::endl;
	while (getline(ifs, str)) {
		Double_t energy;
		Double_t count;
		Bool_t hasEnergy = false; // We write spectrum if read energy and count
		Bool_t hasCount = false; // We write spectrum if read energy and count
		std::stringstream ss(str);
		Int_t col = 1;
		// Read columns from single line until eol or got both energy and count values
		while (!ss.eof() || !(hasEnergy && hasCount)){
			if (col == eColumn){
				if (ss >> energy){
					hasEnergy = true;
				}
			}
			else if (col == cColumn){
				if (ss >> count){
					hasCount = true;
				}
			}
			else {
				Double_t temp;
				if (!(ss >> temp)){ // If couldn't read temp column - break;
					break;
				}
			}
			col++;
			// skip comma or tab
			int c = ss.peek();
			if (c == ',' || c == 9){
				ss.ignore();
			}
		}
		// If successfully read energy and column from a single line - write to spectrum
		if (hasEnergy && hasCount){
			spectrum[energy] = count;
			bins++;
			// Remember Minimum and Maximum energy
			if (bins == 1) {
				energyMin = energy;
				energyMax = energy;
			}
			else {
				if (energy < energyMin){
					energyMin = energy;
				}
				if (energy > energyMax){
					energyMax = energy;
				}
			}
		}
	}
	ifs.close();

	if (bins == 0){
		return NULL;
	}
	// Calculate Bin Width. Histogram Has Bins. Bin Value is actually the value in the middle of the Bin.
	Double_t binWidth = (energyMax - energyMin) / (bins - 1);

	// Make Histogram
	Int_t id = (new TDatime())->Get();
	TH1F* hist = new TH1F(TString::Format("hist%d", id).Data(), "", bins, energyMin - binWidth / 2, energyMax + binWidth / 2);
	// TH1I (const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup)
	// bin = 0;       underflow bin
	// bin = 1;       first bin with low-edge xlow INCLUDED
	// bin = nbins;   last bin with upper-edge xup EXCLUDED
	// bin = nbins+1; overflow bin
	int i = 0;
	std::map<double, double>::iterator it;
	for (it = spectrum.begin(); it != spectrum.end(); ++it){
		Double_t count = it->second;
//		Double_t error = sqrt(count);

		// Count can't be zero for chi2Fit
//		if (count <= 0) {
//			count = 0;
//			error = 1;
//		}

		// Read spectrum data
		hist->SetBinContent(++i, count);
//		hist->SetBinError(i, error);
		//std::cout << "Bin " << i << " error " << error << std::endl;
	}

	return hist;
}

void FileUtils::savePlotsToFile(RooPlot* spectrumPlot, RooPlot* residualsPlot, const char* fileName, RooRealVar* observable){
	std::pair<TMatrixD,TList*> spectrumMatrixAndHeader = RootHelper::rooPlotToMatrix(observable, spectrumPlot);
	std::pair<TMatrixD,TList*> residualsMatrixAndHeader = RootHelper::rooPlotToMatrix(observable, residualsPlot);
	TMatrixD spectrumMatrix = (spectrumMatrixAndHeader.first);
	TMatrixD residualsMatrix = (residualsMatrixAndHeader.first);

	Int_t numberOfRows = TMath::Min(spectrumMatrix.GetNrows(), residualsMatrix.GetNrows());
	Int_t numberOfColumns = spectrumMatrix.GetNcols() + residualsMatrix.GetNcols();

	TMatrixD matrix(numberOfRows, numberOfColumns);
	for (Int_t j=0; j<numberOfRows; j++){
		for (Int_t i=0; i<numberOfColumns; i++){
			if (i<spectrumMatrix.GetNcols()){
				matrix(j,i) = spectrumMatrix(j,i);
			}
			else {
				matrix(j,i) = residualsMatrix(j,i-spectrumMatrix.GetNcols());
			}
		}
	}

	std::ofstream outputFile;
	outputFile.open(fileName);

	// Print header names to file
	std::string delimeter = "\t";

	TList* columnNames = new TList();
	columnNames->AddAll(spectrumMatrixAndHeader.second);
	columnNames->AddAll(residualsMatrixAndHeader.second);

	for (Int_t i=0; i<columnNames->GetSize(); i++){
		TObject* object = columnNames->At(i);
		if (TObjString* str = dynamic_cast<TObjString*>(object)){
			outputFile << (str->String()).Data();
		}
		if (i != columnNames->GetSize()-1){
			outputFile << delimeter.c_str();
		}
	}

	outputFile << std::endl;

	// Print matrix to file
	for (Int_t j=0; j<numberOfRows; j++){
		for (Int_t i=0; i<numberOfColumns; i++){
			if (i != numberOfColumns-1){
				outputFile << matrix(j,i) << delimeter.c_str();
			} else {
				outputFile << matrix(j,i);
			}
		}
		outputFile << std::endl;
	}
}

