#pragma once

#include <cassert>
#include <tuple>
#include <filesystem>

#include <d3d11.h>
#include <comdef.h>

namespace quote{ namespace d3d11{

	namespace detail {
		_COM_SMARTPTR_TYPEDEF(ID3D11Device, __uuidof(ID3D11Device));
		_COM_SMARTPTR_TYPEDEF(ID3D11DeviceContext, __uuidof(ID3D11DeviceContext));
		_COM_SMARTPTR_TYPEDEF(IDXGISwapChain, __uuidof(IDXGISwapChain));
		_COM_SMARTPTR_TYPEDEF(ID3D11Texture2D, __uuidof(ID3D11Texture2D));
		_COM_SMARTPTR_TYPEDEF(ID3D11RenderTargetView, __uuidof(ID3D11RenderTargetView));
	}

	template <class Derived>
	class device {
	public:
		detail::IDXGISwapChainPtr swap_chain_;
		detail::ID3D11DevicePtr device_;
		detail::ID3D11DeviceContextPtr device_ctx_;

	public:
		~device()
		{
			discard_device();
		}

		bool create_device(bool single_thread, const DXGI_SWAP_CHAIN_DESC &desc)
		{
			assert(swap_chain_ == nullptr);
			assert(device_ == nullptr);
			assert(device_ctx_ == nullptr);

#if defined(NDEBUG)
			UINT flags = 0;
#else
			UINT flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SWITCH_TO_REF;
#endif

			if (single_thread) {
				flags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
			}

			HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_HARDWARE,
				nullptr, flags,
				nullptr, 0, D3D11_SDK_VERSION,
				&desc, &swap_chain_, &device_, nullptr, &device_ctx_);

			return SUCCEEDED(hr);
		}

		bool create_device(bool single_thread = true)
		{
			DXGI_SWAP_CHAIN_DESC desc = {};

			std::tie(desc.BufferDesc.Width, desc.BufferDesc.Height) =
				std::tuple<UINT, UINT>(static_cast<Derived*>(this)->get_size());
			desc.BufferDesc.RefreshRate.Numerator = 60u;
			desc.BufferDesc.RefreshRate.Denominator = 1u;
			desc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;

			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;

			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = 2;
			desc.OutputWindow = static_cast<Derived*>(this)->get_hwnd();
			desc.Windowed = TRUE;
			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			return create_device(single_thread, desc);
		}

		void discard_device()
		{
			swap_chain_.Release();
			device_.Release();
			device_ctx_.Release();
		}

		bool create_render_target_view()
		{
			assert(swap_chain_ != nullptr);

			HRESULT hr;

			detail::ID3D11Texture2DPtr buffer;

			hr = swap_chain_->GetBuffer(0, IID_PPV_ARGS(&buffer));

			if (FAILED(hr)) {
				return false;
			}

			detail::ID3D11RenderTargetViewPtr target_view;

			hr = device_->CreateRenderTargetView(buffer, nullptr, &target_view);

			if (FAILED(hr)) {
				return false;
			}

			device_ctx_->OMSetRenderTargets(1, &target_view, nullptr);

			D3D11_VIEWPORT viewport = {};

			std::tie(viewport.Width, viewport.Height) =
				std::tuple<FLOAT, FLOAT>(static_cast<Derived*>(this)->get_size());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			device_ctx_->RSSetViewports(1, &viewport);

			return true;
		}

		void discard_render_target_view()
		{
			
		}

		void on_size(int w, int h)
		{
			assert(swap_chain_ != nullptr);
			assert(device_ != nullptr);

			/*DXGI_SWAP_CHAIN_DESC desc;

			swap_chain_->GetDesc(&desc);

			device_ctx_->OMSetRenderTargets()

			swap_chain_->ResizeBuffers()*/
		}
	};

} }
