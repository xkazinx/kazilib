#pragma once

#ifdef KAZIN_ENET

static_assert("This is being compiled!");
namespace kazin
{
//class Socket;

namespace packet
{
	using NetOp = common::NetOp;

	class Out;
	class In;

	using OutPktPtr = unique_ptr<Out>;
	using Size = enet_uint32;

	class Serializable
	{
	public:
		Serializable()          = default;
		virtual ~Serializable() = default;

		virtual void Write(Out & out) const = 0;

		virtual void Read([[maybe_unused]] In * in)
		{
		} // To allow using Read in constructor

		virtual Size GetPacketSize() const = 0;
	};

	class SerializableString final : public Serializable, public string
	{
	public:
		using Size = enet_uint16;
		static string kDummy;

		SerializableString() :
			_data(nullptr), _sz(0), _ref(static_cast<string&>(*this))
		{
		}

		~SerializableString() override;

		SerializableString(Size sz) :
			_data(nullptr), _sz(sz), _ref(static_cast<string&>(*this))
		{
		}

		// String with any length
		explicit SerializableString(string & str) :
			_data(nullptr), _sz(0), _ref(str)
		{
		}

		// String with fixed length
		explicit SerializableString(string & str, Size sz) :
			_data(nullptr), _sz(sz), _ref(str)
		{
			if(_sz == 0)
			{
				throw std::exception("Trying to construct a fixed SerializableString with 0 length");
			}
		}

		// char* with any length
		explicit SerializableString(const char * data) :
			_data(data), _sz(0), _ref(kDummy)
		{
		}

		// char* with fixed length
		explicit SerializableString(const char * data, Size sz) :
			_data(data), _sz(sz), _ref(kDummy)
		{
			if(_sz == 0)
			{
				throw std::exception("Trying to construct a fixed char* SerializableString with 0 length");
			}
		}

		void Write(Out & out) const override;

		void Read(In * in) override;

		[[nodiscard]] packet::Size GetPacketSize() const override;

		auto IsString() const
		{
			return !_data && _sz == 0;
		}

		auto IsFixedString() const
		{
			return !_data && _sz != 0;
		}

		auto IsChar() const
		{
			return _data && _sz == 0;
		}

		auto IsFixedChar() const
		{
			return _data && _sz != 0;
		}

		/*explicit SerializableString(SerializableString& str)
		{
			if (IsString())
			{
				_ref = str._ref;
				_sz = 0;
				_data = nullptr;
			}
			else if(IsFixedString())
			{
				_ref = str._ref;
				_sz = str._sz;
				_data = nullptr;
			}
			else if(IsChar())
			{
				
			}
		}*/

		const char * _data;
		Size         _sz;
		string &     _ref = kDummy;
	};

	class Base
	{
		char * _ptr;
		Size   _size;

		virtual void Move(const Size sz) = 0;

	public:
		virtual ~Base() = default;
		Base(ENetPacket * packet, const Size sz);

		template<typename T> requires !is_pointer_v<T>
		auto GetPointer() const
		{
			return reinterpret_cast<T*>(_ptr);
		}

		class Sizer
		{
		public:
			template<typename T, typename ... Args, typename = enable_if_t<!is_pointer_v<T>>>
			static Size Get(const T & val, const Args & ...args)
			{
				return _Get(val) + Get(forward<const Args>(args)...);
			}

		private:
			static Size Get()
			{
				return 0;
			}

			template<typename T, typename = enable_if_t<!is_pointer_v<T>>>
			static Size _Get(const T & val)
			{
				if constexpr(is_base_of_v<Serializable, T>)
				{
					return val.GetPacketSize();
				}
				else
				{
					return sizeof(val);
				}
			}
		};

	protected:
		bool         ProcessOverflow(const bool extra_condition);
		virtual bool HasOverflowed(const Size sz) = 0;

		auto GetSize() const
		{
			return _size;
		}

		auto & GetLength() const
		{
			return _enet_packet->dataLength;
		}

		void _Move(const Size pos, const bool is_out);

		ENetPacket * _enet_packet;
		bool         _overflow;
	};

	class Out final : public Base
	{
	public:
		// Create by size and then write in another argument
		Out(const Size sz);
		~Out() override;

		void Move(const Size sz) override;
		bool HasOverflowed(const Size sz) override;

		// #todo packet::CreateStatic // packet::CreateDynamic
		template<typename T, typename = enable_if_t<!is_pointer_v<T>>, typename ... Args>
		static OutPktPtr CreateStatic(const NetOp net_op, const T & val, const Args & ...args)
		{
			auto pkt = CreateDynamic(net_op, Sizer::Get(val, forward<const Args>(args)...));
			pkt->Write(val, forward<const Args>(args)...);
			return pkt;
		}

