/*************************************************************************/
/** Copyright.															**/
/** FileName: RTCNotice.c												**/
/** Author: Polarix														**/
/** Version: 1.0.0.0													**/
/** Description: HMI demo for notice box interface and refresh screen.	**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"
#include "HMI_Engine.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static HMI_ENGINE_RESULT	HMI_DemoBasic_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoBasic_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoBasic_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT	HMI_DemoBasic_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
HMI_SCREEN_ACTION		s_stDemoBasicPaintActions =		{	NULL,
															HMI_DemoBasic_Prepare,
															HMI_DemoBasic_RefreshScreen,
															HMI_DemoBasic_ProcessEvent,
															HMI_DemoBasic_PostProcess
														};
//=======================================================================//
//= Global variable declaration.									    =//
//=======================================================================//
HMI_SCREEN_OBJECT       g_stHMIDemo_BasicPaint =		{	HMI_SCREEN_ID_DEMO_BASIC_PAINT,
															&s_stDemoBasicPaintActions
														};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoBasic_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(NULL != pstDeviceIF->fnClear)
	{
		pstDeviceIF->fnClear();
	}
	else
	{
		SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, pstDeviceIF->stSize.iWidth, pstDeviceIF->stSize.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
	}
	HMI_DemoBasic_RefreshScreen(pstDeviceIF, pstParameters);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoBasic_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_Basic_DrawLine(pstDeviceIF, 0, 10, 96, 10, SGUI_COLOR_FRGCLR);
	SGUI_Basic_DrawRectangle(pstDeviceIF, 5, 5, 50, 50, SGUI_COLOR_FRGCLR, SGUI_COLOR_TRANS);
	SGUI_Basic_DrawCircle(pstDeviceIF, 32, 32, 20, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
	SGUI_Basic_DrawCircle(pstDeviceIF, 96, 32, 20, SGUI_COLOR_FRGCLR, SGUI_COLOR_TRANS);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoBasic_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	SGUI_INT					iProcessAction;
	KEY_PRESS_EVENT*			pstKeyEvent;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	iProcessAction =			HMI_DEMO_PROC_NO_ACT;
	pstKeyEvent =				(KEY_PRESS_EVENT*)pstEvent;

	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(pstEvent->iType == EVENT_TYPE_ACTION)
	{
		// Check event is valid.
		if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
		{
			// Event data is invalid.
			eProcessResult = HMI_RET_INVALID_DATA;
		}
		else if(EVENT_ID_KEY_PRESS == pstEvent->iID)
		{
			iProcessAction = HMI_DEMO_PROC_CANCEL;
		}
	}
	if(NULL != piActionID)
	{
		*piActionID = iProcessAction;
	}

	return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoBasic_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	if(HMI_PROCESS_SUCCESSFUL(eProcResult))
	{
		if(HMI_DEMO_PROC_CANCEL == iActionID)
		{
			HMI_GoBack(NULL);
		}
	}

	return HMI_RET_NORMAL;
}

