#include "main.hpp"

struct DosLibrary *DOSBase = NULL;

int main(int argc, char **argv){
	
	pubScreen = LockPubScreen(NULL);
	//SetupGadgets(mainWindow);
	
	mainWindow = OpenWindowTags(NULL,
        WA_Left,        	20,
        WA_Top,         	20,
        WA_Width,       	400,
        WA_Height,      	400,
        WA_IDCMP,       	IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_GADGETDOWN | IDCMP_MOUSEMOVE | ARROWIDCMP,
        WA_Flags,       	WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET,
        WA_Gadgets,     	gadList,
        WA_Title,       	"Reversi",
        WA_PubScreenName,   "Workbench",
		TAG_DONE);
		
	REV_DrawBoard(mainWindow);
		
	GT_RefreshWindow(mainWindow, NULL);
	MainWindowEventLoop(mainWindow);
	
	//Done with the main loop, clean up and exit.
	CloseWindow(mainWindow);
	if(pubScreen) UnlockPubScreen(NULL, pubScreen);
    if(vInfo) FreeVisualInfo(vInfo);
	if(gadList) FreeGadgets(gadList);	
	
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
        ULONG signal = Wait(waitSignals); //Wait for an event.

        if(signal & windowSignal) {
            //Process the window event.
            msg = GT_GetIMsg(mainWindow->UserPort);
            msgClass = msg->Class;
            GT_ReplyIMsg(msg); //Reply to the message.

            switch(msgClass) {
                case IDCMP_CLOSEWINDOW:
                    CLOSE_WINDOW = TRUE;
                    break;
                case IDCMP_GADGETUP: //A button was clicked.
                    Event_ProcessGadgetUp(msg);
                    break;
                default:
                    //printf("Unhandled msgClass: %lx\n", msgClass);
                    break;
            }
		}
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
