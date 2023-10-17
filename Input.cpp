﻿#include "Input.h"
#include <cassert>





void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
    HRESULT result;

  
    result = DirectInput8Create(
        hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

   
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));


}

void Input::Update()
{
   //前回のキー入力を保存
    memcpy(preKey, key, sizeof(key));

    // キーボード情報の取得開始
    keyboard->Acquire();
    // 全キーの入力状態を取得する
    keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
    if (key[keyNumber]) {
        return true;
    }

    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{

    if (key[keyNumber]&&preKey[keyNumber] == 0) {
        return true;
    }

    return false;
}
