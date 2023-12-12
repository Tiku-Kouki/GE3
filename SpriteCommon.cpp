#include "SpriteCommon.h"
#include <cassert>
#include <d3dx12.h>


#pragma comment(lib,"dxcompiler.lib")

using namespace Microsoft::WRL;

void SpriteCommon::Initialize()
{
	HRESULT result{};

	//DXC������
	ComPtr<IDxcUtils>dxcUils;
	ComPtr<IDxcCompiler3> dxcCompiler;
	ComPtr<IDxcIncludeHandler>	includeHandler ;

	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUils));
	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(result));

	
	result = dxcUils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(result));

	//RootSig
	D3D12_ROOT_SIGNATURE_DESC descriptorRootSignature{};

}

IDxcBlob* SpriteCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	HRESULT result{};

	//hlsl
	IDxcBlobEncoding* shaderSource = nullptr;
	 result = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

	assert(SUCCEEDED(result));

	DxcBuffer shaderSourceBuffer = {};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;


	//2.Compile����
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//���ۂ�Shader���R���p�C������
	IDxcResult* shaderResult = nullptr;
	result = dxcCompiler->Compile(
		&shaderSourceBuffer,//�ǂݍ��񂾃t�@�C��
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)

	);
	assert(SUCCEEDED(result));

	//3.�x���E�G���[���łĂ��Ȃ����m�F����
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		
		//�x���E�G���[�_���[�b�^�C
		assert(false);
	}

	//4.Compile���ʂ��󂯎���ĕԂ�
	//�R���p�C�����ʂ�����s�p�̃o�C�i���������擾
	IDxcBlob* ShaderBlob = nullptr;
	result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&ShaderBlob), nullptr);
	assert(SUCCEEDED(result));
	//�����g��Ȃ����\�[�X�����U
	shaderSource->Release();
	shaderResult->Release();
	//���s�p�̃o�C�i����ԋp
	return ShaderBlob;

	
}
