/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileUtils.h
 * Author: petrstepanov
 *
 * Created on August 2, 2017, 5:46 AM
 */

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <TH1.h>
#include <RooCurve.h>
#include <RooPlot.h>
#include <RooRealVar.h>

enum class InputFileType {
	Undefined,
	Maestro,
	CanberraSingle,
	CanberraCoincidence
};

class FileUtils {
public:
	static FileUtils* getInstance();
	void saveData(TString*, TH1F*, RooCurve*, RooCurve*, TH1F*, TH1F*);
    static void savePlotsToFile(RooPlot* spectrumPlot, RooPlot* residualsPlot, const char* fileName,  RooRealVar* observable);
	static constexpr int prec = 6; // Output values precision

	TH1F* importTH1(const char* path = "");

private:
	FileUtils();                                  // Private so that it can  not be called
	FileUtils(FileUtils const&);                  // Copy constructor is private
	FileUtils& operator=(FileUtils const&);       // Assignment operator is private
	static FileUtils* instance;

	InputFileType detectFileType(const char* fileNamePath);

	TH1F* importTH1Maestro(const char* fileNamePath);
	TH1F* importTH1Canberra(const char* fileNamePath, int columnEnergy = 1, int columnValue = 2);
};

#endif /* FILEUTILS_H */

