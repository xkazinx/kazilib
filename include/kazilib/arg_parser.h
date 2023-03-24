#pragma once

namespace kazin
{
class ArgParser
{
public:
	ArgParser() : ArgParser(__argc, (const char**)__argv)
	{
		
	}

	ArgParser(int argc, const char ** argv)
	{
		_list.assign(argv + 1, argv + argc);
	}

	bool Is(const char * argument) const
	{
		for(const auto & arg : _list)
		{
			if(arg == argument)
				return true;
		}

		return false;
	}

	string Get(const char * argument, string default_value);

	vector<string> _list;
};
}