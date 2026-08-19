////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      File: dxerror.h
//
//    Author: Eric Gessner
//
//   Purpose: A DirectX error handler.
//
//  Contents: Header file for dxerror.cpp. Contains the class definition for
//				  DxError and DxErrorHandler classes.
//				  (Refer to ErrorDoc.txt data and member functions and examples).
//
//  Design:   DxError class is designed for an error.
//            DxErrorHandler class is designed to handle a DxError.
//
//
//  Requirements: (reference dxerror.cpp for implementation).
//
//		  1) Lookup DirectX error data (contained in a file) for the error.
//      2) Display a MessageBox with the following information:
//            a) The constant identifier (e.g. "DDERR_ALREADYINITIALIZED").
//			     b) A description of the error (as contained in the help file).
//            c) Amplifying Data (user supplied amplifyies origin or nature).
//      3) The error handler will automatically call a user supplied clean up
//			  function (when pointer to clean up function passed in).
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef DXERROR_H
#define DXERROR_H

#include <stdio.h>
#include <string.h>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
// constants
////////////////////////////////////////////////////////////////////////////////

#define MAX_DXERR_DESC_LEN            300  //max len of description
#define MAX_DXERR_CONST_LEN           50   //max len text-representation
#define MAX_DXERR_AMPLIFYING_DATA_LEN 60   //max len of amplifying data
#define DXERR_DATA_FILE               "dxerror.dat" //the error data file

////////////////////////////////////////////////////////////////////////////////
// data types
////////////////////////////////////////////////////////////////////////////////

       //error Types
typedef enum
{ DXERR_FATAL=(-2),  //fatal
  DXERR_WARNING,     //warning
  DXERR_ANYERROR     //other
} DxErrorType;

		 //Function Pointer - function to call after error is reported
typedef void (*DxErrorCleanUpFunct)();

////////////////////////////////////////////////////////////////////////////////
// DxError Class Definition - an error.
////////////////////////////////////////////////////////////////////////////////
class DxError
{
public:              //constructors
    DxError(const HRESULT& eNum=0) { Num = eNum; }
    DxError(const HRESULT& eNum, const char *eStr);
    DxError(const HRESULT& eNum, const char *constText, const char *eStr);

                     //members to set information
    void SetData(HRESULT eNum, const char *constText, const char *eStr);
    void SetNum(HRESULT eNum) { Num = eNum; }
    void SetConstText(const char *constText);
    void SetDesc(const char *eStr);

                     //members to retrieve information
    const HRESULT GetNum() { return Num; }
    const char* GetConstText() { return ConstText; }
    const char* GetDesc() { return Desc; }

                     //member to test information
    bool IsNum(const HRESULT eNum) { return (Num == eNum) ? true : false; }

protected:           //overloaded I/O ops to store and retrieve error data
	 friend FILE* operator >>(FILE* is, DxError& dxerror);
	 friend FILE* operator <<(FILE* os, DxError& dxerror);

private:             //private data members
    HRESULT Num;      							 //the error number
    char ConstText[MAX_DXERR_CONST_LEN];//the error const-text representation
    char Desc[MAX_DXERR_DESC_LEN];      //a description of the error
};

////////////////////////////////////////////////////////////////////////////////
// DxErrorHandler Class Definition - the error handler.
////////////////////////////////////////////////////////////////////////////////
class DxErrorHandler
{
public:                 // constructor
    DxErrorHandler(
                   const HRESULT errorNum=0,               // error number
				       DxErrorType errorType = DXERR_ANYERROR, // error type
                   const char *AmpData = NULL, 				  // amplifying data
                   DxErrorCleanUpFunct cleanUpFunct = NULL // function to call
                  );
                       // member to handle the error
    void HandleDxError(const HRESULT eNum=0);             // error number or
    																		 // default

    void HandleDxError(const HRESULT eNum,                // error number and
    						  DxErrorType eType);                // error type

    void HandleDxError(const HRESULT eNum,                // error number and
    						  const char *amplifyingData);       // amplifying data

    void HandleDxError(const HRESULT eNum,                // error number and
    						  DxErrorCleanUpFunct eFunct);       // function to call

    void HandleDxError(const HRESULT eNum,                // error number and
    						  DxErrorType eType,                 // error type and
                       const char *amplifyingData);       // amplifying data

    void HandleDxError(const HRESULT eNum,                // error number and
    						  DxErrorType eType,                 // error type
                       DxErrorCleanUpFunct eFunct);       // and funct to call

    void HandleDxError(const HRESULT eNum,                // error number and
    						  const char *amplifyingData,        // amplifying data
                       DxErrorCleanUpFunct eFunct);       // and funct to call

    void HandleDxError(const HRESULT eNum,                // error number and
    						  DxErrorType eType,                 // error type and
                       const char *amplifyingData,        // amplifying data and
                       DxErrorCleanUpFunct eFunct);       // funct to call

                       //member to retreive the amplifying data
    const char* GetAmplifyingData() { return AmplifyingData; }

    						  //member to set the error type
    void SetDxErrorType(DxErrorType ErrType) { dxErrType = ErrType; }

    						  //member to set the clean up function
    void SetDxErrorFunct(DxErrorCleanUpFunct funct) { dxErrFunct = funct; }

    BOOL GetErrorText(char *pBuffer) { return GetErrorData(pBuffer); };

private:
    DxError dxErr;            //an error (DxError class)
    DxErrorType dxErrType;    //the type of error
    									//member to set amplifying data
    void SetAmplifyingData(const char *str);
    BOOL GetErrorData(char *pBuffer = NULL);		//member to lookup the error data

    							                     //user supplied data
	 char AmplifyingData[MAX_DXERR_AMPLIFYING_DATA_LEN]; //extra information
    DxErrorCleanUpFunct dxErrFunct;                     //function to call
};

#endif