#pragma once

struct TextAttr topaz8 = { (STRPTR)"topaz.font", 8, 0, 1 };

enum GADGET_INDEX {
	GAD_TXT_HELLOWORLD,
	GAD_BTN_EXIT
};

/* Gadget definitions. */
struct NewGadget gad_txt_HelloWorld = {
	125, 10, 			//LeftEdge, TopEdge
	250, 14,			//Width, Height
	(UBYTE*)"Hello YOSPOS! This is GCC 6.3.1!",	//GadgetText
	&topaz8,			//TextAttr
	GAD_TXT_HELLOWORLD,	//GadgetID
	PLACETEXT_IN,		//Flags
	NULL,				//VisualInfo ptr
	NULL				//APTR UserData (user field)
};

struct NewGadget gad_btn_Exit = {
	200, 60,
	100, 14,
	(UBYTE*)"Hooray!",
	&topaz8,
	GAD_BTN_EXIT,
	PLACETEXT_IN,
	NULL,
	NULL
};
