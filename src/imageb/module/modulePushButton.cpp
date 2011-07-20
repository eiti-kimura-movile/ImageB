#include "modulePushButton.h"

modulePushButton::modulePushButton()
{

}

modulePushButton::~modulePushButton()
{
  delete(mod);
}


void modulePushButton::setModuleBase(moduleBase *m)
{
  mod = m;
}


moduleBase *modulePushButton::getModuleBase()
{
  return mod;
}
