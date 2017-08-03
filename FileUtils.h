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

class FileUtils {
public:
	static FileUtils* getInstance();
	TH1I* importTH1(const char*, int, int);
	void saveData(TString*, TH1I*, RooCurve*, RooCurve*, TH1F*, TH1F*);
	static constexpr int prec = 6; // Output values precision

private:
	FileUtils();                                  // Private so that it can  not be called
	FileUtils(FileUtils const&);                  // Copy constructor is private
	FileUtils& operator=(FileUtils const&);       // Assignment operator is private
	static FileUtils* instance;
};

#endif /* FILEUTILS_H */

