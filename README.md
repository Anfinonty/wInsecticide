# wInsecticide

https://github.com/user-attachments/assets/278a0a8a-0f2e-4814-ade5-bfea84567b08



Game made with Win32API and C

Optional Extra Music for the game:
https://github.com/Anfinonty/wInsecticide-xtra

Useful Resources:
- http://www.winprog.org/tutorial/ 
- https://zetcode.com/gui/winapi/gdi/
- (Mingw32-w64) https://onboardcloud.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/8.1.0/threads-win32/dwarf/i686-8.1.0-release-win32-dwarf-rt_v6-rev0.7z

To Clone:
git clone --depth=1 https://github.com/Anfinonty/wInsecticide

To Update when in cloned repo:
git pull

To compile on Linux:
i686-w64-mingw32-gcc-win32 run.c -o "WINSECTICIDE.EXE" my.o -lusp10 -lgdi32 -lgdiplus -lmsimg32 -municode -lwinmm -lshlwapi -lvfw32

