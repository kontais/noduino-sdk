#include "user_config.h"

static system_status_t local_system_status = {
	.init_flag = 0,
	.start_count = 0,
	.start_continue = 0,
	.mcu_status = {
		.s = 1,
		.r = 255,
		.g = 255,
		.b = 255,
		.w = 255,
	},
};

static os_timer_t app_smart_timer;
static app_state_t app_state = app_state_normal;
static uint8_t smart_effect = 0;

float ICACHE_FLASH_ATTR
fast_log2(float val)
{
	int * const  exp_ptr = (int *)(&val);
	int x = *exp_ptr;
	const int log_2 = ((x >> 23) & 255) - 128;
	x &= ~(255 << 23);
	x += 127 << 23;
	*exp_ptr = x;
	return (val + log_2);
}

void ICACHE_FLASH_ATTR
mjyun_receive(const char * event_name, const char * event_data)
{
	//INFO("RECEIVED: key=%s,value=%s\r\n", event_name, event_data);
	// Publish back
	//mjyun_publish(event_name, event_data);
	if (0 == os_strcmp(event_name, "set")) {
		//INFO("SET DATA\r\n");
		cJSON * pD = cJSON_Parse(event_data);
		if ((NULL != pD) && (cJSON_Object == pD->type)) {
			cJSON * pR = cJSON_GetObjectItem(pD, "r");
			cJSON * pG = cJSON_GetObjectItem(pD, "g");
			cJSON * pB = cJSON_GetObjectItem(pD, "b");
			cJSON * pW = cJSON_GetObjectItem(pD, "w");
			cJSON * pS = cJSON_GetObjectItem(pD, "s");
			if ((NULL != pR) && (cJSON_Number == pR->type)) {
				INFO("SET R = %d\r\n", pR->valueint);
				local_system_status.mcu_status.r = pR->valueint;
			}
			if ((NULL != pG) && (cJSON_Number == pG->type)) {
				INFO("SET G = %d\r\n", pG->valueint);
				local_system_status.mcu_status.g = pG->valueint;
			}
			if ((NULL != pB) && (cJSON_Number == pB->type)) {
				INFO("SET B = %d\r\n", pB->valueint);
				local_system_status.mcu_status.b = pB->valueint;
			}
			if ((NULL != pW) && (cJSON_Number == pW->type)) {
				INFO("SET W = %d\r\n", pW->valueint);
				local_system_status.mcu_status.w = pW->valueint;
			}
			if ((NULL != pS) && (cJSON_Number == pS->type)) {
				INFO("SET S = %d\r\n", pS->valueint);
				local_system_status.mcu_status.s = pS->valueint;
			}
			app_apply_settings();
		} else {
			INFO("%s: Error when parse JSON\r\n", __func__);
		}
		cJSON_Delete(pD);
	}

	if (0 == os_strcmp(event_name, "get")) {
		INFO("GET DATA\r\n");
	}

	if(os_strncmp(event_data, "ota", 3) == 0) {
		INFO("OTA: upgrade the firmware!\r\n");
		mjyun_mini_ota_start("ota/dev/bulb/files");
	}
}

void ICACHE_FLASH_ATTR
app_apply_settings(void)
{
	if (local_system_status.mcu_status.s) {
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    (uint16_t)(4095) * local_system_status.mcu_status.r / 255,
		    (uint16_t)(4095) * local_system_status.mcu_status.g / 255,
		    (uint16_t)(4095) * local_system_status.mcu_status.b / 255,
		    (uint16_t)(4095) * local_system_status.mcu_status.w / 255
		);
	} else {
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    0,
		    0,
		    0,
		    0
		);
	}
}

void ICACHE_FLASH_ATTR
app_load(void)
{
	system_param_load(
	    (APP_START_SEC),
	    0,
	    (void *)(&local_system_status),
	    sizeof(local_system_status));
	if (local_system_status.init_flag) {
		local_system_status.mcu_status.s = 1;
		// local_mcu_status = local_system_status.mcu_status;
	} else {
		local_system_status.init_flag = 1;
	}
	local_system_status.start_count += 1;
	local_system_status.start_continue += 1;
	INFO("Mjyun APP: start count:%d, start continue:%d\r\n",
			local_system_status.start_count, local_system_status.start_continue);
	app_save();
}

void ICACHE_FLASH_ATTR
app_save(void)
{
	// local_system_status.mcu_status = local_mcu_status;
	system_param_save_with_protect(
	    (APP_START_SEC),
	    (void *)(&local_system_status),
	    sizeof(local_system_status));
}

