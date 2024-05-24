/*
===============================================================================
    Copyright (C) 2024 Ilya Lyakhovets

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
===============================================================================
*/

#include <windows.h>
#include <cstdio>
#include <iostream>
//#include <filesystem>
#include <vector>
#include <string>
#include "libtga/tga.h"

using namespace std;

//filesystem::path p;

bool IsFolder(const WCHAR *path)
{
    DWORD attributes;

    if ((attributes = GetFileAttributesW(path)) == INVALID_FILE_ATTRIBUTES)
        return false;

    return attributes & FILE_ATTRIBUTE_DIRECTORY ? true : false;
}

void FindFiles(const WCHAR *path, vector<wstring> &files)
{
    WIN32_FIND_DATA findData;
    HANDLE handle;

    wstring pathWithWildcard(path);
    pathWithWildcard.append(L"/*.tga*");

    wstring dirPath(pathWithWildcard);

    size_t index = dirPath.find_last_of(L"\\");
    size_t index2 = dirPath.find_last_of(L"/");

    if (index2 != string::npos && (index == string::npos || index2 > index))
        index = index2;

    if (index != string::npos)
        dirPath.erase(index, dirPath.size() - index);
    
    wstring dirPathWithWildcard(dirPath);
    dirPathWithWildcard.append(L"/*");

    // Looks for files first using a pathWithWildcard filter
    if ((handle = FindFirstFile(pathWithWildcard.c_str(), &findData)) != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                wstring filepath(path);
                filepath.append(L"/");
                filepath.append(findData.cFileName);
                files.push_back(filepath);
            }

        } while (FindNextFile(handle, &findData));

        FindClose(handle);
    }

    // Searches recursively for folders
    if ((handle = FindFirstFile(dirPathWithWildcard.c_str(), &findData)) != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Skips . and .. folders
                if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
                    continue;

                wstring subPath;
                subPath.append(dirPath);
                subPath.append(L"/");
                subPath.append(findData.cFileName);

                FindFiles(subPath.c_str(), files);
            }
        } while (FindNextFile(handle, &findData));

        FindClose(handle);
    }
}

int wmain(int argc, WCHAR *argv[])
{
    vector<wstring> files;
    setlocale(LC_ALL, ".UTF8");

    if (argc != 3)
    {
        wprintf(L"Usage: %s <tga/folder path> <format>\n\n"
                L"Formats:\n"
                L"0 - Uncompressed, 8-bit color-mapped image.\n"
                L"1 - Uncompressed, 24-bit or 32-bit true-color image.\n"
                L"2 - Uncompressed, 15-bit or 16-bit true-color image.\n"
                L"3 - Uncompressed, 16-bit black-and-white image.\n"
                L"4 - Uncompressed, 8-bit black-and-white image.\n"
                L"5 - Run-length encoded, 8-bit color-mapped image.\n"
                L"6 - Run-length encoded, 24-bit or 32-bit true-color image.\n"
                L"7 - Run-length encoded, 15-bit or 16-bit true-color image.\n"
                L"8 - Run-length encoded, 16-bit black-and-white image.\n"
                L"9 - Run-length encoded, 8-bit black-and-white image.\n", argv[0]);

        return -1;
    }
    
    tga_type mode = static_cast<tga_type>(_wtoi(argv[2]));

    if (IsFolder(argv[1]))
        FindFiles(argv[1], files);
    else
        files.push_back(argv[1]);

    for (auto &path : files)
    {
        tga_image tga;

        if (!wload_tga(path.c_str(), &tga))
        {
            wprintf(L"%s - couldn't open.\n", path.c_str());
            continue;
        }

        if (wsave_tga(path.c_str(), &tga, mode))
            wprintf(L"%s - success\n", path.c_str());
        else
            wprintf(L"%s - failure\n", path.c_str());
    }

    return 0;
}
