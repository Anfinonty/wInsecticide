
//https://bobobobo.wordpress.com/2021/09/17/gdi-to-opengl/
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gdiplus.h>
//#include <string>
//#include <map>
//#include <vector>
//using namespace std;
//using namespace Gdiplus;

//#pragma comment( lib, "gdiplus.lib" )
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")

#define PRINT 0

struct Globals
{
    HINSTANCE hInstance;
    HWND hwnd;
    HDC   hdc;
    HGLRC hglrc;
    GLuint texId;
    int width, height;
};

Globals g;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);
void draw();


// GL ERR
map<int, const char*> createErrMap()
{
    map<int, const char*> errmap;
    errmap.insert(make_pair(0x0000, "GL_NO_ERROR"));
    errmap.insert(make_pair(0x0500, "GL_INVALID_ENUM"));
    errmap.insert(make_pair(0x0501, "GL_INVALID_VALUE"));
    errmap.insert(make_pair(0x0502, "GL_INVALID_OPERATION"));
    errmap.insert(make_pair(0x0503, "GL_STACKOVERFLOW"));
    errmap.insert(make_pair(0x0504, "GL_STACK_UNDERFLOW"));
    errmap.insert(make_pair(0x0505, "GL_OUTOFMEMORY"));

    errmap.insert(make_pair(0x8CD5, "GL_FRAMEBUFFER_COMPLETE"));
    errmap.insert(make_pair(0x8CD6, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"));
    errmap.insert(make_pair(0x8CD7, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"));
    errmap.insert(make_pair(0x8CD9, "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS"));
    errmap.insert(make_pair(0x8CDD, "GL_FRAMEBUFFER_UNSUPPORTED"));
    return errmap;
}
map<int, const char*> glErrName = createErrMap();
inline bool GL_OK(int line, const char* file)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf("GLERROR %d %s, line=%d of file=%s\n", err, glErrName[err], line, file);
    return err == GL_NO_ERROR;
}
#define CHECK_GL GL_OK( __LINE__, __FILE__ ) 

/*
const TCHAR* GetStatusString(Gdiplus::Status status)
{
    const TCHAR* statuses[] = {
      TEXT("Ok: Indicates that the method call was successful."),
      TEXT("GenericError: Indicates that there was an error on the method call, which is identified as something other than those defined by the other elements of this enumeration."),
      TEXT("InvalidParameter: Indicates that one of the arguments passed to the method was not valid."),
      TEXT("OutOfMemory: Indicates that the operating system is out of memory and could not allocate memory to process the method call. For an explanation of how constructors use the OutOfMemory status, see the Remarks section at the end of this topic."),
      TEXT("ObjectBusy: Indicates that one of the arguments specified in the API call is already in use in another thread."),
      TEXT("InsufficientBuffer: Indicates that a buffer specified as an argument in the API call is not large enough to hold the data to be received."),
      TEXT("NotImplemented: Indicates that the method is not implemented."),
      TEXT("Win32Error: Indicates that the method generated a Win32 error."),
      TEXT("WrongState: Indicates that the object is in an invalid state to satisfy the API call. For example, calling Pen::GetColor from a pen that is not a single, solid color results in a WrongState status."),
      TEXT("Aborted: Indicates that the method was aborted."),
      TEXT("FileNotFound: Indicates that the specified image file or metafile cannot be found."),
      TEXT("ValueOverflow: Indicates that the method performed an arithmetic operation that produced a numeric overflow."),
      TEXT("AccessDenied: Indicates that a write operation is not allowed on the specified file."),
      TEXT("UnknownImageFormat: Indicates that the specified image file format is not known."),
      TEXT("FontFamilyNotFound: Indicates that the specified font family cannot be found. Either the font family name is incorrect or the font family is not installed."),
      TEXT("FontStyleNotFound: Indicates that the specified style is not available for the specified font family."),
      TEXT("NotTrueTypeFont: Indicates that the font retrieved from an HDC or LOGFONT is not a TrueType font and cannot be used with GDI+."),
      TEXT("UnsupportedGdiplusVersion: Indicates that the version of GDI+ that is installed on the system is incompatible with the version with which the application was compiled."),
      TEXT("GdiplusNotInitialized: Indicates that the GDI+API is not in an initialized state. To function, all GDI+ objects require that GDI+ be in an initialized state. Initialize GDI+ by calling GdiplusStartup."),
      TEXT("PropertyNotFound: Indicates that the specified property does not exist in the image."),
      TEXT("PropertyNotSupported: Indicates that the specified property is not supported by the format of the image and, therefore, cannot be set."),
      TEXT("ProfileNotFound: Indicates that the color profile required to save an image in CMYK format was not found."),
      TEXT("INVALID STATUS CODE")
    };
    if (status < 0 || status > Gdiplus::Status::PropertyNotSupported + 1) // ProfileNotFound may not be there
        status = (Gdiplus::Status)(Gdiplus::Status::PropertyNotSupported + 2); // gives last error (INVALID STATUS CODE)

    return statuses[status];
}*/

class StopWatch
{
    LARGE_INTEGER lastTime;
    double fFreq;
    double totalTime;

public:
    StopWatch()
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        fFreq = (double)freq.QuadPart;
        reset();
    }

    void reset()
    {
        totalTime = 0.;
        QueryPerformanceCounter(&lastTime);
    }

    double getTime()
    {
        LARGE_INTEGER thisTime;
        QueryPerformanceCounter(&thisTime);
        totalTime += (thisTime.QuadPart - lastTime.QuadPart) / fFreq;
        lastTime = thisTime;
        return totalTime;
    }
};



#define AT(COL,ROW,WIDTH) (COL+ROW*WIDTH)

void genTexture(GLuint& texId, int w, int h, const vector<DWORD>& argb)
{
    glGenTextures(1, &texId);  CHECK_GL;
    glBindTexture(GL_TEXTURE_2D, texId);  CHECK_GL;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  CHECK_GL;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  CHECK_GL;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  CHECK_GL;
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  CHECK_GL;
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);  CHECK_GL;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, &argb[0]);  CHECK_GL;
}