		static OutPktPtr CreateDynamic(const NetOp net_op, const Size sz)
		{
			auto pkt = make_unique<Out>(Size(sizeof(net_op) + sz));
			pkt->Write(net_op);
			return pkt;
		}

		void Send(ENetPeer * peer, const enet_uint8 channel = 0) const;

		void Write()
		{

		}

		template<typename T, typename ... Args>
		void Write(const T & t, const Args & ...args)
		{
			_Write(t);
			Write(forward<const Args>(args)...);
		}
		
		template<typename T, std::size_t N>
		void Write(const array<T, N> & arr)
		{
			for(const auto & val : arr)
				Write(val);
		}

		template<typename T, std::size_t N>
		void Write(const T (&arr)[N])
		{
			for(std::size_t i = 0; i < N; i++)
				Write(arr[i]);
		}

	private:
		void WriteData(const char * data, Size size)
		{
			copy_n(data, size, GetPointer<char>());
			Move(size);
		}

		template<typename T, typename = enable_if_t<!is_pointer_v<T>>>
		void _Write(const T & v)
		{
#ifdef CLIENT
			static_assert(!is_same_v<string, T>, "Use fixed strings to send from client to server");
#endif	

			if constexpr(is_base_of_v<Serializable, T>)
			{
				v.Write(*this);
			}
			else if constexpr(is_same_v<string, T> || is_same_v<const string, T>)
			{
				const auto sz = v.size();
				Write((u16)sz);
				copy_n(v.c_str(), sz, GetPointer<char>());
				Move(sz);	
			}
			else
			{
				const auto sz = Sizer::Get(v);
				if(HasOverflowed(sz))
					return;

				auto ptr = GetPointer<T>();
				*ptr     = v;
				Move(sz);
			}
		}
	};

	class In final : public Base
	{
	public:
		In(ENetPacket * p);
		~In() override;

		bool HasOverflowed(const Size sz) override;
		void Move(const Size sz) override;

		template<typename T, typename ... Args>
		void Read(T & t, Args & ...args)
		{
			_Read(t);
			Read(args...);
		}

		template<typename T, size_t N>
		void Read(std::array<T, N> & arr)
		{
			for(auto & val : arr)
				Read(val);
		}

		template<typename T, size_t N>
		void Read(T (&arr)[N])
		{
			for(std::size_t i = 0; i < N; i++)
				Read(arr[i]);
		}

		template<typename T>
		T ReadAs()
		{
			T value;
			Read(value);
			return value;
		}

	private:
		void Read()
		{
		}

		void ReadData(char* dst, u32 sz)
		{
			copy_n(dst, sz, GetPointer<char>());
			Move(sz);
		}

		void ReadString(string & data, u32 sz)
		{
			data.assign(GetPointer<char>(), sz);
			const auto zero_len = strlen(data.c_str());
			if (data.length() != zero_len)
				data.resize(zero_len);
			Move(sz);
		}

		template<typename T, typename = enable_if_t<!is_pointer_v<T>>>
		void _Read(T & v)
		{
#ifndef CLIENT
			static_assert(!is_same_v<string, T>, "don't read strings from server!");
#endif

			if constexpr(is_base_of_v<Serializable, T>)
			{
				v.Read(this);
			}
			else if constexpr(is_same_v<string, T> || is_same_v<const string, T>)
			{
				auto sz = ReadAs<u16>();
				v.assign(GetPointer<char>(), sz);
				Move(sz);
			}
			else
			{
				const auto sz = Sizer::Get(v);
				if(HasOverflowed(sz))
					return;

				v = *GetPointer<T>();
				Move(sz);

/*#ifdef DEBUG_PACKET
				// #todo std
				if constexpr(is_same_v<NetOp, T>)
				{
					Print("Reading NetOp{}", Format(v));
				}
#endif*/
			}
		}
	};
}

using InPacket = packet::In;
using OutPkt = packet::Out;
using OutPktRef = packet::OutPktPtr&;
using NetString = packet::SerializableString;

#define SERIALIZE_IMPL(params)					\
	void Write(OutPkt & out) const override	\
	{											\
		out.Write(params);						\
	}											\
	void Read(InPacket * in) override			\
	{											\
		in->Read(params);						\
	}											\
	packet::Size GetPacketSize() const override			\
	{											\
		return packet::Base::Sizer::Get(params);				\
	}
}
#endif