#pragma once

#ifdef KAZIN_ENET
namespace kazin
{
class Auth final : public packet::Serializable
{
public:
	// #todo make customizable per project?
	static constexpr auto kInputSize = 32;
	Auth();
	explicit Auth(packet::In * in);

	~Auth() override = default;

	void Write(packet::Out & out) const override;
	void Read(packet::In * in) override;
	[[nodiscard]] packet::Size GetPacketSize() const override;
	
	[[nodiscard]] bool IsValid() const;

	// #todo use constant player::kUsername
	NetString _user{ kInputSize }, _pass{ kInputSize };
};
}
#endif