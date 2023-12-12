#pragma once

#include <Windows.h>
#include <wrl.h>
#include <string>
#include <dxcapi.h>

class SpriteCommon
{
public:

	void Initialize();

private:
	static IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

private:



};