void ICACHE_FLASH_ATTR
app_set_smart_effect(uint8_t effect)
{
	smart_effect = effect;
}

void ICACHE_FLASH_ATTR
app_smart_timer_tick()
{
	static uint16_t value = 0;

	static bool flag = true;

	// uint16_t rlog = 4095 - (uint16_t)(340 * fast_log2(4095 - value + 1));
	switch (smart_effect) {
	case 0:
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    (uint16_t)value,
		    0,
		    0,
		    0
		);
		break;
	case 1:
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    0,
		    (uint16_t)value,
		    0,
		    0
		);
		break;
	case 2:
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    0,
		    0,
		    (uint16_t)value,
		    0
		);
		break;
	case 3:
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    0,
		    0,
		    0,
		    (uint16_t)value
		);
		break;
	default:
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    (uint16_t)value,
		    (uint16_t)value,
		    (uint16_t)value,
		    (uint16_t)value
		);
		break;
	}

	if (flag)
		value += 64;
	else
		value -= 64;
	if (value / 64 == 4095 / 64 - 1) {
		flag = false;
	}
	if (value / 64 == 0) {
		flag = true;
	}
	os_timer_disarm(&app_smart_timer);
	os_timer_setfn(&app_smart_timer, (os_timer_func_t *)app_smart_timer_tick, NULL);
	os_timer_arm(&app_smart_timer, 20, 1);
}

void ICACHE_FLASH_ATTR
app_start_check(uint32_t system_start_seconds)
{
	if ((local_system_status.start_continue != 0) && (system_start_seconds > 5)) {
		local_system_status.start_continue = 0;
		app_save();
	}

#if defined(APP_AGEING)
	if (local_system_status.start_count >= 65535) {
		INFO("Mjyun APP: clean ageing\r\n");
		local_system_status.start_count = 65534;
		app_save();
	} else if (local_system_status.start_count <= 1) {
		INFO("Mjyun APP: begin ageing\r\n");
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    4095,
		    4095,
		    4095,
		    4095
		);
	}
#endif

	if (local_system_status.start_continue >= 10) {
		if (app_state_restore != app_state) {
			INFO("Mjyun APP: system restore\r\n");
			app_state = app_state_restore;
			// Init flag and counter
			local_system_status.init_flag = 0;
			local_system_status.start_continue = 0;
			// Save param
			app_save();
			// Restore system
			system_restore();
			// Restart system
			system_restart();
		}
	} else if (local_system_status.start_continue >= 9) {
		os_timer_disarm(&app_smart_timer);
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    0,
		    0,
		    4095,
		    0
		);

	} else if (local_system_status.start_continue >= 8) {
		os_timer_disarm(&app_smart_timer);
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    0,
		    4095,
		    0,
		    0
		);
	} else if (local_system_status.start_continue >= 7) {
		os_timer_disarm(&app_smart_timer);
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    4095,
		    0,
		    0,
		    0
		);

	} else if (local_system_status.start_continue >= 6) {
		os_timer_disarm(&app_smart_timer);
		mjpwm_send_duty(
		    PIN_DI,
		    PIN_DCKI,
		    4095,
		    4095,
		    4095,
		    4095
		);
	} else if (local_system_status.start_continue >= 5) {
		if (MJYUN_CONNECTED == mjyun_state()) {
			if (app_state_upgrade != app_state) {
				INFO("Mjyun APP: OTA update\r\n");
				app_state = app_state_upgrade;
				mjyun_check_update();
			}
		}
	} else if (local_system_status.start_continue >= 3) {
		mjyun_forceentersmartlinkmode();
	}
	if ((WIFI_SMARTLINK_START == network_current_state()) ||
	    (WIFI_SMARTLINK_LINKING == network_current_state()) ||
	    (WIFI_SMARTLINK_FINDING == network_current_state()) ||
	    (WIFI_SMARTLINK_GETTING == network_current_state())) {
		if (app_state_smart != app_state) {
			INFO("Mjyun APP: begin smart config effect\r\n");
			app_state = app_state_smart;
			os_timer_disarm(&app_smart_timer);
			os_timer_setfn(&app_smart_timer, (os_timer_func_t *)app_smart_timer_tick, NULL);
			os_timer_arm(&app_smart_timer, 20, 1);
		}
	} else {
		if (app_state_smart == app_state) {
			app_state = app_state_normal;
			app_apply_settings();
			os_timer_disarm(&app_smart_timer);
		}
	}
}
