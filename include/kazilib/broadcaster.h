#pragma once

namespace kazin::broadcaster
{
	using NetOp = common::NetOp;

	class InOperationBase
	{
		InPacket * _pkt = nullptr;

	public:
		InOperationBase() = default;
		explicit InOperationBase(InPacket & pkt) : _pkt(&pkt) {}
		auto GetPacket() const { return _pkt; }
	};

	enum class DummyOp { };

	template<class Op>
	class Base
	{
	public:
		using Peer = ENetPeer;

		explicit Base(const NetOp net_op) : _net_op(net_op) {}
		virtual ~Base() = default;

		auto GetOperation() const { return _op; }

		template<typename ... Args>
		packet::OutPktPtr StaticPacket(Op op, Args && ... args) const
		{
			return OutPkt::CreateStatic(_net_op, op, args...); // #why not forward here?
		}
		/**/
		template<typename ... Args>
		packet::OutPktPtr DynamicPacket(Op op, packet::Size sz, Args && ... args) const
		{
			auto pkt = OutPkt::CreateDynamic(_net_op, sz + packet::Base::Sizer::Get(op, std::forward<Args>(args)...));
			pkt->Write(op, std::forward<Args>(args)...);
			return pkt;
		}

	protected:
		Op _op;
		NetOp _net_op;
	};
}

#define OP(op) case Op::op: NetOp_##op(); break;