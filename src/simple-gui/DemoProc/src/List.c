/*************************************************************************/
/** Copyright.															**/
/** FileName: List.c													**/
/** Author: Polarix														**/
/** Description: HMI demo for list control interface.					**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_List.h"
#include "SGUI_FontResource.h"
#include "Resource.h"

//=======================================================================//
//= User Macro definition.											    =//
//=======================================================================//
#define					NOTICE_TEXT_BUFFER_SIZE				(64)

//=======================================================================//
//= Static function declaration.									    =//
//=======================================================================//
static HMI_ENGINE_RESULT	HMI_DemoList_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT	HMI_DemoList_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT	HMI_DemoList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT	HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.									    =//
//=======================================================================//
static SGUI_ITEMS_ITEM		s_arrstListItems[] =		{	{SCR1_LIST_ITEM1, NULL},
															{SCR1_LIST_ITEM2, NULL},
															{SCR1_LIST_ITEM3, NULL},
															{SCR1_LIST_ITEM4, NULL},
															{SCR1_LIST_ITEM5, NULL},
															{SCR1_LIST_ITEM6, NULL},
															{SCR1_LIST_ITEM7, NULL}
														};
static SGUI_LIST_STRUCT		s_stDemoListObject = 		{0x00};

//=======================================================================//
//= Global variable declaration.									    =//
//=======================================================================//
HMI_SCREEN_ACTION		s_stDemoListActions =			{	HMI_DemoList_Initialize,
															HMI_DemoList_Prepare,
															HMI_DemoList_RefreshScreen,
															HMI_DemoList_ProcessEvent,
															HMI_DemoList_PostProcess
														};
HMI_SCREEN_OBJECT       g_stHMIDemo_List =				{	HMI_SCREEN_ID_DEMO_LIST,
															&s_stDemoListActions
														};

//=======================================================================//
//= Function define.										            =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoList_Initialize(SGUI_SCR_DEV* pstDeviceIF)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
    // Initialize list data.
    SGUI_SystemIF_MemorySet(&s_stDemoListObject, 0x00, sizeof(SGUI_LIST_STRUCT));
    // Title and font size must set before initialize list object.
    s_stDemoListObject.stLayout.iX = 0;
    s_stDemoListObject.stLayout.iY = 0;
    s_stDemoListObject.stLayout.iWidth = pstDeviceIF->stSize.iWidth;
    s_stDemoListObject.stLayout.iHeight = pstDeviceIF->stSize.iHeight;
    s_stDemoListObject.szTitle = SCR1_TITLE;
     //Initialize list object.
	SGUI_List_Initialize(&s_stDemoListObject, &GB2312_FZXS12, s_arrstListItems, sizeof(s_arrstListItems)/sizeof(SGUI_ITEMS_ITEM));
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_Prepare (SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
	/*----------------------------------*/
	/* Process							*/
	/*----------------------------------*/
	SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
	/*----------------------------------*/
	/* Variable Declaration				*/
	/*----------------------------------*/
	HMI_ENGINE_RESULT           eProcessResult;
	SGUI_UINT16					uiKeyCode;
	SGUI_UINT16					uiKeyValue;
	KEY_PRESS_EVENT*			pstKeyEvent;
	SGUI_INT					iProcessAction;

	/*----------------------------------*/
	/* Initialize						*/
	/*----------------------------------*/
	eProcessResult =			HMI_RET_NORMAL;
	pstKeyEvent =				(KEY_PRESS_EVENT*)pstEvent;
	iProcessAction =			HMI_DEMO_PROC_NO_ACT;

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
			uiKeyCode = pstKeyEvent->Data.uiKeyValue;
			uiKeyValue = KEY_CODE_VALUE(uiKeyCode);
			switch(uiKeyValue)
			{
				case KEY_VALUE_ENTER:
				{
					iProcessAction = HMI_DEMO_PROC_CONFIRM;
					break;
				}
				case KEY_VALUE_ESC:
				{
					iProcessAction = HMI_DEMO_PROC_CANCEL;
					break;
				}
				case KEY_VALUE_UP:
				{
					if(s_stDemoListObject.stItems.iSelection > 0)
					{
						s_stDemoListObject.stItems.iSelection -= 1;
					}
					SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
					break;
				}
				case KEY_VALUE_DOWN:
				{
					if(s_stDemoListObject.stItems.iSelection < s_stDemoListObject.stItems.iCount-1)
					{
						s_stDemoListObject.stItems.iSelection += 1;
					}
					SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
	if(NULL != piActionID)
	{
		*piActionID = iProcessAction;
	}

	return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
	if(HMI_DEMO_PROC_CONFIRM == iActionID)
	{
        switch(s_stDemoListObject.stItems.iSelection)
        {
        	case 0:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_BASIC_PAINT, NULL);
				break;
			}
			case 1:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_PAINT_TEXT, NULL);
				break;
			}
			case 2:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_VARIABLE_BOX, NULL);
				break;
			}
			case 3:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH, NULL);
				break;
			}
			case 4:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_MENU, NULL);
				break;
			}
			case 5:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_TEXT_NOTICE, SCR7_NOTICE_TEXT);
				break;
			}
			case 6:
			{
				HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_CURVE, NULL);
				break;
			}
			default:
			{

				/* do nothing. */
			}
        }
	}
	else if(HMI_DEMO_PROC_CANCEL == iActionID)
	{
		HMI_GoBack(NULL);
	}

	return HMI_RET_NORMAL;
}

