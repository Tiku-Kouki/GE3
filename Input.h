#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//using namespace Microsoft::WRL;

class Input
{
public:


	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;

public:
	//初期化
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	//更新
	void Update();

	


};

