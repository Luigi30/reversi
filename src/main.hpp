#pragma once

#include <stdio.h>
#include <string.h>

#include <intuition/intuition.h>

#include <libraries/gadtools.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#include "gadgets.hpp"
#include "reversi.hpp"

//OS4 list functions missing from OS3
#define NewList(list) ((list)->lh_Head = (struct Node *)&(list)->lh_Tail,(list)->lh_TailPred = (struct Node *)&(list)->lh_Head,(list)->lh_Tail = NULL)
#define GetHead(list) ((list) && (list)->lh_Head && (list)->lh_Head->ln_Succ ? (list)->lh_Head : NULL)
#define GetTail(list) ((list) && (list)->lh_TailPred && (list)->lh_TailPred->ln_Pred ? (list)->lh_TailPred : NULL)
#define GetSucc(node) ((node) && (node)->ln_Succ->ln_Succ ? (node)->ln_Succ : NULL)
#define GetPred(node) ((node) && (node)->ln_Pred->ln_Pred ? (node)->ln_Pred : NULL)

#define GAD_VINFO(GAD_NAME) GAD_NAME.ng_VisualInfo = vInfo

struct Gadget *gadList, *gadTemp, *gadgets[16]; //gadList is internal, gadgets is mine
struct Screen *pubScreen;
struct Window *mainWindow;

BOOL ApplicationRequestsClose;

//gadtools
APTR vInfo;

void SetupGadgets(struct Window *window);
void MainWindowEventLoop(struct Window *window);
void Event_ProcessGadgetUp(struct IntuiMessage *msg);

/* requesters */
struct EasyStruct es_CriticalError = {
	sizeof(struct EasyStruct),
	0,
	"Critical Error",
	"Error: %s",
	"Exit",
};
