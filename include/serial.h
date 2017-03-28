#pragma once

#include "stdio.h"

#include <devices/serial.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <intuition/intuition.h>

#include <proto/alib.h>
#include <proto/dos.h>
#include <proto/exec.h>

class SerialPort {
    struct MsgPort *SerialMP;
    struct IOExtSer *SerialIO;

    public:
    void OpenPort();
    void ClosePort();

    void SendString(STRPTR str);
};

extern SerialPort serialPort;