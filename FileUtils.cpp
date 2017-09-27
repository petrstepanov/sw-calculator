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
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

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
	TH1F* hist = new TH1F("hist", "", bins, energyMin - binWidth / 2, energyMax + binWidth / 2);
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

void FileUtils::saveData(TString* outFileName, TH1F* hist, RooCurve* bg, RooCurve* fit, TH1F* chi2, TH1F* peakHistNoBg){
	std::ofstream outputFile;
	outputFile.open(outFileName->Data());
	outputFile.precision(prec);

	Double_t nBins = hist->GetXaxis()->GetNbins();

	// Write file header
	outputFile << "Energy [KeV]" << ","
		<< "Count" << ","
		<< "Count Err" << ","
		<< "Fit" << ","
		<< "Chi^2" << ","
		<< "Atan Background" << ","
		<< "Count-Bg" << ","
		<< "Count-Bg Err" << std::endl;

	for (int i = 1; i <= nBins; i++){
		Double_t energy = hist->GetXaxis()->GetBinCenter(i);
		Int_t count = hist->GetBinContent(i);
		Double_t count_err = hist->GetBinError(i);
		Double_t fit_count = fit->Eval(energy);
		Double_t chi_2 = chi2->GetBinContent(i);

		Double_t fit_bg = bg->Eval(energy);
		Double_t count_min_bg = peakHistNoBg->GetBinContent(i);
		Double_t count_min_bg_err = peakHistNoBg->GetBinContent(i);

		outputFile << energy << ","
			<< count << ","
			<< count_err << ","
			<< fit_count << ","
			<< chi_2 << ","
			<< fit_bg << ","
			<< count_min_bg << ","
			<< count_min_bg_err << std::endl;
	}
	outputFile.close();
}
