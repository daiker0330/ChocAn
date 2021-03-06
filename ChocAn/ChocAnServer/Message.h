#pragma once

#define MSG_NULL				0x00

#define MSG_SIGNIN_REQUEST		0x10
#define MSG_SIGNIN_SUCCESS		0x11
#define MSG_SIGNIN_FAILED		0x12

#define MSG_ISVALID_REQUEST		0x20
#define MSG_ISVALID_VALID		0x21
#define MSG_ISVALID_INVALID		0x22
#define MSG_ISVALID_SUSPEND		0x23

#define MSG_SEVRNAME_REQUEST	0x30
#define MSG_SEVRNAME_RETURN		0x31

#define MSG_SEVRPRICE_REQUEST	0x40
#define MSG_SEVRPRICE_RETURN	0x41

#define MSG_SERVRECORD_REQUEST	0x50
#define MSG_SERVRECORD_SUCCESS	0x51
#define MSG_SERVRECORD_FAILED	0x52

#define MSG_PRODSUM_REQUEST		0x60
#define MSG_PRODSUM_RETURN		0x61

class CMessage
{
public:
	unsigned short type1;
	char msg[254];
};