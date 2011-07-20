#ifndef moduleMenuAction_H
#define moduleMenuAction_H

#include <QAction>
#include "moduleBase.h"

class moduleMenuAction: public QAction
{
    public: 
       //moduleMenuAction();
       moduleMenuAction(QObject*);
       moduleMenuAction(const QString &text, QObject *parent);
       ~moduleMenuAction();
       void setModuleBase(moduleBase *);
       moduleBase *getModuleBase();
    
    private:
       moduleBase *mod;
};

#endif
