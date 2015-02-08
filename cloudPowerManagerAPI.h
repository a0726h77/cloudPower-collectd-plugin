#ifndef _CLOUDPOWERMANAGER_H_
#define _CLOUDPOWERMANAGER_H_

/*****************************************************************************
* File: cloudPowerManagerTool.h
*	2013-11-18: First release
*	2014-12-25 released for delta
*****************************************************************************/



#define	HOSTNAME_LEN	32
typedef struct {
	char            devName[HOSTNAME_LEN];
	char            	devIp[15];
	unsigned int	netMask;
	unsigned int	gateway;
	unsigned char   devMac[6];
	unsigned int    devType;
	unsigned int    devSerialNum;
	unsigned char   regStata;
	unsigned char   svrConnected;
	unsigned char   dhcpMode;
} cloudPowerHost_t;

#define	NUM_HOST	32
typedef struct {
	unsigned char		nHost;
	cloudPowerHost_t	host[NUM_HOST];

}CloudPowerDiscovery_t;
#ifndef _CLOUDTIMER_H
enum {
	CLOUDTIMER_DevType_2400 = 0,
	CLOUDTIMER_DevType_2401,

	CLOUDTIMER_DevType_reserved_02,

	/*
	 * 3
	 */
	CLOUDTIMER_DevType_aw2100_gwRf,	/* cloud gw RF freescale */
	CLOUDTIMER_DevType_aw2101_gwWifi,	/* cloud gw wifi  mtk s605 linux */
	CLOUDTIMER_DevType_aw2110_devRf,	/* cloud outlet RF nuvoton m0 */
	CLOUDTIMER_DevType_aw2111_devWifi,	/* cloud outlet wifi mtk s605 linux */
	CLOUDTIMER_DevType_aw2410_devRf4,	/* cloud PowerSW RF nuvoton M0 */

	/*
	 * 8
	 */
	CLOUDTIMER_DevType_aw2100_gwBt,		/*m4, ethernet, ble*/
	CLOUDTIMER_DevType_aw2110_devBt,	/* m0 ble*/
	CLOUDTIMER_DevType_aw2111_gwBt,		/* s605 wifi ble*/
	CLOUDTIMER_DevType_aw2410_devBt,	/* m0 4Port ble*/
	CLOUDTIMER_DevType_aw2420_wifi,		/* m0 4port wifi */



	
	CLOUDTIMER_DevType_MaxAllowed,

};
enum {
	EventSrc_null = 0,
	EventSrc_local,
	EventSrc_LAN,
	EventSrc_trusted,
	EventSrc_eventBroker,
};

enum {
	EventCmpr_null = 0,
	EventCmpr_gt,				//greater than
	EventCmpr_ge,				//greater or equal
	EventCmpr_eq,				//equal
	EventCmpr_le,				//less or equal
	EventCmpr_lt,				//less than
};




enum {
	EventStatusType_null = 0,
	EventStatusType_virual,
	EventStatusType_timerDay,
	EventStatusType_timerWeekly,
	EventStatusType_tempCelsius,
	EventStatusType_tempKelvin,
	EventStatusType_humidity,
	EventStatusType_GasCo,
	EventStatusType_GasCh4,

};

enum {
	actType_null = 0,
	reservered_1,			
	reservered_2,		
	actType_setRelayOn,
	actType_setRelayOff,
	actType_IR_noWait,			//one time

	actType_send2SvrCritical,
	actType_send2SvrMajor,
	actType_send2SvrMinor,
	actType_send2SvrInfo,
	
	actType_send2BrokerCritical,	
	actType_send2BrokerMajor,
	actType_send2BrokerMinor,
	actType_send2BrokerInfo	
};
#endif
#ifndef _DEVNET_H_
enum{
	DevNetOpCmd_null=0,
	DevNetOpCmd_config, 	//this cmd valid only in wps mode, for safety
	DevNetOpCmd_maintenanceOn,
	DevNetOpCmd_maintenanceOff,
	DevNetOpCmd_BlinkingOn,
	DevNetOpCmd_BlinkingOff,
	

};
#endif
/*
 *====================================================================
 *=                      	export    
 *====================================================================
 */

