#include "Graphics.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace wrl = Microsoft::WRL;

// 'hr' must exist locally in function!
#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))

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

    HRESULT hr;

    GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
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
    ));

    wrl::ComPtr<ID3D11Resource> backBuffer_ptr;
    GFX_THROW_FAILED(swap_ptr->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer_ptr));
    GFX_THROW_FAILED(device_ptr->CreateRenderTargetView(
        backBuffer_ptr.Get(),
        nullptr,
        &view_ptr
    ));
}

void Graphics::endFrame() {
    HRESULT hr;

    if(FAILED(hr = swap_ptr->Present(1u, 0u))) {
        if(hr == DXGI_ERROR_DEVICE_REMOVED) {
            GFX_DEVICE_REMOVED_EXCEPT(device_ptr->GetDeviceRemovedReason());
        }
        else {
            GFX_THROW_FAILED(hr);
        }
    }
}

void Graphics::clearBuffer(float red, float green, float blue) noexcept {
    const float color[] = {red, green, blue, 1.0f};
    context_ptr->ClearRenderTargetView(view_ptr.Get(), color);
}

// graphics exception implementation

Graphics::HrException::HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line,file ),
	hr( hr )
{
	// join all info messages with newlines into single string
	for( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl;

	if( !info.empty() )
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::getType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}


const char* Graphics::DeviceRemovedException::getType() const noexcept
{
	return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
Graphics::InfoException::InfoException( int line,const char * file,std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line,file )
{
	// join all info messages with newlines into single string
	for( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if( !info.empty() )
	{
		info.pop_back();
	}
}


const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::getType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}