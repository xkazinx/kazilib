#include "pch.h"
#include "arg_parser.h"
#include "console/log.h"

namespace kazin
{
	string ArgParser::Get(const char* argument, string default_value)
	{
		for (auto& arg : _list)
		{
			if (auto pos = arg.find_first_of("=");
				pos != string::npos)
			{
				if (arg.substr(0, pos) != argument)
				{
					PrintDebug("substr argument {} is different than {}", arg.substr(0, pos), argument);
					continue;
				}
				else
				{
					PrintDebug("substr argument {} is same than {}", arg.substr(0, pos), argument);
				}

				return arg.substr(pos + 1, arg.size());
			}
			else
			{
				PrintError("argument {} doesn't contain =", arg);
			}
		}

		return default_value;
	}
}
