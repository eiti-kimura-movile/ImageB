#ifndef MODULEPUSHBUTTON_H
#define MODULEPUSHBUTTON_H

#include <QPushButton>
#include "moduleBase.h"

class modulePushButton: public QPushButton
{
    public: 
       modulePushButton();
       ~modulePushButton();
       void setModuleBase(moduleBase *);
       moduleBase *getModuleBase();
    
    private:
       moduleBase *mod;
};

#endif
