////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      File: dxerror.cpp
//
//    Author: Eric Gessner
//
//   Purpose: A DirectX error handler.
//
//  Contents: This file contains the implementation for the DxError and
//				  DxErrorHandler classes (see dxerror.h for class definitions and
//				  ErrorDoc.txt data and member functions and examples).
//
//  Design:  Lookup DirectX error data (contained in a file) for the error.
//           Display a MessageBox with the following information:
//             a) The constant identifier (e.g. "DDERR_ALREADYINITIALIZED").
//				   b) A description of the error (as contained in the help file).
//             c) Amplifying Data (user supplied amplifyies origin or nature).
//           The error handler will automatically call a user supplied clean up
//				 function (when pointer to clean up function passed in).
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "dxerror.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
// DxError Class constructors and Class Member Implementation
////////////////////////////////////////////////////////////////////////////////

//DxError Class Constructor
DxError::DxError(const HRESULT& eNum, const char *eStr)
{
   Num = eNum;
   strcpy(Desc,eStr);
}

//DxError Class Constructor
DxError::DxError(const HRESULT& eNum, const char *constText, const char *eStr)
{
  Num = eNum;
  strcpy(ConstText,constText);
  strcpy(Desc,eStr);
}

//DxError SetData Member Function Implementation
void DxError::SetData(HRESULT eNum, const char *constText, const char *eStr)
{
  Num = eNum;
  strcpy(ConstText,constText);
  strcpy(Desc,eStr);
}

//DxError SetConstText Member Function Implementation
void DxError::SetConstText(const char *constText)
{
   strcpy(ConstText,constText);
}

//DxError SetDesc Member Function Implementation
void DxError::SetDesc(const char *eStr)
{
  strcpy(Desc,eStr);
}

//DxError File Data Input Operator
FILE* operator >>(FILE* is, DxError& dxerror)
{
  HRESULT eNum=0;
  char constText[MAX_DXERR_CONST_LEN],
       eStr[MAX_DXERR_DESC_LEN], chbuf[2];
  int i=0;

  fscanf(is,"%ld",&eNum);
  fscanf(is,"%c",&chbuf);      //eat eol ch
  fscanf(is,"%s",&constText);
  fscanf(is,"%c",&chbuf);      //eat eol from last line
  strcpy(chbuf," \0");     //init ch buf
  while (chbuf[0] != '\n' && i < MAX_DXERR_DESC_LEN)
  {
    fscanf(is,"%c",&chbuf);
    if (chbuf[0] != '\n')
    {
      eStr[i++] = chbuf[0];
    }
    else
      eStr[i] = '\0';
  }
  dxerror.SetData(eNum, constText, eStr);
  return is;
}

//DxError File Data Output Operator
FILE* operator <<(FILE* os, DxError& dxerror)
{
  fprintf(os,"%ld\n",dxerror.GetNum());
  fprintf(os,"%s\n",dxerror.GetConstText());
  fprintf(os,"%s\n",dxerror.GetDesc());
  return os;
}


////////////////////////////////////////////////////////////////////////////////
// DxErrorHandler Class constructors and Class Member Implementation
////////////////////////////////////////////////////////////////////////////////

//DxErrorHandler Constructor
DxErrorHandler::DxErrorHandler( const HRESULT errorNum,     
										  DxErrorType errorType,
                                const char *amplifyingData,
								 	     DxErrorCleanUpFunct cleanUpFunct
                              ) : dxErr(errorNum)
{
  dxErrType = errorType;
  if (amplifyingData != NULL)
    SetAmplifyingData(amplifyingData);
  dxErrFunct = cleanUpFunct;
}


//DxErrorHandler Overloaded HandleDxError Member Function

void DxErrorHandler::HandleDxError(const HRESULT eNum)
{
   dxErr = eNum;
   dxErrType = DXERR_ANYERROR;
   strset(AmplifyingData,'\0');
   dxErrFunct = NULL;
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum, DxErrorType eType)
{
   dxErr = eNum;
   dxErrType = eType;
   strset(AmplifyingData,'\0');
   dxErrFunct = NULL;
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum,
											  const char *amplifyingData)
{
   dxErr = eNum;
   SetAmplifyingData(amplifyingData);
   dxErrType = DXERR_ANYERROR;
   dxErrFunct = NULL;
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum,
											  DxErrorCleanUpFunct eFunct)
{
   dxErr = eNum;
   dxErrFunct = eFunct;
   dxErrType = DXERR_ANYERROR;
   strset(AmplifyingData,'\0');
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum,
											  DxErrorType eType,
                                   const char *amplifyingData)
{
   dxErr = eNum;
   dxErrType = eType;
   SetAmplifyingData(amplifyingData);
   dxErrFunct = NULL;
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum,
											  DxErrorType eType,
                                   DxErrorCleanUpFunct eFunct)
{
   dxErr = eNum;
   dxErrType = eType;
   dxErrFunct = eFunct;
   strset(AmplifyingData,'\0');
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum,
											  const char *amplifyingData,
                                   DxErrorCleanUpFunct eFunct)
{
   dxErr = eNum;
   SetAmplifyingData(amplifyingData);
   dxErrFunct = eFunct;
   dxErrType = DXERR_ANYERROR;
   GetErrorData();
}

