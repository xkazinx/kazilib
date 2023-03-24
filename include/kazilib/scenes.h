#pragma once

#ifdef KAZIN_GRAPHICS
namespace kazin
{
class Scene
{
public:
	Scene()
	{
	}

	virtual ~Scene()
	{
	}

	virtual void Tick() = 0;
	virtual void Render(sf::RenderTarget & tgt, const sf::RenderStates & states) const = 0;
	virtual bool HandleEvent(const sf::Event & ev) = 0;

#ifdef KAZIN_IMGUI
	virtual void ProcessUI()
	{
	}
#endif
};

class Scenes final : public Scene
{
public:
	void Tick() override
	{
		for(auto & scene : _list)
		{
			scene.Tick();
		}
	}

	void Render(sf::RenderTarget & tgt, const sf::RenderStates & states) const override
	{
		for(auto & scene : _list)
			scene.Render(tgt, states);
	}

	bool HandleEvent(const sf::Event & ev) override
	{
		for(auto & scene : _list)
			if(scene.HandleEvent(ev))
				return true;

		return false;
	}

#ifdef KAZIN_IMGUI
	void ProcessUI()
	{
		for(auto & scene : _list)
			scene.ProcessUI();
	}
#endif

	vector<Scene> _list;
};
}
#endif