void GetBits(Bitmap& bmp, vector<DWORD>& argb, bool invert = 0)
{
    argb.resize(bmp.GetWidth() * bmp.GetHeight());

    Color c;
    for (int i = 0; i < bmp.GetWidth(); i++)
    {
        for (int j = 0; j < bmp.GetHeight(); j++)
        {
            if (invert)  bmp.GetPixel(i, bmp.GetHeight() - 1 - j, &c);
            else  bmp.GetPixel(i, j, &c);
            argb[AT(i, j, bmp.GetWidth())] = c.GetValue();
            if (PRINT) printf("%03d %03d %03d %03d  ", c.GetR(), c.GetG(), c.GetB(), c.GetA());
        }
        if (PRINT) puts("");
    }
}

void GetBitsLockBits(Bitmap& bmp, vector<DWORD>& argb, bool invert = 0)
{
    BitmapData bmpData;
    RectF rectf;
    Unit unit;
    bmp.GetBounds(&rectf, &unit);
    Rect rect(rectf.X, rectf.Y, rectf.Width, rectf.Height);
    printf("Got rect %d %d %d %d\n", rect.X, rect.Y, rect.Width, rect.Height);
    bmp.LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);
    printf("BMP has w=%d h=%d stride=%d\n", bmpData.Width, bmpData.Height, bmpData.Stride);
    argb.resize(bmpData.Width * bmpData.Height);

    // The stride is the number of BYTES in a row.
    // It is negative if the bitmap is bottom-up.
    // Didn't use stride because of the possibility of it being negative, or including padding.
    if (invert)
        for (int i = 0; i < bmpData.Height; i++)
            memcpy(&argb[i * bmpData.Width], (GLuint*)bmpData.Scan0 + (bmpData.Height - 1 - i) * bmpData.Width, bmpData.Width * 4);
    else if (bmpData.Stride == bmpData.Width * 4)
        memcpy(&argb[0], bmpData.Scan0, bmpData.Width * bmpData.Height * 4); // If the bmp is padded then
    // this won't read the image correctly (it will read it with pad bits between)
    else
        for (int i = 0; i < bmpData.Height; i++)
            memcpy(&argb[i * bmpData.Width], (GLuint*)bmpData.Scan0 + i * bmpData.Width, bmpData.Width * 4);
    bmp.UnlockBits(&bmpData);
}

