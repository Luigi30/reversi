#include "serial.h"

void SerialPort::SendString(STRPTR str){
	SerialIO->IOSer.io_Length	= -1;
	SerialIO->IOSer.io_Data		= str;
	SerialIO->IOSer.io_Command	= CMD_WRITE;
	DoIO((struct IORequest *)SerialIO);
}

void SerialPort::OpenPort(){
    printf("SerialPort()\n");

    SerialMP = CreateMsgPort();
    if(SerialMP == NULL){
        printf("Failed to CreateMsgPort\n");
    }

    SerialIO = (struct IOExtSer *)CreateExtIO(SerialMP, sizeof(struct IOExtSer));
    if(SerialIO == NULL){
        printf("Failed to CreateExtIO\n");
    }

    printf("Opening device\n");
    OpenDevice(SERIALNAME, 0, (struct IORequest *)SerialIO, 0L);
}

void SerialPort::ClosePort(){
    CloseDevice((struct IORequest *)SerialIO);
    DeleteExtIO((struct IORequest *)SerialIO);
    DeleteMsgPort(SerialMP);
}

SerialPort serialPort;