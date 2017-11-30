
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include "pci_1680.h"
#include "commdef.h"
#include <time.h>

typedef struct
{
      	unsigned short    	initFlag;
        unsigned char   	Owner_Set;
        int    				DeviceID;
        struct sigaction sig_act1, sig_act2; 
 //       struct sigaction sig_actErrorWarning1, sig_actDataOverrun1; 
 //       struct sigaction sig_actWakeUp1, sig_actPassiveEvent1; 
 //       struct sigaction sig_actArbitrationLost1, sig_actBusError1; 
 //       struct sigaction sig_actErrorWarning2, sig_actDataOverrun2; 
 //       struct sigaction sig_actWakeUp2, sig_actPassiveEvent2; 
 //       struct sigaction sig_actArbitrationLost2, sig_actBusError2; 
} Lib_PCI_Info;

typedef struct
{
  U16 ret_val;
  int can_sel;
  int offset;
  BYTE data;
} REG_RW;

typedef struct
{
  DWORD handle;
  DWORD mode;
} RETINFO;

typedef struct
{
  DWORD handle;
  U16 ret_val;
  CAN_PACKET msg;
} CAN_MSG;

typedef struct
{
  DWORD handle;
  U16 ret_val;
} REC_CN;

int initFlag=0, totalCards=0;
int LED_ADDR_START = 64;
WORD majorVersion = 0x00;
WORD minorVersion = 0x04;
Lib_PCI_Info pci_Info_1680[MAXSUPPORT];
int InstallRef[MAXSUPPORT*2] = { 0, 0, 0, 0, 0, 0, 0, 0};
PORT_STRUCT portStruct[MAXSUPPORT*2];
BYTE canIntStatus[MAXSUPPORT*2];
BYTE LEDStatus[MAXSUPPORT];

WORD  GetDriverVersion(void)
{
	return majorVersion<<8 | minorVersion;
}

int  _1680_InstallPCI(int card_number)
{
     int DevId;
    char buf[255];

    if (card_number>=MAXSUPPORT)return ERR_InvalidCardNumber;

    sprintf(buf,"/dev/PCI1680W%d",card_number);
    DevId = open( buf, O_RDWR );
    if (DevId == -1)
       return ERR_OpenFileFailed;

    memset( &(pci_Info_1680[card_number]), '\0', sizeof( Lib_PCI_Info ) );
    pci_Info_1680[card_number].DeviceID = DevId;
    pci_Info_1680[card_number].initFlag=0x1680;
    if( fcntl( DevId, F_SETOWN, getpid()) != -1 ){
    	pci_Info_1680[card_number].Owner_Set = 1;
        pci_Info_1680[card_number].sig_act1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_act2.sa_handler = SIG_IGN;
/*
        pci_Info_1680[card_number].sig_actErrorWarning1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actDataOverrun1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actWakeUp1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actPassiveEvent1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actArbitrationLost1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actBusError1.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actErrorWarning2.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actDataOverrun2.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actWakeUp2.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actPassiveEvent2.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actArbitrationLost2.sa_handler = SIG_IGN;
        pci_Info_1680[card_number].sig_actBusError2.sa_handler = SIG_IGN;
*/
        if( sigaction( SIGEVENT1, &(pci_Info_1680[0].sig_act1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT2, &(pci_Info_1680[0].sig_act2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
/*
        if( sigaction( SIGEVENT3, &(pci_Info_1680[0].sig_actErrorWarning1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT4, &(pci_Info_1680[0].sig_actDataOverrun1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT5, &(pci_Info_1680[0].sig_actWakeUp1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT6, &(pci_Info_1680[0].sig_actPassiveEvent1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT7, &(pci_Info_1680[0].sig_actArbitrationLost1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT8, &(pci_Info_1680[0].sig_actBusError1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT9, &(pci_Info_1680[0].sig_actErrorWarning2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT10, &(pci_Info_1680[0].sig_actDataOverrun2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT11, &(pci_Info_1680[0].sig_actWakeUp2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT12, &(pci_Info_1680[0].sig_actPassiveEvent2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT13, &(pci_Info_1680[0].sig_actArbitrationLost2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT14, &(pci_Info_1680[0].sig_actBusError2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
*/
    }
    else 
	pci_Info_1680[0].Owner_Set = 0;
    return(ERR_NoError);
}


int  CanGetReceiveEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

    if(pci_Info_1680[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) 
    {
	if( pci_Info_1680[handle/2].Owner_Set == 0) 
	   return ERR_OwnerSet; // check the owner setting
	if( event_handler == NULL ) 
	   return (ERR_SignalHandle);
	else{
	   if ((handle%2) == 0){
		pci_Info_1680[handle/2].sig_act1.sa_handler = event_handler;
		pci_Info_1680[handle/2].sig_act1.sa_flags |= SA_RESTART; 
		if( sigaction( SIGEVENT1, &(pci_Info_1680[handle/2].sig_act1), NULL ) == -1 )
			return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_1680[handle/2].DeviceID,IOCTL_1680_SIGNAL1_SET,&(enable_flag));
	   }
	   else {
		pci_Info_1680[handle/2].sig_act2.sa_handler = event_handler;
		pci_Info_1680[handle/2].sig_act2.sa_flags |= SA_RESTART; 
		if( sigaction( SIGEVENT2, &(pci_Info_1680[handle/2].sig_act2), NULL ) == -1 )
			return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_1680[handle/2].DeviceID,IOCTL_1680_SIGNAL2_SET,&(enable_flag));
	   }
	   if( ret_value == 0 )    
		return(ERR_NoError);
	   else 
		return (ERR_AllocateMemory);
	}
    }
    else
      return ERR_BoardNoInit;
}

int  CanStopReceiveEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_1680[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) 
    {
	if ((handle%2) == 0) {
	   ret_value = ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_SIGNAL1_SET, &enable_flag);
	   pci_Info_1680[handle/2].sig_act1.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT1, &(pci_Info_1680[handle/2].sig_act1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1){
	   ret_value = ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_SIGNAL2_SET, &enable_flag);
	   pci_Info_1680[handle/2].sig_act2.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT2, &(pci_Info_1680[handle/2].sig_act2), NULL ) ==-1 )	
		return(ERR_SignalNotify);
	}
        if( ret_value == 0 )    
	   return(ERR_NoError);
        else 
	   return (ERR_AllocateMemory);
    }
    else
      return ERR_BoardNoInit;
}


