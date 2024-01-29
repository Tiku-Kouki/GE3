#pragma once
#include "DirectXCommon.h"
#include <string>
#include <DirectXTex.h>

class TextureManager
{
private:
	//�摜��`�悷��̂ɕK�v
	struct TextureDate {
		std::wstring filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE	sevHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE	srvHandleGPU;
	};

public:
	// �V���O���g��
	static TextureManager* GetInstance();
	void Finalize();

	void Initialize(DirectXCommon* dxCommon);

	//�摜�ǂݍ���
	void Loadtexture(const std::wstring& filePath);

private:
	// �ǂݍ��񂾉摜��GPU(�V�F�[�_�[�ɑ���)
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImage);


private:
	static TextureManager* instance;

	static uint32_t kSRVIndexTop;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

private:
	DirectXCommon* dxCommon_ = nullptr;

	std::vector<TextureDate> textureDatas;

};

