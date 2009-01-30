/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __UPNPHTTP_H__
#define __UPNPHTTP_H__

#include <netinet/in.h>
#include <sys/queue.h>

#include "config.h"

/* server: HTTP header returned in all HTTP responses : */
#define MINIUPNPD_SERVER_STRING	OS_VERSION " UPnP/1.0 miniupnpd/1.0"

/*
 states :
  0 - waiting for data to read
  1 - waiting for HTTP Post Content.
  ...
  >= 100 - to be deleted
*/
enum httpCommands {
	EUnknown = 0,
	EGet,
	EPost,
	EHead,
	ESubscribe,
	EUnSubscribe
};

struct upnphttp {
	int socket;
	struct in_addr clientaddr;	/* client address */
	int state;
	char HttpVer[16];
	/* request */
	char * req_buf;
	int req_buflen;
	int req_contentlen;
	int req_contentoff;     /* header length */
	enum httpCommands req_command;
	const char * req_soapAction;
	int req_soapActionLen;
#ifdef ENABLE_EVENTS
	const char * req_Callback;	/* For SUBSCRIBE */
	int req_CallbackLen;
	int req_Timeout;
	const char * req_SID;		/* For UNSUBSCRIBE */
	int req_SIDLen;
#endif
	long long int req_RangeStart;
	long long int req_RangeEnd;
	long int req_chunklen;
	int reqflags;
	int respflags;
	/* response */
	char * res_buf;
	int res_buflen;
	int res_buf_alloclen;
	/*int res_contentlen;*/
	/*int res_contentoff;*/		/* header length */
	LIST_ENTRY(upnphttp) entries;
};

#define FLAG_TIMEOUT	0x01
#define FLAG_SID		0x02
#define FLAG_RANGE		0x04
#define FLAG_HOST		0x08

#define FLAG_HTML		0x80
#define FLAG_INVALID_REQ	0x10

#define FLAG_CHUNKED            0x0100
#define FLAG_TIMESEEK           0x0200
#define FLAG_REALTIMEINFO       0x0400
#define FLAG_XFERSTREAMING      0x1000
#define FLAG_XFERINTERACTIVE    0x2000
#define FLAG_XFERBACKGROUND     0x4000

/* New_upnphttp() */
struct upnphttp *
New_upnphttp(int);

/* CloseSocket_upnphttp() */
void
CloseSocket_upnphttp(struct upnphttp *);

/* Delete_upnphttp() */
void
Delete_upnphttp(struct upnphttp *);

/* Process_upnphttp() */
void
Process_upnphttp(struct upnphttp *);

/* BuildHeader_upnphttp()
 * build the header for the HTTP Response
 * also allocate the buffer for body data */
void
BuildHeader_upnphttp(struct upnphttp * h, int respcode,
                     const char * respmsg,
                     int bodylen);

/* BuildResp_upnphttp() 
 * fill the res_buf buffer with the complete
 * HTTP 200 OK response from the body passed as argument */
void
BuildResp_upnphttp(struct upnphttp *, const char *, int);

/* BuildResp2_upnphttp()
 * same but with given response code/message */
void
BuildResp2_upnphttp(struct upnphttp * h, int respcode,
                    const char * respmsg,
                    const char * body, int bodylen);

/* SendResp_upnphttp() */
void
SendResp_upnphttp(struct upnphttp *);

void
SendResp_albumArt(struct upnphttp *, char * url);
void
SendResp_resizedimg(struct upnphttp *, char * url);
void
SendResp_thumbnail(struct upnphttp *, char * url);
/* SendResp_dlnafile()
 * send the actual file data for a UPnP-A/V or DLNA request. */
void
SendResp_dlnafile(struct upnphttp *, char * url);
#endif

