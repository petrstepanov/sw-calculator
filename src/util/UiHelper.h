/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UiHelper.h
 * Author: petrstepanov
 *
 * Created on October 11, 2017, 1:31 AM
 */

#ifndef UIHELPER_H
#define UIHELPER_H

#include <TGFileDialog.h>

class UiHelper {
public:
    UiHelper();
    UiHelper(const UiHelper& orig);
    virtual ~UiHelper();
    
    static TGFileInfo* getFileFromDialog();
    static void showOkDialog(const char* message);
private:

};

#endif /* UIHELPER_H */

