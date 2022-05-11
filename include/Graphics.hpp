#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "RWin.hpp"
#include <d3d11.h>

class Graphics {
public:
    Graphics(HWND hWnd);

    ~Graphics();

    Graphics(const Graphics& graphics) = delete;

    Graphics& operator=(const Graphics& graphics) = delete;

    void endFrame();

    void clearBuffer(float red, float green, float blue) noexcept;

private:
    ID3D11Device *device_ptr = nullptr;
    IDXGISwapChain *swap_ptr = nullptr;
    ID3D11DeviceContext *context_ptr = nullptr;
    ID3D11RenderTargetView *view_ptr = nullptr;
};

#endif // GRAPHICS_HPP