#include "main.h"

struct DosLibrary *DOSBase = NULL;

UWORD pens[] = { (short unsigned int)-1 };

int main(int argc, char **argv){
	serialPort = SerialPort();
	serialPort.OpenPort();
	serialPort.SendString("*** Connected ***\r\n");
	
	myScreen = OpenScreenTags(NULL,
		SA_DisplayID,	(DEFAULT_MONITOR_ID | LORES_KEY),
		SA_Pens, 		(ULONG)pens,
		SA_Depth,		4,
		TAG_DONE);		
	
	mainWindow = OpenWindowTags(NULL,
        WA_Left,        	0,
        WA_Top,         	0,
        WA_Width,       	320,
        WA_Height,      	200,
        WA_IDCMP,       	IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_GADGETDOWN | IDCMP_MOUSEBUTTONS | ARROWIDCMP,
        WA_Flags,       	WFLG_BORDERLESS | WFLG_BACKDROP,
        WA_Gadgets,     	gadList,
        WA_Title,       	NULL,
        WA_CustomScreen,	myScreen,
		TAG_DONE);
	//ShowTitle(myScreen, FALSE);
	
	/* Set up for flood fill */
	#define WINW 320           /* size of drawing area */
	#define WINH 200
	rassize = RASSIZE(mainWindow->GZZWidth, mainWindow->GZZHeight);

	tmpBuf = AllocVec(rassize, MEMF_CHIP|MEMF_CLEAR);
	InitTmpRas(&tmpRas, tmpBuf, rassize);
	mainWindow->RPort->TmpRas = &tmpRas;

	areaBuf = AllocVec(5*MAXVEC, MEMF_CLEAR);
	InitArea(&areaInfo, areaBuf, MAXVEC);
	mainWindow->RPort->AreaInfo = &areaInfo;

	board.initForNewGame();
	gameState = GAME_TURN_WHITE;
	REV_DrawBoard(mainWindow);
	REV_DrawPieces(mainWindow);
	REV_DrawScores(mainWindow);
		
	GT_RefreshWindow(mainWindow, NULL);
	MainWindowEventLoop(mainWindow);
	
	//Done with the main loop, clean up and exit.
	if(tmpBuf){
		mainWindow->RPort->TmpRas = NULL;
		FreeVec(tmpBuf);
	}
	if(areaBuf){
		mainWindow->RPort->AreaInfo = NULL;
		FreeVec(areaBuf);
	}

	CloseWindow(mainWindow);
	CloseScreen(myScreen);

    //if(vInfo) FreeVisualInfo(vInfo);
	//if(gadList) FreeGadgets(gadList);

	serialPort.SendString("*** Disconnected ***\r\n");
	serialPort.ClosePort();
	
	return 0;
}

void MainWindowEventLoop(struct Window *window){
	struct IntuiMessage *msg;
	ULONG msgClass;
	
	ULONG windowSignal = (1L << window->UserPort->mp_SigBit);
	ULONG waitSignals = windowSignal;
	
	int CLOSE_WINDOW = FALSE;
	ApplicationRequestsClose = FALSE;
    while(CLOSE_WINDOW == FALSE && ApplicationRequestsClose == FALSE){
		msg = GT_GetIMsg(window->UserPort);

		if(!msg) {
        	Wait(waitSignals); //Wait for an event.
			continue;
		}

		//Process the window event.
		msgClass = msg->Class;

		switch(msgClass) {
			case IDCMP_CLOSEWINDOW:
				CLOSE_WINDOW = TRUE;
				break;
			case IDCMP_GADGETUP: //A button was clicked.
				Event_ProcessGadgetUp(msg);
				break;
			case IDCMP_MOUSEBUTTONS:
				if(msg->MouseX == 0) {
					CLOSE_WINDOW = TRUE;
				}	
				if(msg->Code == SELECTDOWN){
					Event_HandleMouseClick(window, msg->MouseX, msg->MouseY);					
				}
				break;
			default:
				//printf("Unhandled msgClass: %lx\n", msgClass);
				break;
		}

		GT_ReplyIMsg(msg); //Reply to the message.

	}
}

void SetupGadgets(struct Window *window){
	vInfo = GetVisualInfo(pubScreen, TAG_DONE);
	if(!(gadTemp = CreateContext(&gadList))) {
		char error[] = "Failed to create GadTools context.";
		Write(Output(), error, strlen(error));
	}
	
	GAD_VINFO(gad_txt_HelloWorld);
	gadgets[GAD_TXT_HELLOWORLD] = gadTemp = CreateGadget(TEXT_KIND, gadTemp, &gad_txt_HelloWorld, TAG_DONE);
	
	GAD_VINFO(gad_btn_Exit);
	gadgets[GAD_BTN_EXIT] = gadTemp = CreateGadget(BUTTON_KIND, gadTemp, &gad_btn_Exit, TAG_DONE);
}

void Event_ProcessGadgetUp(struct IntuiMessage *msg) {
	struct Gadget *gadget = (struct Gadget *)msg->IAddress;
	
	switch(gadget->GadgetID){
		case GAD_BTN_EXIT:
			ApplicationRequestsClose = TRUE;
			break;
		default:
			break;
	}
}

void Event_HandleMouseClick(struct Window *window, UWORD x, UWORD y){	
	SetAPen(window->RPort, PEN_WHITE);
	
	char clickStr[32];
	sprintf(clickStr, "X %3d Y %3d", x, y);
	
	Move(window->RPort, 0, 24);
	Text(window->RPort, clickStr, strlen(clickStr));
	
	REV_HandleTileClick(window, x, y);
}
