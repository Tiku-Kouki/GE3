#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"

#include<vector>

#include "TextureManager.h"

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

    ImGuiManager* imgui = ImGuiManager::Create();
    ImGuiManager::Initialize(
        winApp_->GetHwnd(), dxCommon_);

    //スプライトコモン
    SpriteCommon* spriteCommon = new SpriteCommon();
    spriteCommon->Initialize(dxCommon_);
    
    //テクスチャマネージャー
    TextureManager::GetInstance()->Initialize(dxCommon_);
    TextureManager::GetInstance()->Loadtexture(L"Resources/mario.jpg");
    TextureManager::GetInstance()->Loadtexture(L"Resources/reimu.png");

    
    std::vector<Sprite*> sprite;
    for (int i = 0; i < 2; i++) {
        Sprite* temp = new Sprite();
        
        if(i%2==1) temp->Initialize(spriteCommon, L"Resources/mario.jpg");
        else if (i % 2 == 0) temp->Initialize(spriteCommon, L"Resources/reimu.png");
        
        temp->SetPosition({ 400,200 });
        
        sprite.push_back(temp);

    }



    // ゲームループ
    while (true) {
        //更新
        if (winApp_->Update() == true) {

            break;
        }
        ImGuiManager::NewFrame();
        imgui->ShowDemo();
        //入力
        input_->Update();
        
        
        ////移動
        DirectX::XMFLOAT2 pos = sprite[1]->GetPosition();
        if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT))
        {
            if (input_->PushKey(DIK_UP)) { pos.y -= 5.0f; }
            else if (input_->PushKey(DIK_DOWN)) { pos.y += 5.0f; }

            if (input_->PushKey(DIK_RIGHT)) { pos.x += 5.0f; }
            else if (input_->PushKey(DIK_LEFT)) { pos.x -= 5.0f; }

            sprite[1]->SetPosition(pos);
        }

        ////色
        //DirectX::XMFLOAT4 color = sprite->GetColor();
        //color.x -= 0.01f;
        //if (color.x < 0) {
        //    color.x = 1.0f;
        //}
        //sprite->SetColor(color);

        ////サイズ
        //DirectX::XMFLOAT2 size = sprite->GetSize();
        //size.y += 0.01f;
        //sprite->SetSize(size);

        for (int i = 0; i < 2; i++) {
            sprite[i]->Update();
        }

        //描画前処理
        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();

        for (int i = 0; i < 2; i++) {
            sprite[i]->Draw();
        }

        //描画後処理
        ImGuiManager::CommandExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();
       

    }



    for (int i = 0; i < 2; i++) {
        delete sprite[i];
    }

    TextureManager::GetInstance()->Finalize();

    delete spriteCommon;

    delete imgui;

    delete input_;

    winApp_->Finalize();

    delete winApp_;
    delete dxCommon_;


    return 0;
}
