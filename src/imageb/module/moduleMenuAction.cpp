#include "moduleMenuAction.h"

/*
moduleMenuAction::moduleMenuAction()
{
}
*/

moduleMenuAction::moduleMenuAction(QObject *parent):QAction(parent)
{
  
}

moduleMenuAction::moduleMenuAction(const QString &text, QObject *parent):QAction(text,parent)
{
  
}



moduleMenuAction::~moduleMenuAction()
{
  delete(mod);
}


void moduleMenuAction::setModuleBase(moduleBase *m)
{
  mod = m;
}


moduleBase *moduleMenuAction::getModuleBase()
{
  return mod;
}
