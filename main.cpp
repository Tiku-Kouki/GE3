#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    //ポインタ置き場
    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;

#pragma region WindowsAPI初期化処理
    winApp_ = new WinApp();
    winApp_->Initialize();

#pragma endregion

#pragma region DirectX初期化処理
    dxCommon_ = new DirectXCommon();
    dxCommon_->Initialize(winApp_);

  #pragma endregion

    input_ = new Input();
    input_->Initialize(winApp_);


    // ゲームループ
    while (true) {
        //更新
        if (winApp_->Update() == true) {

            break;
        }
         //入力
        input_->Update();
       
        //描画前処理
        dxCommon_->PreDraw();

       
        dxCommon_->PostDraw();
       

    }

    delete input_;

    winApp_->Finalize();

    delete winApp_;
    delete dxCommon_;


    return 0;
}
