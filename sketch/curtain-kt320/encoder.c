/*
 *  Copyright (c) 2015 - 2025 MaiKe Labs
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include "user_config.h"

/*
 * In Dooya KT320:
 *
 *  CW is opening state
 *  CCW is closing state
 *
 *  The circle time of the encoder is about 70ms
 *  The cnt is 8 when the motor run a circle (cw/ccw)
 *
 */

static os_timer_t check_encoder_timer;

static bool cw = 0;

/* circle time of the encoder, unit is ms */
static uint32_t encoder_circle_time = 0;
static uint32_t circtime[30];
static uint32_t ci = 0;

static int cnt = 8000;

/* count the max circle of cw */
static int cw_pos_max = 8000;

/* count the min circle of ccw */
static int ccw_pos_min = 8000;

/* cw_pos_max - ccw_pos_min is the max lengh of the track */

bool encoder_direction()
{
	/*
	 * True is the CW (opening)
	 * False is the CCW (closing)
	 */
	return cw;
}

/*
 * return the position:
 *
 *   0 is the closed state
 *   100 is the opened state
 *
 */
int encoder_pos()
{
	int t = 100 * (cnt - ccw_pos_min) / (cw_pos_max - ccw_pos_min);

	if (t < 0)
		t = 0;

	if (t > 100)
		t = 100;

	return t;
}

uint32_t encoder_circle(void)
{
	int i;
	uint32_t acc = 0;

	for (i = 0; i<30; i++) {
		if (circtime[i] != 0) {
			acc += circtime[i];	
		} else {
			encoder_circle_time = acc / i / 1000;
			return encoder_circle_time;
		}
	}

	encoder_circle_time = acc / i / 1000;

	if (encoder_circle_time <= 0 ||
			encoder_circle_time > 1500) {
		encoder_circle_time = 70;
	} 

	INFO("encoder_circle = %d\r\n", encoder_circle_time);
	return encoder_circle_time;
}

void encoder_reset()
{
	cw = 0;
	cnt = 8000;
	cw_pos_max = 8000;
	ccw_pos_min = 8000;
}

static void do_encoder_a()
{
	static uint32_t ts = 0;

	if (ts != 0) {
		circtime[ci] = system_get_time() - ts;
		ci = (ci + 1) % 30;
	}
	ts = system_get_time();

	cw = digitalRead(D9);

	if (cw == 1) {
		cnt++;

		if (cnt > cw_pos_max)
			cw_pos_max = cnt;

	} else {
		cnt--;

		if (cnt < ccw_pos_min)
			ccw_pos_min = cnt;
	}
}

void check_encoder()
{
	static int test = 0;
	
	if (test != 0) {
		if (cnt > test) {
			/* cw go */
			param_set_status(2);
		} else if (cnt < test) {
			/* ccw go */
			param_set_status(0);
		} else {
			/* do not move */
			param_set_status(1);
		}
		param_set_position(encoder_pos());
	}

	test = cnt;
}

irom void encoder_init()
{
	cw_pos_max = param_get_pos_max();
	ccw_pos_min = param_get_pos_min();

	cnt = param_get_position() * (cw_pos_max - ccw_pos_min) / 100 + ccw_pos_min;

	INFO("encoder_init, pos_max = %d, pos_min = %d, cnt = %d\r\n",
			cw_pos_max, ccw_pos_min, cnt);

	pinMode(D9, INPUT);

	cw = (bool)digitalRead(D9);

	attachInterrupt(D8, do_encoder_a, RISING);

	os_timer_disarm(&check_encoder_timer);
	os_timer_setfn(&check_encoder_timer, (os_timer_func_t *)check_encoder, NULL);
	os_timer_arm(&check_encoder_timer, 70/2 + 5, 1);
}