int  CanOpenDriver(int card, int port)
{
	int index;
	PORT_STRUCT setPort;	// 2006/06/05, for init config

	if(pci_Info_1680[card].initFlag==FALSE) {
		if (_1680_InstallPCI(card) != ERR_NoError) {
			printf("Couldn't find PCI-1680 card %d\n", card);
            		return -1;
		}
		else
			totalCards++;
	}    
        index = card*2+port;
//printf("CanOpenDriver index %d\n",index);
        InstallRef[index] = 1;
 
	// 2006/06/05, init config------------------------------------------------
        setPort.mode     = 0;       //  0 : 11-bit ;  1 : 29-bit CAN network
        setPort.accCode  = 0;       //  Only ID = accCode can pass the filter
        setPort.accMask  = 0xffffffff;       //  Don't care bit
        setPort.baudrate = 4;       //  0: 10kBps; 1: 20kBps; 2: 50kBps; 3: 100kBps; 4: 125kBps; 5: 250kBps; 6:500kBps; 7: 800kBps; 8:1MBps
        if (CanConfigPort(index, &setPort) < 0)
    { printf("CanConfig Port%d fail\n",index);
        }
	else
    { //printf("CanConfig Port%d OK\n",index);
        }
	// --------------------------------------------------------------------------

        return index;    	
}

int  CanCloseDriver(int handle)
{
	int anotherPort, count;
    	REG_RW temp_write_reg;

	if (InstallRef[handle] != 1)
		return -1;
	InstallRef[handle]=0;
	
	temp_write_reg.can_sel = handle;
     	temp_write_reg.offset = 0; //(handle%2)*0x200;
    	temp_write_reg.data = 0x01;
	ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_CLOSE_PORT, &temp_write_reg);
	anotherPort = 0;
	for(count=0;count<totalCards*2;count++)
	    if (InstallRef[count] == 1)
	        anotherPort = 1;
	if (anotherPort == 0)
	{
		temp_write_reg.offset = LED_ADDR_START;
		temp_write_reg.data = 0xFF;
		ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_CLOSE_CARD, &temp_write_reg);
	}
	return 0;
}

