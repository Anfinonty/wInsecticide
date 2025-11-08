//#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
//#include <stdio.h>
//#include <stdlib.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define WIDTH 1920
#define HEIGHT 1080
#define BUFFER_SIZE (WIDTH * HEIGHT * 4)
#define SAFE_RELEASE(x) if (x) { x->lpVtbl->Release(x); x = NULL; }

unsigned char ImgData[BUFFER_SIZE];
unsigned char tImgData[BUFFER_SIZE];

ID3D11Device* gDevice = NULL;
ID3D11DeviceContext* gContext = NULL;
IDXGIOutputDuplication* gDeskDupl = NULL;

typedef NTSTATUS(__stdcall* ZwDelayExecution_t)(BOOL, PLARGE_INTEGER);
typedef NTSTATUS(__stdcall* ZwSetTimerResolution_t)(ULONG, BOOLEAN, PULONG);

ZwDelayExecution_t ZwDelayExecution = NULL;
ZwSetTimerResolution_t ZwSetTimerResolution = NULL;

void SleepHighPrecision(__int64 ms) {
    LARGE_INTEGER interval;
    ULONG actualResolution;
    if (!ZwDelayExecution || !ZwSetTimerResolution) {
        HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
        ZwDelayExecution = (ZwDelayExecution_t)GetProcAddress(hNtDll, "ZwDelayExecution");
        ZwSetTimerResolution = (ZwSetTimerResolution_t)GetProcAddress(hNtDll, "ZwSetTimerResolution");
    }
    if (ZwSetTimerResolution) {
        ZwSetTimerResolution(1, TRUE, &actualResolution);
    }
    interval.QuadPart = -1 * ms * 10000;
    if (ZwDelayExecution) {
        ZwDelayExecution(FALSE, &interval);
    } else {
        Sleep((DWORD)ms);
    }
}

BOOL InitDuplication() {
    HRESULT hr;
    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
        NULL, 0, D3D11_SDK_VERSION, &gDevice, &featureLevel, &gContext);
    if (FAILED(hr)) return FALSE;

    IDXGIDevice* dxgiDevice = NULL;
    hr = gDevice->lpVtbl->QueryInterface(gDevice, &IID_IDXGIDevice, (void**)&dxgiDevice);
    if (FAILED(hr)) return FALSE;

    IDXGIAdapter* dxgiAdapter = NULL;
    hr = dxgiDevice->lpVtbl->GetParent(dxgiDevice, &IID_IDXGIAdapter, (void**)&dxgiAdapter);
    SAFE_RELEASE(dxgiDevice);
    if (FAILED(hr)) return FALSE;

    IDXGIOutput* dxgiOutput = NULL;
    hr = dxgiAdapter->lpVtbl->EnumOutputs(dxgiAdapter, 0, &dxgiOutput);
    SAFE_RELEASE(dxgiAdapter);
    if (FAILED(hr)) return FALSE;

    IDXGIOutput1* dxgiOutput1 = NULL;
    hr = dxgiOutput->lpVtbl->QueryInterface(dxgiOutput, &IID_IDXGIOutput1, (void**)&dxgiOutput1);
    SAFE_RELEASE(dxgiOutput);
    if (FAILED(hr)) return FALSE;

    hr = dxgiOutput1->lpVtbl->DuplicateOutput(dxgiOutput1, (IUnknown*)gDevice, &gDeskDupl);
    SAFE_RELEASE(dxgiOutput1);
    return SUCCEEDED(hr);
}

BOOL CaptureFrame() {
    HRESULT hr;
    IDXGIResource* desktopResource = NULL;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;

    gDeskDupl->lpVtbl->ReleaseFrame(gDeskDupl);
    hr = gDeskDupl->lpVtbl->AcquireNextFrame(gDeskDupl, 1000, &frameInfo, &desktopResource);
    if (FAILED(hr)) return FALSE;

    ID3D11Texture2D* acquiredImage = NULL;
    hr = desktopResource->lpVtbl->QueryInterface(desktopResource, &IID_ID3D11Texture2D, (void**)&acquiredImage);
    SAFE_RELEASE(desktopResource);
    if (FAILED(hr)) return FALSE;

    D3D11_TEXTURE2D_DESC desc;
    acquiredImage->lpVtbl->GetDesc(acquiredImage, &desc);
    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;

    ID3D11Texture2D* stagingImage = NULL;
    hr = gDevice->lpVtbl->CreateTexture2D(gDevice, &desc, NULL, &stagingImage);
    if (FAILED(hr)) {
        SAFE_RELEASE(acquiredImage);
        gDeskDupl->lpVtbl->ReleaseFrame(gDeskDupl);
        return FALSE;
    }

    gContext->lpVtbl->CopyResource(gContext, (ID3D11Resource*)stagingImage, (ID3D11Resource*)acquiredImage);
    SAFE_RELEASE(acquiredImage);
    gDeskDupl->lpVtbl->ReleaseFrame(gDeskDupl);

    IDXGISurface* surface = NULL;
    hr = stagingImage->lpVtbl->QueryInterface(stagingImage, &IID_IDXGISurface, (void**)&surface);
    SAFE_RELEASE(stagingImage);
    if (FAILED(hr)) return FALSE;

    DXGI_MAPPED_RECT mappedRect;
    hr = surface->lpVtbl->Map(surface, &mappedRect, DXGI_MAP_READ);
    if (SUCCEEDED(hr)) {
        for (int i = 0; i < HEIGHT; i++) {
            memcpy(tImgData + i * WIDTH * 4, mappedRect.pBits + i * mappedRect.Pitch, WIDTH * 4);
        }
        tImgData[BUFFER_SIZE - 1] = (rand() % 250) + 1;
        memcpy(ImgData, tImgData, BUFFER_SIZE);
        surface->lpVtbl->Unmap(surface);
    }
    SAFE_RELEASE(surface);
    return SUCCEEDED(hr);
}