void MakeTexture(GLuint& texId)
{
    Bitmap offscreen(512, 512, PixelFormat32bppARGB);
    Graphics gr(&offscreen);

    gr.Clear(Color(128, 255, 0, 0));
    Gdiplus::SolidBrush brush(Color(255, 0, 0, 255));
    Pen pen(Color(128, 255, 0, 0), 16.f);
    Font font(L"Arial", 48.f);
    Rect r(25, 25, 100, 100);
    gr.DrawRectangle(&pen, r);
    gr.DrawString(TEXT("TEST STRING"), -1, &font, PointF(50, 50), &brush);

    vector<DWORD> argb;
    GetBitsLockBits(offscreen, argb, 1);
    genTexture(texId, offscreen.GetWidth(), offscreen.GetHeight(), argb);
}

bool LoadTexture(GLuint& texId, const WCHAR* filename)
{
    Bitmap bmp(filename);
    Status status = bmp.GetLastStatus();
    if (status)
    {
        wprintf(TEXT("Error loading texture file: %s\n"), GetStatusString(status));
        return 0;
    }

    StopWatch s;
    vector<DWORD> argb;
    GetBitsLockBits(bmp, argb, 1);
    wprintf(TEXT("%s took %f seconds to load\n"), filename, s.getTime());
    genTexture(texId, bmp.GetWidth(), bmp.GetHeight(), argb);
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);

    // Start up GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    g.hInstance = hInstance;

    //const TCHAR* wndclassName = TEXT("opengl");
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = wndclassName;
    wc.lpszMenuName = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    RegisterClass(&wc);

    RECT rect = { 50, 50, 850, 650 };

    g.width = rect.right - rect.left;
    g.height = rect.bottom - rect.top;

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    g.hwnd = CreateWindow(wndclassName,
        TEXT("GL WINDOW!"),
        WS_OVERLAPPEDWINDOW,
        rect.left, rect.top,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL,
        hInstance, NULL);

    if (!g.hwnd)  FatalAppExit(NULL, TEXT("CreateWindow() failed!"));

    ShowWindow(g.hwnd, iCmdShow);

    g.hdc = GetDC(g.hwnd);

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;

    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;

    int chosenPixelFormat = ChoosePixelFormat(g.hdc, &pfd);
    if (!chosenPixelFormat)  FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));

    printf("You got ID# %d as your pixelformat!\n", chosenPixelFormat);

    int result = SetPixelFormat(g.hdc, chosenPixelFormat, &pfd);

    if (!result)  FatalAppExit(NULL, TEXT("SetPixelFormat() failed!"));
    g.hglrc = wglCreateContext(g.hdc);

    wglMakeCurrent(g.hdc, g.hglrc);

    //LoadTexture(g.texId, TEXT("face16x16.png"));
    //LoadTexture(g.texId, TEXT("picture-1.jpg"));
    MakeTexture(g.texId);

    MSG msg;
    while (1)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)  break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            draw();
        }
    }

    wglMakeCurrent(0, 0);
    wglDeleteContext(g.hglrc);
    ReleaseDC(g.hwnd, g.hdc);

    AnimateWindow(g.hwnd, 200, AW_HIDE | AW_BLEND);
    GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}

void draw()
{
    glViewport(0, 0, g.width, g.height);  CHECK_GL;

    glMatrixMode(GL_PROJECTION);  CHECK_GL;
    glLoadIdentity();  CHECK_GL;
    //gluPerspective(45.0, (float)g.width / (float)g.height, 1, 1000);  CHECK_GL;

    glMatrixMode(GL_MODELVIEW);  CHECK_GL;
    glLoadIdentity();  CHECK_GL;

    glEnable(GL_TEXTURE_2D);  CHECK_GL;
    glEnable(GL_BLEND);  CHECK_GL;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  CHECK_GL;
    glBindTexture(GL_TEXTURE_2D, g.texId);  CHECK_GL;

    //gluLookAt(0, 0, 10,   0, 0, 0,   0, 1, 0);  CHECK_GL;

    glClearColor(0.5, 0, 0, 0);  CHECK_GL;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  CHECK_GL;

    // 3--2
    // |  |
    // 4--1
    //
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 0);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0);

    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 0);

    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 0);
    glEnd();

    SwapBuffers(g.hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_CREATE:
        Beep(50, 10);
        return 0;

    case WM_KEYDOWN:
        switch (wparam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        default:
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}
