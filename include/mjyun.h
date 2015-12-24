#ifndef __MJYUN_H__
#define __MJYUN_H__

#if defined(GLOBAL_DEBUG_ON)
#define MJYUN_DEBUG_ON
#endif
#if defined(MJYUN_DEBUG_ON)
#define MJYUN_DEBUG(format, ...) os_printf(format, ##__VA_ARGS__)
#else
#define MJYUN_DEBUG(format, ...)
#endif

typedef enum {
	WIFI_IDLE,
	WIFI_STAR_SMARTLINK,
	WIFI_SMARTLINK_LINKING,
	WIFI_SMARTLINK_FINDING,
	WIFI_SMARTLINK_TIMEOUT,
	WIFI_SMARTLINK_GETTING,
	WIFI_SMARTLINK_OK,
	WIFI_AP_OK,
	WIFI_AP_ERROR,
	WIFI_AP_STATION_OK,
	WIFI_AP_STATION_ERROR,
	WIFI_STATION_OK,
	WIFI_STATION_ERROR,
	MJYUN_CONNECTING,
	MJYUN_CONNECTING_ERROR,
	MJYUN_CONNECTED,
	MJYUN_DISCONNECTED,
} MJYUN_State_t;

typedef enum {
	MARTLINK,
	WIFI,
	MJYUN_CLOUD,
} MJYUN_MsgType_t;

/*----------------------- */
/*mjyun APIs*/
/*----------------------- */

typedef void (*MjyunCallback)();
typedef void (*MjyunDataCallback)(const char *event_name, const char *event_data);
typedef void (*MjyunStateCallback)(/* MJYUN_MsgType_t type ,*/MJYUN_State_t state);

void ICACHE_FLASH_ATTR MJYUN_Init(const char* product_id);
void ICACHE_FLASH_ATTR MJYUN_OnConnected(MjyunCallback connectedCb);
void ICACHE_FLASH_ATTR MJYUN_OnDisconnected(MjyunCallback disconnectedCb);
void ICACHE_FLASH_ATTR MJYUN_Connect();
void ICACHE_FLASH_ATTR MJYUN_Disconnect();
void ICACHE_FLASH_ATTR MJYUN_OnData(MjyunDataCallback dataCb);
void ICACHE_FLASH_ATTR MJYUN_OnPublished(MjyunCallback sendCb);
BOOL ICACHE_FLASH_ATTR MJYUN_Publish(const char* event_name, const char* event_data);
BOOL ICACHE_FLASH_ATTR MJYUN_PublishStatus(const char* status_data);


void ICACHE_FLASH_ATTR MJYUN_StateChanged(MjyunStateCallback stateChangedCb);
MJYUN_State_t ICACHE_FLASH_ATTR MJYUN_State();

/*----------------------- */
/*others APIs*/
/*----------------------- */

/******************************************************************************
 * FunctionName : MJYUN_GetDevicePasswd
 * Description  : get mjyun passwd of this device
 * Parameters   : none 
 * Returns      : if this device didn't init, return NULL
*******************************************************************************/
uint8_t* MJYUN_GetDevicePasswd();

/******************************************************************************
 * FunctionName : MJYUN_GetDeviceId
 * Description  : get mjyun DID of this device
 * Parameters   : none 
 * Returns      : passwd, if did didn't init, return NULL
*******************************************************************************/
uint8_t* MJYUN_GetDeviceId();

//mjyun_get_firmware_id( uint8_t slot );
//mjyun_get_firmware_version( void );
//mjyun_get_hardware_version( void );

/******************************************************************************
 * FunctionName : MJYUN_SystemRecovery
 * Description  : recovery mjyun data
 * Parameters   : none 
 * Returns      : none
*******************************************************************************/
void MJYUN_SystemRecovery(); //系统恢复，抹掉所有mjcloud相关数据 --> 一般对应物理按键

void MJYUN_ForceEnterSmartlinkMode(); //强制进入配网模式 --> 一般对应物理按键

void MJYUN_SetssidPrefix(const char* sPrefix);


#endif /*  __MJYUN_H__ */