void DxErrorHandler::HandleDxError(const HRESULT eNum,
											  DxErrorType eType,
                                   const char *amplifyingData,
                                   DxErrorCleanUpFunct eFunct)
{
   dxErr = eNum;
   dxErrType = eType;
   SetAmplifyingData(amplifyingData);
   dxErrFunct = eFunct;
   GetErrorData();
}

// DxErrorHandler::GetErrorData looks up in the error in the dxerror.dat file.
BOOL DxErrorHandler::GetErrorData(char *pBuffer)
{
  //dx error values are defined by call to MAKE_DDHRESULT(#)
  //which inturn calls MAKE_HRESULT() winerror.h macro
  //see dx header files and winerror.h for details

  char msgbuf[MAX_DXERR_DESC_LEN + MAX_DXERR_AMPLIFYING_DATA_LEN + 20];
  char capbuf[30 + MAX_DXERR_CONST_LEN];
  bool found=false, fileerror=false;
  FILE *dF;

  if (!dxErr.GetNum())  //if an error number was reported
    return FALSE;

    					 //open the errorfile
  if ((dF = fopen(DXERR_DATA_FILE,"r")) == NULL)
  {	   		      //if errorfile not open
    fileerror = true;
					    //format the cap and msg
    sprintf(msgbuf,"%s %ld %s\n",\
            "Error:",dxErr.GetNum(),"Can't open error data file.");
    sprintf(capbuf,"%s\n","ERROR DATA NOT AVALIABLE");

        				//display cant open file msg
    if (pBuffer)
      strcpy(pBuffer, msgbuf);
    else
      MessageBox(NULL, msgbuf, capbuf, MB_OK);
  }
  else             //else get the error data
  {
     while (!found && !feof(dF))
     {
        DxError Err;
        dF >> Err;                  //read in file data
        if (dxErr.IsNum(Err.GetNum()))  //if it matches the error reported
        {
           found = true;
               								//format error data
           if (AmplifyingData[0] == '\0')
             SetAmplifyingData("None Supplied");
           sprintf(msgbuf,"%s <Amplifying Data: %s >",\
							               	Err.GetDesc(),GetAmplifyingData());
           if (dxErrType == DXERR_WARNING)
             sprintf(capbuf,"WARNING - DirectX Error '%s'",\
                 												 Err.GetConstText());
           else if (dxErrType == DXERR_FATAL)
             sprintf(capbuf,"FATAL - DirectX Error '%s'",\
											                 	 Err.GetConstText());
           else
             sprintf(capbuf,"DirectX Error '%s'",Err.GetConstText());

                 							 //display error data
           if (pBuffer)
             strcpy(pBuffer, capbuf);
           else
           {
             int ret = MessageBox(NULL, msgbuf, capbuf, MB_ABORTRETRYIGNORE);

             if (ret == IDRETRY) __asm int 3h;
             if (ret == IDABORT) exit(-1);
                 							  //if funct to call, call it
             if (dxErrFunct != NULL)
                (*dxErrFunct)();
           }
        }
     }//while

     fclose(dF);                    //close the data file
  }//else
                                     //if error reported not found
  if (!fileerror && !found)
  {                                  //format cap and msg
    sprintf(msgbuf,"ERROR: %ld %s",dxErr.GetNum(),"no data found.");
    sprintf(capbuf,"%s","UNKNOWN DX ERROR REPORTED");
                                     //display error and not found msg
    if (pBuffer)
      strcpy(pBuffer, msgbuf);
    else
      MessageBox(NULL, msgbuf, capbuf , MB_OK);
  }

  return TRUE;
}

// DxErrorHandler::SetAmplifyingData copies amplifying str to AmplifyingData.
void DxErrorHandler::SetAmplifyingData(const char *str)
{
   int len = strlen(str);

   if (len)
   {
     if (len >= MAX_DXERR_AMPLIFYING_DATA_LEN)
     {
       strncpy(AmplifyingData,str,MAX_DXERR_AMPLIFYING_DATA_LEN-1);
       AmplifyingData[MAX_DXERR_AMPLIFYING_DATA_LEN-1] = '\0';
     }
     else
     {
       strncpy(AmplifyingData,str,len);
       AmplifyingData[len] = '\0';
     }
   }//if
}


