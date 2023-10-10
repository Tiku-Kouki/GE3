﻿#pragma once
#include <windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



class Input
{
public:


	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//初期化
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	//更新
	void Update();


private:
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard;

};