int  CanConfigPort(int handle, PORT_STRUCT *PortStruct)
{
    	BYTE temp;
	RETINFO RetInfo;
	//  Port has not installed
	if (InstallRef[handle] != 1)
		return -1;
//printf("CanConfigPort port %d\n",handle);
    	if (PortStruct == NULL)
        	return -1;
	if ((PortStruct->mode != 0) && (PortStruct->mode != 1))
		return -1;
//printf("Starting\n");
	//_1680_Write(handle, 0x100 ,0x01);  //???
	_1680_Write(handle, 0, 0x21);  //Enter reset mode
	
        int st=500;
	do
	{
		if((st--) <0) break;
		temp=_1680_Read(handle,0);
	        usleep(10000);
	}while((temp & 0x01) !=0x01);

	if( (temp & 0x01) != 0x01)
	{	
	     return -1;
	}
	_1680_Write(handle, 0x8, 0x5e);    //Reg OCR, set normal output mode
    _1680_Write(handle, 0x1f, 0x80);   //Reg CDR, set PeliCAN mode
	st = 500;
	do
        {       
                if((st--) <0) break;
                temp=_1680_Read(handle,0x1f);
                usleep(10000);
        }while((temp & 0x80) !=0x80);
        
	//	setup acceptance code & acceptance mask
	//	Standard frame, single filter mode

		//	Disable checking data field
	
        //      Fill Acceptance Code Register
        _1680_Write(handle, 16, (BYTE)(PortStruct->accCode));
        _1680_Write(handle, 17, (BYTE)(PortStruct->accCode >> 8));
        _1680_Write(handle, 18, (BYTE)(PortStruct->accCode >> 16));
        _1680_Write(handle, 19, (BYTE)(PortStruct->accCode >> 24));

        //      Fill Acceptance Mask Register
        _1680_Write(handle, 20, (BYTE)(PortStruct->accMask));
        _1680_Write(handle, 21, (BYTE)(PortStruct->accMask >> 8));
        _1680_Write(handle, 22, (BYTE)(PortStruct->accMask >> 16));
        _1680_Write(handle, 23, (BYTE)(PortStruct->accMask >> 24));
        
	st=1000;
	do
        {
                if((st--) <0) break;
                usleep(10000);
        }while(_1680_Read(handle,0x14) != 0xff);
	 //      125kBps
	switch(PortStruct->baudrate)
	{
                //      10kBps
                case 0: _1680_Write(handle, 6, 0x31);
                                _1680_Write(handle, 7, 0x1C);
                                break;
                //      20kBps
                case 1: _1680_Write(handle, 6, 0x18);
                                _1680_Write(handle, 7, 0x1C);
                                break;
                //      50kBps
                case 2: _1680_Write(handle, 6, 0x09);
                                _1680_Write(handle, 7, 0x1C);
                                break;
                //      100kBps
                case 3: _1680_Write(handle, 6, 0x04);
                                _1680_Write(handle, 7, 0x1C);
                                break;
		//	125kBps
		case 4:	_1680_Write(handle, 6, 0x03);
				_1680_Write(handle, 7, 0x1C);
				break;
		//	250kBps
		case 5:	_1680_Write(handle, 6, 0x01);
				_1680_Write(handle, 7, 0x1C);
				break;
		//	500kBps
		case 6:	_1680_Write(handle, 6, 0x00);
				_1680_Write(handle, 7, 0x1C);
				break;
                //      800kBps
                case 7: _1680_Write(handle, 6, 0x00);
                                _1680_Write(handle, 7, 0x16);
                                break;
		//	1MkBps
		case 8:	_1680_Write(handle, 6, 0x00);
				_1680_Write(handle, 7, 0x14);
				break;
		//	Self Defined Baudrate
		case 9: _1680_Write(handle, 6, PortStruct->brp | PortStruct->sjw << 6);
                		_1680_Write(handle, 7, PortStruct->sam << 8 | PortStruct->tseg2 << 4 | PortStruct->tseg1);
				break;
		default :   return -1;
	}

	//	setup output control register;  normal mode; push-pull
	//	return operation mode
	//	write port mode to VXD
	RetInfo.handle=handle;
	RetInfo.mode=PortStruct->mode;
    	ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_SET_PORT_MODE, &RetInfo);
	
	//	enable receive and transmit interrupt
    	_1680_Write(handle, 4, 0x3);
	temp = _1680_Read(handle,0) & 0xfe;
	_1680_Write(handle, 0, temp);	
	return 0;
}

int  CanGetPortStatus(int handle, PORT_STATUS *PortStatus)
{
    memcpy((void*)&(PortStatus->port), (void*)(&(portStruct[handle])), sizeof(PORT_STRUCT));
    (PortStatus->status).reg = _1680_Read(handle, 2);
    return 0;
}

BYTE  _1680_Read(int handle, int offset)
{
    REG_RW temp_reg;

    if (InstallRef[handle] == 0)
	return 0;
    temp_reg.can_sel = handle%2; //CAN#
    temp_reg.offset = offset; //+(0x200 * (handle%2));
    ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_REG_READ, &temp_reg);
    return (temp_reg.data);

}

