#ifndef  __PUSH_H__
#define  __PUSH_H__

#include "osapi.h"

#define PUSH_MAGICNUMBER 0xAA33CC55

// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
                            (((uint16)(A) & 0x00ff) << 8))
// 长整型大小端互换
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                            (((uint32)(A) & 0x00ff0000) >> 8) | \
                            (((uint32)(A) & 0x0000ff00) << 8) | \
                            (((uint32)(A) & 0x000000ff) << 24))

#ifdef PUSH_BIG_ENDIAN
#define push_htons(arg) (arg)
#else
#define push_htons(arg) BigLittleSwap16(arg)
#endif

#ifdef PUSH_BIG_ENDIAN
#define push_htonl(arg) (arg)
#else
#define push_htonl(arg) BigLittleSwap32(arg)
#endif

#ifdef PUSH_BIG_ENDIAN
#define push_ntohs(arg) (arg)
#else
#define push_ntohs(arg) BigLittleSwap16(arg)
#endif

#ifdef PUSH_BIG_ENDIAN
#define push_ntohl(arg) (arg)
#else
#define push_ntohl(arg) BigLittleSwap32(arg)
#endif

//检查结果
#define CHK(x) do {if(x<0) return x;} while(0)

#define ENCODE_BEGIN(buf, len) char *_buf = buf; char *_cursor = buf; uint32_t _left = len; int _ret = 0
#define ENCODE_GETLEN() ({_cursor - _buf;})
#define ENCODE_CHAR(v) ({if (_left < 1) _ret = -__LINE__;  else { *_cursor = v; _cursor += 1; _left -= 1;} _ret;})
#define ENCODE_INT16(v) ({if (_left < 2) _ret = -__LINE__; else { *(uint16_t *)_cursor = push_htons(v); _cursor += 2; _left -= 2;} _ret;})
#define ENCODE_INT32(v) ({if (_left < 4) _ret = -__LINE__; else { *(uint32_t *)_cursor = push_htonl(v); _cursor += 4; _left -= 4;} _ret;})
#define ENCODE_INT64(v) ({ \
        if (_left < 8) _ret = -__LINE__; \
        else { \
            *(uint32_t *)_cursor = push_htonl((uint32_t)v); \
            *(uint32_t *)(_cursor+4) = push_htonl((uint32_t)(v>>32)); \
            _cursor += 8; \
            _left -= 8; \
        } \
        _ret; \
        })

#define ENCODE_STR(str, len) ({ \
        if (_left < len) _ret = -__LINE__; \
        else { \
            os_memmove(_cursor, str, len); \
            _cursor += len; \
            _left -= len; \
        } \
        _ret; \
        })

#define ENCODE_LV16_STR(str, len, max_len) ({ \
        if (len > max_len) _ret = -__LINE__; \
        else if (_left < uint16_t(2 + len)) _ret = -__LINE__; \
        else { \
            *(uint16_t *)_cursor = push_htons(len); \
            _cursor += 2;  \
            os_memmove(_cursor, str, len); \
            _cursor += len; \
            _left -= 2 + len; \
        } \
        _ret; \
        })

#define ENCODE_LV32_STR(str, len, max_len) ({ \
        if (len > max_len) _ret = -__LINE__; \
        else if (_left < uint32_t(4 + len)) _ret = -__LINE__; \
        else { \
            *(uint32_t *)_cursor = push_htonl(len); \
            _cursor += 4;  \
            os_memmove(_cursor, str, len); \
            _cursor += len; \
            _left -= 4 + len; \
        } \
        _ret; \
        })

#define DECODE_BEGIN(buf, len) const char *_buf = buf; const char *_cursor = buf; uint32_t _left = len; int _ret = 0;
#define DECODE_GETLEN() ({_cursor - _buf;})
#define DECODE_CHAR(v) ({if (_left < 1) _ret = -__LINE__;  else {v = *_cursor; _cursor += 1; _left -= 1;} _ret; })
#define DECODE_INT16(v) ({if (_left < 2) _ret = -__LINE__; else {v = push_ntohs(*(uint16_t *)_cursor); _cursor += 2; _left -= 2;} _ret;})
#define DECODE_INT32(v) ({if (_left < 4) _ret = -__LINE__; else {v = push_ntohl(*(uint32_t *)_cursor); _cursor += 4; _left -= 4;} _ret;})
#define DECODE_INT64(v) ({ \
        if (_left < 8) _ret = -__LINE__; \
        else { \
            uint64_t _vl = push_ntohl(*(uint32_t *)_cursor); \
            uint64_t _vh = push_ntohl(*(uint32_t *)(_cursor+4)); \
            _cursor += 8; \
            _left -= 8; \
            v = (_vh << 32) + _vl; \
        } \
        _ret; \
        })

#define DECODE_STR(str, len) ({ \
        if (_left < len) _ret = -__LINE__; \
        else { \
            os_memmove(str, _cursor, len); \
            _cursor += len; \
            _left -= len; \
        } \
        _ret; \
        })

#define DECODE_LV16_STR(str, len, max_len) ({ \
        if (_left < 2) _ret = -__LINE__; \
        else { \
            len = push_ntohs(*(uint16_t *)_cursor); \
            _cursor += 2; \
            _left -= 2; \
            if (_left < len || len > max_len) _ret = -__LINE__; \
            else { \
                os_memmove(str, _cursor, len); \
                _cursor += len; \
                _left -= len; \
            } \
        } \
        _ret; \
})

#define DECODE_LV32_STR(str, len, max_len) ({ \
        if (_left < 4) _ret = -__LINE__; \
        else { \
            len = push_ntohl(*(uint32_t *)_cursor); \
            _cursor += 4; \
            _left -= 4; \
            if (_left < len || len > max_len) _ret = -__LINE__; \
            else { \
                os_memmove(str, _cursor, len); \
                _cursor += len; \
                _left -= len; \
            } \
        } \
        _ret; \
})

//SDK与intf之间的通信命令号
#define SDK_CMD_CLOSE    0x03E8	//1000 连接关闭(虚拟命令字)
#define SDK_CMD_INIT     0x03E9	//1001 初始化连接
#define SDK_CMD_HB       0x03EA	//1002 心跳
#define SDK_CMD_LOGIN    0x03EB	//1003 APPID上下线
#define SDK_CMD_OPTAG    0x03EC	//1004 设置或删除TAG
#define SDK_CMD_OPALIAS  0x03ED	//1005 设置或删除别名
#define SDK_CMD_PUSH     0x03EE	//1006 PUSH消息
#define SDK_CMD_SDKPUSH  0x03EF	//1007 SDK PUSH信息
#define SDK_CMD_QRYALIAS 0x03F0	//1008 SDK 查询SDK状态

#define PUSH_VERSION_JSON (1)
#define PUSH_VERSION_PB (2)

//二进制定长包头
typedef struct PushHead_t {
	uint32_t magicnum;	//Magic Code, 0xAA33CC55 
	uint32_t version;	//版本号, 默认为PUSH_VERSION_JSON
	uint32_t bodylen;	//包体长度(不含包头)
	uint32_t cmdid;		//命令号, 表示包体格式
	uint32_t seq;		//请求包序列号, 递增, 回传
	uint32_t checksum;	//包体校验码, 预留
	uint32_t flag;		//请求包的一些标志, 预留
} __attribute__ ((aligned(1), packed)) PushHead_t;

#endif
