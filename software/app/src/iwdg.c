#include "iwdg_proxy.h"


void iwdg_proxy_init()
{
  IWDG_KeyRegWrite(IWDG_KeyRegWrite_Enable);
  IWDG_SetPrescaler(IWDG_Psc_256);

  IWDG_SetReload(0xFFF);

  IWDG_ReloadCounter();

  IWDG_Enable();
}




void iwdg_proxy_feed_dog()
{
  IWDG_ReloadCounter();
}