void  _1680_Write(int handle, int offset, BYTE data)
{
    REG_RW temp_reg;

    if (InstallRef[handle] == 0)
	return;
    temp_reg.can_sel = handle%2;
    temp_reg.offset = offset; //+(0x200*(handle%2));
    temp_reg.data = data;
    ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_REG_WRITE, &temp_reg);
}


BYTE  CanGetLedStatus(int card_number, int index)
{
	return (LEDStatus[card_number]>>index) & 1;
}

void  CanSetLedStatus(int cardno, int index, int mode)
{
	BYTE tmp;
    REG_RW temp_write_reg;

    tmp = 0x01 << index;
    switch(mode)
    {
        case 0 : LEDStatus[cardno] &= ~tmp;                
                 break;
        case 1 : LEDStatus[cardno] |= tmp;
                 break;
        case 2 : break;
    }


    temp_write_reg.can_sel = index;
    temp_write_reg.offset = LED_ADDR_START;
    temp_write_reg.data = LEDStatus[cardno];
    ioctl(pci_Info_1680[cardno].DeviceID, IOCTL_1680_REG_WRITE, &temp_write_reg);
	return;
}

void  CanEnableReceive(int handle)
{
    _1680_Write(handle, 4, _1680_Read(handle, 4) | 0x01);
   	return;
}


void  CanDisableReceive(int handle)
{
    _1680_Write(handle, 4, _1680_Read(handle, 4) & 0xFE);
	return;
}

int  CanSendMsg(int handle, CAN_PACKET *msg)
{
	CAN_MSG	canmsg;
	I16   result;
	
	BYTE sts;
	int st=500;

    //_1680_Write(handle,0,0x08);  //set AFM mode as single mode
	do
	{
		if((st--) <0) break;
        sts=_1680_Read(handle,0x2);
		//printf("st %d",st);
        usleep(10000);
    }while((sts & 0x04) !=0x04);

	if(( sts & 0x04 ) == 0x04)
	{
		memcpy((CAN_PACKET*)(&(canmsg.msg)), msg, sizeof(CAN_PACKET));
		canmsg.handle = handle;	
    	ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_SET_BUFFER_DATA, &canmsg);
		//yuan modify 06/18/01
		result = canmsg.ret_val;
		if (result < 0) return -1;
		return 0;
	}
	return 0;                               
}

int  CanRcvMsg(int handle, CAN_PACKET *msg)
{
	CAN_MSG	canmsg;
    U16   result;
	//printf("CanRcvMsg handle %d\n",handle);
    if(InstallRef[handle] != 1)
        printf("HUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
	if( (msg == NULL) || (InstallRef[handle] != 1) )return -1;	
	canmsg.handle = handle;
	//printf("IOCTL GET BUFFER DATA\n");
	ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_GET_BUFFER_DATA, &canmsg);
	//yuan modify 06/18/01
	result = canmsg.ret_val;
    //printf("Result %d\n", result);
	if (result >= 0) 
	{   //yuan modify for data received successfully 
		memcpy(msg, (CAN_PACKET*)(&(canmsg.msg)), sizeof(CAN_PACKET));
		return 0;
    }
    return -1;
}

int  CanGetRcvCnt(int handle)
{
	REC_CN	rec_cn;

	if(InstallRef[handle] != 1)
		return -1;
	//printf("CanGetRcvCnt , dev node handle=%d\n",pci_Info_1680[0].DeviceID);
	rec_cn.handle = handle;	
    ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_GET_UNRD_COUNT, &rec_cn);
    return rec_cn.ret_val;
}

void  CanClearOverrun(int handle)
{
    //	Clear Data Overrun
	_1680_Write(handle, 1, 0x08);
	//	Enable Data Overrun Interrupt and Receive Interrupt
	_1680_Write(handle, 4, _1680_Read(handle, 4) | 0x09);
}

void  CanClearRxBuffer(int handle)
{
	if(InstallRef[handle] != 1)
		return ;
     	
    while(_1680_Read(handle, 29) != 0)
        _1680_Write(handle, 1, 0x04);
        
	return;
}

void  CanClearTxBuffer(int handle)
{
	if(InstallRef[handle] != 1)
		return ;

    //  write SJA CMR to abort transmission
    _1680_Write(handle, 1, 0x02);
	return;
}

BYTE  CanGetArbitrationLostBit(int handle)
{
    return _1680_Read(handle, 11);
}

