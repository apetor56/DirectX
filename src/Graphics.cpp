#include "Graphics.hpp"

Graphics::Graphics(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sc_des = {};
    sc_des.BufferDesc.Height = 0;
    sc_des.BufferDesc.Width = 0;
    sc_des.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sc_des.BufferDesc.RefreshRate.Numerator = 0;
    sc_des.BufferDesc.RefreshRate.Denominator = 0;
    sc_des.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sc_des.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sc_des.SampleDesc.Count = 1;
    sc_des.SampleDesc.Quality = 0;
    sc_des.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sc_des.BufferCount = 1;
    sc_des.OutputWindow = hWnd;
    sc_des.Windowed = TRUE;
    sc_des.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sc_des.Flags = 0;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sc_des,
        &swap_ptr,
        &device_ptr,
        nullptr,
        &context_ptr
    );

    ID3D11Resource *backBuffer_ptr = nullptr;
    swap_ptr->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer_ptr));
    device_ptr->CreateRenderTargetView(
        backBuffer_ptr,
        nullptr,
        &view_ptr
    );

    backBuffer_ptr->Release();
}

Graphics::~Graphics() {
    if(view_ptr != nullptr) {
        view_ptr->Release();
    }
    if(swap_ptr != nullptr) {
        swap_ptr->Release();
    }
    if(device_ptr != nullptr) {
        device_ptr->Release();
    }
    if(context_ptr != nullptr) {
        context_ptr->Release();
    }
}

void Graphics::endFrame() {
    swap_ptr->Present(1u, 0u);
}

void Graphics::clearBuffer(float red, float green, float blue) noexcept {
    const float color[] = {red, green, blue, 1.0f};
    context_ptr->ClearRenderTargetView(view_ptr, color);
}