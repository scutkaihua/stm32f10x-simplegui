#ifndef _INCLUDE_RESOURCE_H_
#define _INCLUDE_RESOURCE_H_

#include "SGUI_Typedef.h"

#define _SIMPLE_GUI_DEMO_ONLY_ASCII_

#ifdef _SIMPLE_GUI_DEMO_ONLY_ASCII_
#include "DemoResource_ASCII.h"
#else
//#include "DemoResource_GB2312.h"
#include "DemoResource_UTF8.h"
#endif

extern const SGUI_FONT_RES GB2312_FZXS12;


#endif // _INCLUDE_RESOURCE_H_
