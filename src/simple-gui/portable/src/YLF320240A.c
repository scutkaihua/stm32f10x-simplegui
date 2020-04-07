/*************************************************************************/
/** Copyright.															**/
/** FileName: SDKInterface.cpp											**/
/** Author: Polarix														**/
/** Description: SDK swap interface function defines.					**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "stm32f10x_type.h"
//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_SDK_SetPixel                                     **/
/** Purpose:		Set virtual device pixel register data.             **/
/** Params:																**/
/**	@ iX[in]:		Pixel x-coordinate on display panel.			**/
/**	@ iY[in]:		Pixel y-coordinate on display panel.			**/
/**	@ iPixelValue[out]:	Pixel value, 0 for clear, 1 for set.			**/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_SetPixel(int iX, int iY, int iPixelValue)
{

}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GetPixel									**/
/** Purpose:		Get a pixel value form virtual device register.     **/
/** Params:																**/
/**	@ iX[in]:		Pixel x-coordinate on display panel.			**/
/**	@ iY[in]:		Pixel y-coordinate on display panel.		    **/
/** Return:			Pixel state, 0 for cleared, 1 for set.              **/
/** Notice:			None.                                               **/
/*************************************************************************/
int SGUI_SDK_GetPixel(int iX, int iY)
{

}

/*************************************************************************/
/** Function Name:	SGUI_SDK_RefreshDisplay								**/
/** Purpose:		Refresh virtual device display.                     **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_RefreshDisplay(void)
{

}

/*************************************************************************/
/** Function Name:	SGUI_SDK_ClearDisplay								**/
/** Purpose:		Clear screen display.                               **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_ClearDisplay(void)
{

}

/*************************************************************************/
/** Function Name:	SGUI_SDK_SyncKeyEventData.							**/
/** Purpose:		Set key code value when key press event targets.	**/
/** Params:																**/
/**	@ uiKeyCode[in]:	Key code.										**/
/** Return:			None.												**/
/** Notice:			This function used to save and keep pressed key 	**/
/**					code value, must be called before SetEvnetFlag.		**/
/*************************************************************************/
void SGUI_SDK_SyncKeyEventData(unsigned int uiKeyCode)
{

}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GetKeyEventData.							**/
/** Purpose:		Set key code value when key press event targets.	**/
/** Params:			none.												**/
/** Return:			Last targeted key event value.						**/
/** Notice:			This function used to save and keep pressed key 	**/
/**					code value, must be called before SetEvnetFlag.		**/
/*************************************************************************/
//unsigned int SGUI_SDK_GetKeyEventData(void)
//{
//
//}




void CheckEventFlag(int idx)
{}


void SGUI_SDK_ConfigHearBeatTimer(int ms)
{}


void SGUI_SDK_EnableRTCInterrupt(bool bEnable)
{}