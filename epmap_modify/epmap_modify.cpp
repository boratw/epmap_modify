﻿
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;
typedef wchar_t wchar;

int wmain(int argc, const wchar* argv[])
{
    if (argc > 1)
    {
        if (wcslen(argv[1]) < 6)
            return -1;
        if (wcscmp(argv[1] + (wcslen(argv[1]) - 6), L".epmap") != 0)
            return -1;
        wchar output_temp[256];
        swprintf_s(output_temp, L"%s.tmp", argv[1]);

        wifstream ifs(argv[1]);
        if(ifs.fail())
            return -1;
        wofstream ofs(output_temp);
        if(ofs.fail())
            return -1;

        wchar buffer[1024];
        wchar* delim;
        int offset;
        wchar offsettmp[1024];
        while(!ifs.eof())
        {
            ifs.getline(buffer, 1024);
            delim = wcschr(buffer, L'%');
            if ((delim != NULL) && ((buffer - delim) < 14))
            {
                *delim = 0;
                swscanf_s(buffer + 2, L"%d", &offset);
                swprintf_s(offsettmp, L"%08X", offset);
                ofs.write(L" - ", 3);
                ofs.write(offsettmp, wcslen(offsettmp));
                ofs.write(delim + 4, wcslen(delim + 4));
                ofs.write(L"\n", 1);
            }
            else
            {
                ofs.write(buffer, wcslen(buffer));
                ofs.write(L"\n", 1);
            }
        }

        ofs.flush();
        ifs.close();
        ofs.close();

        ifs = wifstream(output_temp);
        if(ifs.fail())
            return -1;
        ifs.close();

        DeleteFile(argv[1]);
        MoveFile(output_temp, argv[1]);
    }
    return 0;
}
