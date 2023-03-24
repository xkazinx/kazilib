#include "pch.h"
#include "file.h"

namespace kazin::file
{
bool Exist(const Path & path)
{
	return GetFileAttributes(path.generic_string().c_str()) != INVALID_FILE_ATTRIBUTES;
}

u32 GetAttributes(const Path & path)
{
	return GetFileAttributes(path.generic_string().c_str());
}

void Fetch(const Path & src, vector<Path> & results, const bool include_subdirs)
{
	ForEach(src, [&](const Path & p)
	{
		results.push_back(p);
	}, include_subdirs);
}

vector<Path> Get(const Path & src, const bool include_subdirs)
{
	vector<Path> results;
	Fetch(src, results, include_subdirs);
	return results;
}

u32 GetSize(const Path & path)
{
	if (!Exist(path))
		return 0;

	const HANDLE handle = CreateFile(path.generic_string().c_str(), GENERIC_READ, NULL, nullptr, OPEN_EXISTING, NULL, nullptr);
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	
	const auto sz = ::GetFileSize(handle, nullptr);
	CloseHandle(handle);
	return sz;
}

void Delete(const Path & path, const bool delete_full_directory)
{
	if (!is_directory(path))
	{
		remove(path.string().c_str());
		return;
	}

	if (!delete_full_directory)
		return;

	vector<Path> dirs;
	ForEach(path, [&](const Path & fetched)
	{
		if (is_directory(fetched))
		{
			dirs.push_back(fetched);
		}
		else
		{
			Delete(path);
		}
	}, true, true);

	for (auto& dir : dirs)
	{
		RemoveDirectory(dir.generic_string().c_str());
	}

	RemoveDirectory(path.generic_string().c_str());
}

void CreateDir(const Path & p)
{
	if(Exist(p))
		return;
	
	::CreateDirectoryA(p.string().c_str(), nullptr);
}
}