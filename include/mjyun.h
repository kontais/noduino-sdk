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
	WIFI_SMARTLINK_START,
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
} mjyun_state_t;

typedef enum {
	MARTLINK,
	WIFI,
	MJYUN_CLOUD,
} mjyun_msgtype_t;

/*----------------------- */
/*mjyun APIs*/
/*----------------------- */

typedef void (*mjyun_callback)();
typedef void (*mjyun_data_callback)(const char *event_name, const char *event_data);
typedef void (*mjyun_state_callback)(/* mjyun_msgtype_t type ,*/mjyun_state_t state);

/******************************************************************************
 * FunctionName : mjyun_init
 * Description  : device parame init based on mjyun platform
 * Parameters   : const char* product_id -- mjyun product id
 *              : const char* offline_words -- device will word
 * Returns      : return 0 success, others fails
 *******************************************************************************/
void mjyun_init(const char* product_id, const char* offline_words);
void mjyun_onconnected(mjyun_callback connectedCb);
void mjyun_ondisconnected(mjyun_callback disconnectedCb);
void mjyun_connect();
void mjyun_disconnect();
void mjyun_ondata(mjyun_data_callback dataCb);
void mjyun_onpublished(mjyun_callback sendCb);
BOOL mjyun_publish(const char* event_name, const char* event_data);
BOOL mjyun_publishstatus(const char* status_data);


void mjyun_statechanged(mjyun_state_callback stateChangedCb);
mjyun_state_t mjyun_state();

/*----------------------- */
/*others APIs*/
/*----------------------- */

/******************************************************************************
 * FunctionName : MJYUN_GetDevicePasswd
 * Description  : get mjyun passwd of this device
 * Parameters   : none 
 * Returns      : if this device didn't init, return NULL
*******************************************************************************/
uint8_t* mjyun_getdevicepasswd();

/******************************************************************************
 * FunctionName : mjyun_getdeviceid
 * Description  : get mjyun DID of this device
 * Parameters   : none 
 * Returns      : passwd, if did didn't init, return NULL
*******************************************************************************/
uint8_t* mjyun_getdeviceid();

//mjyun_get_firmware_id( uint8_t slot );
//mjyun_get_firmware_version( void );
//mjyun_get_hardware_version( void );

/******************************************************************************
 * FunctionName : mjyun_systemrecovery
 * Description  : recovery mjyun data
 * Parameters   : none 
 * Returns      : none
*******************************************************************************/
void mjyun_systemrecovery(); //系统恢复，抹掉所有mjcloud相关数据 --> 一般对应物理按键

void mjyun_forceentersmartlinkmode(); //强制进入配网模式 --> 一般对应物理按键

void mjyun_setssidprefix(const char* sPrefix);


#endif /*  __MJYUN_H__ */

