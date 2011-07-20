#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include "NumMeth.h"

using namespace std;

class moduleBase
{
 public:
   moduleBase();
   virtual ~moduleBase();
   virtual moduleBase *clone();
 
   void init_module();
   void setName(char mname[80]);
   char *getName();

   void setDescription(string desc);
   string getDescription();
 
   void setMessage(string mmessage);
   string getMessage();
  
   void setDlgTitle(char mdlg_title[50]);
   char *getDlgTitle();
 
   void setShowMessage(bool);
   void setShowInput(bool);
 
   bool showMessage();
   bool showInput();
 
   virtual void setData(Matrix);
   virtual Matrix getData();
 
   virtual void process();
   
 
 private:
   char name[80];
   string description;
   string message;
   char dlg_title[50];
  
   Matrix _mat;
   int _run;
   bool _show_message;
   bool _show_input;
 
   //bool visualImageMenu;
};

#endif

