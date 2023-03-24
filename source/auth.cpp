#include "pch.h"
#include "auth.h"

#ifdef KAZIN_ENET
namespace kazin
{
Auth::Auth() :
	_user(kInputSize),
	_pass(kInputSize)
{
}

Auth::Auth(InPacket * in)
{
	Read(in);
}

void Auth::Write(packet::Out & out) const
{
	out.Write(_user, _pass);
}

void Auth::Read(packet::In * in)
{
	in->Read(_user, _pass);
}

packet::Size Auth::GetPacketSize() const
{
	return _user.GetPacketSize() + _pass.GetPacketSize();
}

bool Auth::IsValid() const
{
	constexpr std::string::size_type kLength = 32;
	return _user.size() < kLength && _pass.size() < kLength;
}
}
#endif