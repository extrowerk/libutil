// Copyright (C) 2005 - 2021 Settlers Freaks (sf-team at siedler25.org)
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "getExecutablePath.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/predef/os.h>
#include <vector>

namespace bfs = boost::filesystem;

// Heavily based on https://stackoverflow.com/a/34109000/1930508 by "Ben Key"
// with inspirations from WhereAmI https://github.com/gpakosz/whereami

#if(BOOST_OS_CYGWIN || BOOST_OS_WINDOWS) // {

#    include <boost/nowide/convert.hpp>
#    include <windows.h>
#    include <array>

std::string getExecutablePath()
{
    std::vector<wchar_t> buf(MAX_PATH);
    while(true)
    {
        DWORD ret = GetModuleFileNameW(nullptr, buf.data(), static_cast<DWORD>(buf.size()));
        if(ret == 0)
            return "";
        else if(ret == static_cast<DWORD>(buf.size()))
            buf.resize(buf.size() * 2);
        else
            break;
    }
    std::array<wchar_t, MAX_PATH> fullPath;
    if(!_wfullpath(&fullPath.front(), &buf.front(), fullPath.size()))
        return "";
    return boost::nowide::narrow(&fullPath.front());
}

#elif(BOOST_OS_MACOS)

#    include <iostream>
#    include <mach-o/dyld.h>

std::string getExecutablePath()
{
    std::vector<char> buf(1024, 0);
    uint32_t size = buf.size();
    if(_NSGetExecutablePath(&buf.front(), &size) == -1)
    {
        buf.resize(size);
        if(!_NSGetExecutablePath(&buf.front(), &size))
            return "";
    }
    boost::system::error_code ec;
    using bfs::canonical;
    bfs::path p(canonical(&buf.front(), ec));
    return ec ? "" : p.make_preferred().string();
}

#elif(BOOST_OS_SOLARIS)

std::string getExecutablePath()
{
    std::string ret = getexecname();
    if(ret.empty())
        return "";
    bfs::path p(ret);
    if(!p.has_root_directory())
    {
        boost::system::error_code ec;
        using bfs::canonical;
        p = canonical(p, ec);
        ret = (ec) ? "" : p.make_preferred().string();
    }
    return ret;
}

#elif(BOOST_OS_BSD)

#    include <sys/sysctl.h>

std::string getExecutablePath()
{
    std::array<int, 4> mib = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
    std::vector<char> buf(1024, 0);
    size_t size = buf.size();
    if(sysctl(mib.data(), 4, buf.data(), &size, nullptr, 0) != 0)
        return "";
    if(size == 0 || size == buf.size())
        return "";
    std::string path(buf.begin(), buf.begin() + size);
    boost::system::error_code ec;
    using bfs::canonical;
    bfs::path p(canonical(path, ec));
    return ec ? "" : p.make_preferred().string();
}

#elif(BOOST_OS_LINUX)

#    include <unistd.h>

std::string getExecutablePath()
{
    std::vector<char> buf(1024, 0);
    int size;
    while(true)
    {
        size = readlink("/proc/self/exe", buf.data(), buf.size());
        if(size <= 0)
            return "";
        if(size < static_cast<int>(buf.size()))
            break;
        buf.resize(buf.size() * 2);
    }
    std::string path(buf.begin(), buf.begin() + size);
    boost::system::error_code ec;
    using bfs::canonical;
    bfs::path p(canonical(path, ec));
    return ec ? "" : p.make_preferred().string();
}

#elif(BOOST_OS_HAIKU)
std::string getExecutablePath()
{
	std::string ret;
	int32 cookie = 0;
	image_info info;
	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &info) == B_OK)
	{
		if (info.type == B_APP_IMAGE)
		{
			ret = info.name;
		}
	}
    return ret;
}

#else

#    error Unsupported plattform!

#endif