BYTE  CanGetErrorCode(int handle)
{
    return _1680_Read(handle, 12);
}

BYTE  CanGetErrorWarningLimit(int handle)
{
    return _1680_Read(handle, 13);
}

void  CanSetErrorWarningLimit(int handle, BYTE limit)
{
    BYTE v = _1680_Read(handle, 0);
    //  enter RESET mode
    _1680_Write(handle, 0, 0);
    _1680_Write(handle, 13, limit);
    _1680_Write(handle, 0, v);
}

BYTE  CanGetRxErrorCount(int handle)
{
    return _1680_Read(handle, 14);
}

BYTE  CanGetTxErrorCount(int handle)
{
    return _1680_Read(handle, 15);
}

void  CanSetTxErrorCount(int handle, BYTE value)
{
    _1680_Write(handle, 15, value);
}

BYTE  CanGetIntStatusReg(int handle)
{
    return canIntStatus[handle];
}
BYTE CanGetFrameMode(int handle)
{
    BYTE res;
    RETINFO RetInfo;
    RetInfo.handle = handle;
    ioctl(pci_Info_1680[handle/2].DeviceID, IOCTL_1680_GET_PORT_MODE, &RetInfo);
    res = RetInfo.mode;
    return res;
}
int CanDetectBaudrate(int handle, int miliSecs)
{
    PORT_STRUCT save, set;
    long count, mode = -1;
    time_t start,end;
    BYTE previousMode;

    //  Save previous Port setting
    memcpy((void*)&save, (void*)&portStruct[handle], sizeof(PORT_STRUCT));
    memcpy((void*)&set, (void*)&save, sizeof(PORT_STRUCT));
    //  Bit-rate detection algorithm
    //  save previoius mode and enter Listen only mode
    previousMode = _1680_Read(handle, 0);
    _1680_Write(handle, 0, 0x02);

    for(count=3;count>=0;count--)
    {
          set.baudrate = count;
          CanConfigPort(handle, &set);
          canIntStatus[handle] = _1680_Read(handle,0x02);
          canIntStatus[handle] &= 0xFE;
          _1680_Write(handle,0x02,canIntStatus[handle]);
          CanEnableReceive(handle);
          time(&start);
 	  do
	  {
	       canIntStatus[handle] = _1680_Read(handle,0x02);
	       //  Bus error occurs
	       if(canIntStatus[handle] & 0x80)
               {
                     printf("0x80");
                     break;
               }
               if(canIntStatus[handle] & 0x01)
               {
                     mode = count;
                     break;
               }
               time(&end);
          } while(difftime(end,start)<((double)miliSecs/1000.0));

	  if (mode != -1)
             break;
      }
      CanConfigPort(handle, &save);
      CanEnableReceive(handle);
      _1680_Write(handle, 0, previousMode);

      if (mode == -1)    return -1;
      return mode;
}

bool  CanIsRunning(int handle)
{
	BYTE temp;
	temp=_1680_Read(handle,0);
	//printf("[DEBUG] Can%d is running? %d\n",handle,temp);
	return (temp & 0x01);
}

void  CanGetBusTiming(int handle, BYTE *bt0, BYTE *bt1)
{
	*bt0 = _1680_Read(handle,6);
	*bt1 = _1680_Read(handle,7);
}

bool  CanStart(int handle)
{
	int temp, st;

	temp = _1680_Read(handle,0);
	temp = temp & 0xfe;
	_1680_Write(handle,0,temp);
	st=500;
	do
	{
		if((st--) <0) break;
		usleep(10000);
		temp=_1680_Read(handle,0);
	}while((temp & 0x01)==0x01);
	if((temp & 0x01) == 0x01)
	    return false;	//starting failed while still in reset mode

	return true;
}

bool  CanReset(int handle)
{
	int temp, st;

	temp = _1680_Read(handle,0);
	temp = temp | 0x1;
	_1680_Write(handle,0,temp);

	st=500;
	do
	{
		if((st--) <0) break;
		usleep(10000);
		temp=_1680_Read(handle,0);
	}while((temp & 0x01)!=0x01);

	if((temp & 0x01) != 0x01)
	    return false;

	return true;
}

int CanGetPortCount()
{
	REC_CN rec_cn;
	rec_cn.handle = 0;	
	ioctl(pci_Info_1680[0].DeviceID,IOCTL_1680_GET_PORT_COUNT,&rec_cn);
	//printf("Get Port Count %d\n",rec_cn.ret_val);
	return rec_cn.ret_val;
}

