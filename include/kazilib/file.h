#pragma once
#include <filesystem>

namespace kazin::file
{
using Path = std::filesystem::path;
using std::vector;

void Fetch(
	const Path &   src,
	vector<Path> & results,
	const bool     include_subdirs = true
);

vector<Path> Get(
	const Path & src,
	const bool   include_subdirs = true
);

template<typename Func>
void ForEach(
	const Path & src,
	Func         func,
	const bool   include_subdirs = true,
	const bool   fetch_dirs      = false
);

template<typename Func>
void ForEach(
	const Path & src,
	Func         func,
	const bool   include_subdirs /*= true*/,
	const bool   fetch_dirs /*= false*/
)
{
	HANDLE          dir;
	WIN32_FIND_DATA file_data;

	if((dir = FindFirstFile(
			(src / Path{"*"}).generic_string().c_str(), &file_data)
	) == INVALID_HANDLE_VALUE)
	{
		// No files found
		return;
	}

	do
	{
		Path fn{ file_data.cFileName };
		if(fn.filename().string() == ".")
			continue;

		Path p = src / fn;
		if(p.filename().string() == "..")
			continue;
		
		if(is_directory(p) && include_subdirs)
		{
			ForEach(p, func, include_subdirs);

			if(!fetch_dirs)
				continue;
		}

		func(p);
	}
	while(FindNextFile(dir, &file_data));

	FindClose(dir);
}

bool Exist(const Path & path);
u32  GetSize(const Path & path);
void CreateDir(const Path & path);
void Delete(const Path & path, const bool delete_full_directory = false);
}