//#include "cloudPowerManagerAPI_internal.h"

/*====================================================================
 * Function:     sendDiscovery
 *
 * Description:  discover device within this subnet
 *
 * In Parms:     none
 * Out Parms:   none

 *
 * Returns:      
	 	CloudPowerDiscovery_t pointer. 
		typedef struct {
			char            devName[HOSTNAME_LEN];
			char            	devIp[15];
			unsigned int	netMask;
			unsigned int	gateway;
			unsigned char   devMac[6];
			unsigned int    devType;
			unsigned int    devSerialNum;
			unsigned char   regStata;
			unsigned char   svrConnected;
		} cloudPowerHost_t;

		typedef struct {
			unsigned char		nHost;
			cloudPowerHost_t	host[NUM_HOST];

		}CloudPowerDiscovery_t;
 
 ====================================================================*/
extern  CloudPowerDiscovery_t * sendDiscovery (  );




/*====================================================================
 * Function:     getDevSerialNum
 *
 * Description:   get device serial number; this reqires in privilleged mode
 *
 * In Parms:     device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: serial number
 		

  ====================================================================*/
extern char           *
getDevSerialNum ( char *devIPString );




/*====================================================================
 * Function:     getVersion
 *
 * Description:   get device hardware/software version
 *				e.g. charPtr= getVersion();
 *
 * In Parms:     device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	4 entries separated by space
	1.	Hardware verion
	2.	Software verion
	3.	Version date
	4.	Version time

  ====================================================================*/
extern char           *
getVersion ( char *devIPString );



/*====================================================================
 * Function:     setBlinkingOn
 *
 * Description:   turn on device LED blinking 
 *				e.g. charPtr= setBlinkingOn();
 *
 * In Parms:     device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
setBlinkingOn ( char *devIPString );



/*====================================================================
 * Function:     setBlinkingOff
 *
 * Description:   turn off device LED blinking 
 *				e.g. charPtr= setBlinkingOff();
 *
 * In Parms:     device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
setBlinkingOff ( char *devIPString );



/*====================================================================
 * Function:     getReg
 *
 * Description:   get registration status
 *				e.g. charPtr= getReg();
 *
 * In Parms:     device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: 
 	"no": no register
	"yes": register

  ====================================================================*/
extern char           *
getReg ( char *devIPString );



/*====================================================================
 * Function:     getLocalMode
 *
 * Description:   get local mode
 *				e.g. getLocalMode("192.168.1.5", 3, 34);
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: 
         "0": off
         "1": on
         "-1":  fail to get

  ====================================================================*/
extern char           *
getLocalMode ( char *devIPString );



/*====================================================================
 * Function:     getElectric /getElectric2 /getElectricCfg / setElectricCfg
 *
 * Description:   
 	getElectric 
 	two numbers replied
 	 	<electriCurrent in 0.1A> <kwhrTotal in 0.1 kwhr unit>
	 	e.g. "45 3254"
	 	where 
	 		45: curent electrical current is 4.5 A
	 		325.4 is the kwhr total accumulated since system config reset

 	getElectric2
 	two numbers replied
 	 	<electriCurrent in 0.001A> <kwhrTotal in 0.001 kwhr unit>
	 	e.g. "4512 325432"
	 	where 
	 		4512: curent electrical current is 4.512 A
	 		325.432 is the kwhr total accumulated since system config reset


	getElectricCfg
 	two numbers replied
 		<electric current limit in 0.1A> <voltage>
		e.g. 120 220
		the current limit is 12A
		the voltage is 220 volt

	
	
	getElectricCfg 
 	two numbers replied
 		<electric current limit in 0.1A> <voltage>
		e.g. 120 220
		the current limit is 12A
		the voltage is 220 volt




 	
 * In Parms:     
 	devIPString: device IP String, e.g. "192.168.2.55"
	electriCurrentLimit: the electric current limit in 0.1A
	voltage: the voltage in the outlet


 	
  ====================================================================*/
extern char           *
getElectric( char *devIPString );

extern char           *
getElectric2( char *devIPString );

