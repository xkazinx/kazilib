#include "pch.h"
#include "socket.h"

#ifdef KAZIN_ENET
namespace kazin
{
u8 Socket::Count = 0;

Socket::Socket(Type type, Fnc on_connect, Fnc on_packet, Fnc on_disconnect, szt peers) :
	_type(type), _on_connect(on_connect), _on_packet(on_packet), _on_disconnect(on_disconnect)
{
	if(Count++ == 0)
	{
		enet_initialize();
	}

	// For server only
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 333;

	_host = enet_host_create(IsClient() ? nullptr : &address,
	                         IsClient() ? 1 : peers, 1, 0, 0);

	if(!_host)
	{
		PrintError("enet_host_create failure");
	}
}

Socket::~Socket()
{
	if(_host)
	{
		enet_host_destroy(_host);
	}

	if(--Count == 0)
	{
		enet_deinitialize();
	}
}

void Socket::Tick() const
{
	ENetEvent ev;
	while(enet_host_service(_host, &ev, 0) != 0)
	{
		switch(ev.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			Print("Client {} connected", kazin::Socket::FetchIP(ev.peer));
			ev.peer->data = nullptr;
			_on_connect(ev);
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			Print("Received packet of {} bytes", ev.packet->dataLength);
			_on_packet(ev);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			if(ev.peer->data)
			{
				Print("Client {} disconnected", kazin::Socket::FetchIP(ev.peer));
				ev.peer->data = nullptr;
			}
			_on_disconnect(ev);
			break;
		}
	}
}

bool Socket::Connect(const string & ip, u16 port)
{
	PrintDebug("Socket::Connect({}, {})", ip, port);

	ENetAddress address;

	enet_address_set_host(&address, ip.c_str());
	address.port = port;

	return (_peer = enet_host_connect(_host, &address, 1, 0)) != nullptr;
}

void Socket::Send(OutPktRef out, enet_uint8 channel /*= 0u*/) const
{
	out->Send(_peer, channel);
}

void Socket::Send(ENetPeer * peer, OutPktRef out, const enet_uint8 channel) const
{
	out->Send(peer, channel);
}

string Socket::FetchIP(ENetPeer * peer)
{
	char data[15]{};
	if(enet_address_get_host_ip(&peer->address, data, sizeof(data)) < 0)
	{
		PrintError("enet_address_get_host_ip");
	}

	return data;
}
}
#endif