#pragma once

#include <stdio.h>
#include <string.h>

#include <devices/serial.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <graphics/gfx.h>
#include <graphics/gfxbase.h>
#include <graphics/gels.h>
#include <graphics/collide.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>

#include <libraries/gadtools.h>

#include <proto/alib.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#include "gadgets.h"
#include "reversi/reversi.h"
#include "serial.h"

//OS4 list functions missing from OS3
//#define NewList(list) ((list)->lh_Head = (struct Node *)&(list)->lh_Tail,(list)->lh_TailPred = (struct Node *)&(list)->lh_Head,(list)->lh_Tail = NULL)
#define GetHead(list) ((list) && (list)->lh_Head && (list)->lh_Head->ln_Succ ? (list)->lh_Head : NULL)
#define GetTail(list) ((list) && (list)->lh_TailPred && (list)->lh_TailPred->ln_Pred ? (list)->lh_TailPred : NULL)
#define GetSucc(node) ((node) && (node)->ln_Succ->ln_Succ ? (node)->ln_Succ : NULL)
#define GetPred(node) ((node) && (node)->ln_Pred->ln_Pred ? (node)->ln_Pred : NULL)

#define GAD_VINFO(GAD_NAME) GAD_NAME.ng_VisualInfo = vInfo

struct TextAttr Topaz8 =
{
    "topaz.font", 8, 0, 0
};

struct Gadget *gadList, *gadTemp, *gadgets[16]; //gadList is internal, gadgets is mine
struct Screen *pubScreen, *myScreen;
struct Window *mainWindow;

BOOL ApplicationRequestsClose;

//flood fill
#define MAXVEC 50
struct TmpRas tmpRas;
struct AreaInfo areaInfo;
ULONG rassize;
UBYTE *tmpBuf;
UBYTE *areaBuf;

//gadtools
APTR vInfo;

void SetupGadgets(struct Window *window);
void MainWindowEventLoop(struct Window *window);
void Event_ProcessGadgetUp(struct IntuiMessage *msg);
void Event_HandleMouseClick(struct Window *window, UWORD x, UWORD y);

/* requesters */
struct EasyStruct es_CriticalError = {
	sizeof(struct EasyStruct),
	0,
	"Critical Error",
	"Error: %s",
	"Exit",
};
