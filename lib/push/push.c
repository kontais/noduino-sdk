#include "os_type.h"
#include "push.h"

void ICACHE_FLASH_ATTR init_push(PushHead_t * ph)
{
	ph->magicnum = PUSH_MAGICNUMBER;
	ph->version = PUSH_VERSION_JSON;
	ph->bodylen = 0;
	ph->cmdid = 0;
	ph->seq = 0;
	ph->checksum = 0;
	ph->flag = 0;
}

int ICACHE_FLASH_ATTR encode_push(PushHead_t * ph, char *buf, uint32_t len)
{
	if (len < sizeof(PushHead_t))
		return -1;

	ENCODE_BEGIN(buf, len);
	CHK(ENCODE_INT32(ph->magicnum));
	CHK(ENCODE_INT32(ph->version));
	CHK(ENCODE_INT32(ph->bodylen));
	CHK(ENCODE_INT32(ph->cmdid));
	CHK(ENCODE_INT32(ph->seq));
	CHK(ENCODE_INT32(ph->checksum));
	CHK(ENCODE_INT32(ph->flag));
	return ENCODE_GETLEN();
}

int ICACHE_FLASH_ATTR decode_push(PushHead_t * ph, char *buf, uint32_t len)
{
	if (len < sizeof(PushHead_t))
		return -1;

	DECODE_BEGIN(buf, len);
	CHK(DECODE_INT32(ph->magicnum));
	CHK(DECODE_INT32(ph->version));
	CHK(DECODE_INT32(ph->bodylen));
	CHK(DECODE_INT32(ph->cmdid));
	CHK(DECODE_INT32(ph->seq));
	CHK(DECODE_INT32(ph->checksum));
	CHK(DECODE_INT32(ph->flag));

	if (ph->magicnum != PUSH_MAGICNUMBER)
		return -1;
	return DECODE_GETLEN();
}

#if 0
void ICACHE_FLASH_ATTR push_test1()
{
	char package_buf[200];
	char *data = "ddd";
	int data_len = 3;

	PushHead_t head;
	init_push(&head);
	head.version = PUSH_VERSION_JSON;
	head.cmdid = 0;
	head.bodylen = data_len;
	head.seq = 0;
	if (encode_push(&head, package_buf, sizeof(PushHead_t)) <= 0)
		os_printf("\n encode_push error \n");

	int send_buf_length = sizeof(PushHead_t) + data_len;

	if (data != NULL)
		os_memmove(package_buf + sizeof(PushHead_t), data, data_len);

	//--------------------------------------------------------------------//
	PushHead_t rec_head;
	init_push(&rec_head);
	if (decode_push(&rec_head, (char *)package_buf, sizeof(PushHead_t)) <=
	    0)
		os_printf("\n decode_push error \n");
	char *rec_body = (char *)package_buf + sizeof(PushHead_t);

	int package_len = sizeof(PushHead_t) + rec_head.bodylen;
	package_buf[package_len] = '\0';

	os_printf
	    ("\nreceive message type :\tcmdid:%d,\tbody_len:%d, \tdata:%s\n",
	     rec_head.cmdid, rec_head.bodylen, rec_body);
}
#endif
