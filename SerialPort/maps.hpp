#ifndef MAPS_HPP
#define MAPS_HPP

#include <unordered_map>

std::unordered_map<std::string, ATResponse> responseMap=
{
	{"+COPS",ATResponse::COPS},
	{"+CREG",ATResponse::CREG},
	{"+CSQ",ATResponse::CSQ},
	{"+CNUM",ATResponse::CNUM},
	{"RING",ATResponse::RING},
    {"+CLIP",ATResponse::CLIP},
    {"+CLCC",ATResponse::CLCC}
};

//Values from SIM7600 documentation
//https://simcom.ee/documents/SIM7600C/SIM7500_SIM7600%20Series_AT%20Command%20Manual_V1.01.pdf
std::unordered_map<uint8_t, const char*> callNumberType=
{
	{128,"Restricted number type includes unknown type and format"},
	{145,"International number type"},
	{161,"National number type"},
	{177,"Network specific number, ISDN format"},
	{129,"Otherwise"}
};

std::unordered_map<ConnectionStatus, const char*> connectionStatus=
{
    {ConnectionStatus::NO_REG_NO_SEARCH,"Not registered, not currently searching a new operator"},
    {ConnectionStatus::REG,"Registered, home nerwork"},
    {ConnectionStatus::NO_REG_SEARCH,"Not registered, currently searching a new operator"},
    {ConnectionStatus::REG_DECLINED,"Registration denied"},
    {ConnectionStatus::UNKNOWN,"Unknown"},
    {ConnectionStatus::ROAMING,"Roaming"}
};

#endif
