#ifndef ENUMS_HPP
#define ENUMS_HPP

enum class ConnectionStatus
{
	NO_REG_NO_SEARCH=0,
    REG,
    NO_REG_SEARCH,
    REG_DECLINED,
    UNKNOWN,
    ROAMING
};

enum class ConnectionType
{
	GSM=0,
	GSM_COMPACT,
	UTRAN,
	GSM_W_EGPRS,
	UTRAN_W_HSDPA,
	UTRAN_W_HSUPA,
	UTRAN_W_HSDPA_HSUPA,
	E_UTRAN,
	EC_GMS_IOT,
	E_UTRAN_NB_S1,
        UNKNOWN
};

enum class ATResponse
{
	COPS,
	CREG,
	CSQ,
	CNUM,
	CLIP,
	RING,
	CLCC,
    VOICE_CALL,
    MISSED_CALL,
    CMTI,
    CMGRD,
	UNKNOWN
};

enum class CallStatus
{
	ACTIVE=0,
	HELD,
	DIALING,
	ALERTING,
	INCOMING,
	WAITING,
	DISCONNECT,
	UNKNOWN
};

enum class CallMode
{
	VOICE=0,
	DATA,
	FAX,
	UNKNOWN=9
};

enum class CallType
{
    INCOMING=0,
    OUTGOING=1,
    UNKNOWN
};

#endif