extern char           *
getElectricCfg( char *devIPString );

extern char           *
setElectricCfg( char *devIPString, unsigned int, unsigned int );






/*====================================================================
 * Function:     setPortState
 *
 * Description:   set port state
 *				e.g. setPortState("192.168.1.5", 3, 34, 0);
 *				e.g. setPortState("192.168.1.5", 3, 34, 1);
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
	device type number
	device serial number
	portStateString: (not char string, it is integer)
		0		off
		1		on
		other	ignored

		e.g for 4-port device
		it will look like "0110"
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
setPortState ( char *devIPString, unsigned int devType, unsigned int devSerial , char *portState );





/*====================================================================
 * Function:     getPortState
 *
 * Description:  get port state
 *				e.g. getPortState("192.168.1.5", 3, 34); 
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
	device type number
	device serial number
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
           "0": off
           "1": on
	if more than one port, it will be "1001", it mean port 1 and 4 are on, port 2 and 3 are off
  ====================================================================*/
extern char           *
getPortState ( char *devIPString, unsigned int devType, unsigned int devSerial);





/*====================================================================
* Function:     setWifiConfig / setWifiConfig2
 *
 * Description:   
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
 	ssid, e.g. "myAffair"
 	passwd, e.g. "mySecretLove"

	setWifiConfig
		the target will reboot to take effective after command processed
	setWifiConfig2
		the target will 	NOT reboot to take effective after command processed
 	
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
setWifiConfig ( char *devIPString, char *ssid, char *password );


extern char           *
setWifiConfig2 ( char *devIPString, char *ssid, char *password );




/*====================================================================
 * Function:     getWifiSsid
 *
 * Description: get wifi ssid
 *				e.g. getWifiSsid("192.168.1.5");   
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
 	
 * Out Parms:   none
 *
 * Return: char*:  the ssid string
  ====================================================================*/
extern char           *
getWifiSsid ( char *devIPString );





/*====================================================================
 * Function:     setNetConfig/setNetConfig2
 *
 * Description:   
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
 	isDhcp:
 		0			static IP settting
 		otherwise		dhcp
 	ipAddr: IP Address, e.g. "192.168.1.45"
 	netmask:  e.g. "255.255.254.0"
 	gateway: IP Address, e.g. "192.168.1.1"


	setNetConfig
		the target will reboot to take effective after command processed
	setNetConfig2
		the target will 	NOT reboot to take effective after command processed
 	
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
setNetConfig ( char *devIPString, char isDhcp, char *ipAddr, char *netmask, char *gateway );

char           *
setNetConfig2 ( char *devIPString, char isDhcp, char *ipAddr, char *netmask, char *gateway );



/*====================================================================
 * Function:     getNetworkMode
 *
 * Description:   get network configuration
 *				e.g. getNetConfig("192.168.1.5");   
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
 	
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
getNetConfig ( char *devIPString );






/*====================================================================
 * Function:     sendReboot
 *
 * Description:   reboot the device, 
 *				e.g. sendReboot("192.168.1.56", 3, 45)
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
sendReboot ( char *devIPString, unsigned int devType, unsigned int devSerial);





/*====================================================================
 * Function:     sendReset
 *
 * Description:   send config reset request to device 
 *				e.g. sendReset("192.168.1.56", 3, 45)
 *
 * In Parms:     
 	device IP String, e.g. "192.168.2.55"
	device type number
	device serial number
	
 * Out Parms:   none
 *
 * Return: char*: Success/Fail
	"0": Success
	"-1":  fail to get
  ====================================================================*/
extern char           *
sendReset ( char *devIPString, unsigned int devType, unsigned int devSerial);


/*====================================================================
 * Function:     setCloudOff / getCloudOff
 *
 * Description:   
 *
 * In Parms:     
 		device IP String, e.g. "192.168.2.55"
 		off	otherwise		set cloud off
 			0			set cloud on
 * Out Parms:   none
 *
 * Return: 
  ====================================================================*/
extern char           *
setCloudOff ( char *devIPString, char off );
extern char           *
getCloudOff ( char *devIPString );


#endif /*_CLOUDPOWERMANAGER_H_*/

