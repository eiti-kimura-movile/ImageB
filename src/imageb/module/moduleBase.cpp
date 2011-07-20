#include "moduleBase.h"

moduleBase :: moduleBase()
{
   _show_message = false;
   _show_input = false;
}

moduleBase :: ~moduleBase()
{  
}

moduleBase *moduleBase::clone()
{
 return new moduleBase;
}

void moduleBase::process()
{
}

void moduleBase :: setName(char mname[80])
{
  strcpy(name,mname);
}

char *moduleBase :: getName()
{
  return name;
}

void moduleBase :: setDescription(string desc)
{
  description = desc;
}

string moduleBase :: getDescription()
{
  return description;
}

void moduleBase :: setMessage(string mmessage)
{
  message = mmessage;
}

string moduleBase :: getMessage()
{
  return message;
}

void moduleBase :: setDlgTitle(char mdlg_title[50])
{
  strcpy(dlg_title,mdlg_title);
}

char *moduleBase :: getDlgTitle()
{
  return dlg_title;
}

void moduleBase::setData(Matrix mat)
{
  _mat = mat;
}

Matrix moduleBase::getData()
{
  return _mat;
}

bool moduleBase::showMessage()
{
   return _show_message;
}

void moduleBase::setShowMessage(bool _val)
{
   _show_message = _val;
}

bool moduleBase::showInput()
{
   return _show_input;
}

void moduleBase::setShowInput(bool _val)
{
   _show_input = _val;
}

