
#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
#include <vector>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;



class DirectXCommon
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//������
	void Initialize(WinApp* winApp);
	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	ComPtr<ID3D12Device>  GetDevice() const { return device; }

private:
	//�f�o�C�X
	void DeviceInitialize();
	//�R�}���h
	void CommandInitialize();
	//�X���b�v�`�F�[��
	void SwapChainInitialize();
	//�����_�^�[�Q�b�g
	void RenderTargetInitialize();
	//�[�x�o�b�t�@
	void DepthBufferTargetInitialize();
	//�t�F���X
	void FenceTargetInitialize();



private:
	//WindowsAPI
	WinApp* winApp = nullptr;

	

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;

	
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	ComPtr<IDXGISwapChain4> swapChain;

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;


	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	
	D3D12_RESOURCE_BARRIER barrierDesc{};
};