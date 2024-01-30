#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>

#include <DirectXMath.h>


#include "SpriteCommon.h"


class Sprite
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	struct Transform {
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotate;
		DirectX::XMFLOAT3 tlanslate;
	};

	struct VertexData {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;
		
	};
	struct MaterialDate {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX uvTransform;
		
	};

public:

	void Initialize(SpriteCommon* common,std::wstring textureFilePath);

	void Update();

	void Draw();

	//Getter/Setter
	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation() { return rotation; }
	DirectX::XMFLOAT4 GetColor() { return color_; }
	DirectX::XMFLOAT2 GetSize() { return size; }
	 //
	DirectX::XMFLOAT2 GetAnchorPoint() { return anchorPoint; }
	//左右反転
	bool GetFlipX() { return isFlipX; }
	//上下反転
	bool GetFlipY() { return isFlipY; }

	DirectX::XMFLOAT2 GetTextureLeftTop() { return textureLeftTop; }

	DirectX::XMFLOAT2 GetTextureSize()    {return textureSize;}




	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }
	//
	void  SetAnchorPoint(DirectX::XMFLOAT2 anchor) { anchorPoint = anchor; }

	//左右反転
	void SetFlipX(bool isFlip) { isFlipX = isFlip; }
	//上下反転
	void SetFlipY(bool isFlip) { isFlipY = isFlip; }

	void SetTextureLeftTop(DirectX::XMFLOAT2 leftTop) { textureLeftTop = leftTop; }

	void SetTextureSize(DirectX::XMFLOAT2 size) { textureSize = size; }


	void SetTexture(std::wstring textureFilePath);



private:
	//頂点情報作成
	void CreateVertex();
	//インデックス情報作成
	void CreateIndex();

	//マテリアル情報作成
	void CreateMaterial();
	 //行列情報作成
	void CreateWVP();
	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private:
	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	 //頂点情報
	ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexDate = nullptr;


	//インデックス
	ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};


	//マテリアル情報
	ComPtr<ID3D12Resource> materialResource;
	MaterialDate* materialData = nullptr;


	//行列情報
	ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	

	 //パラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//						   scale   rotate  translate
	Transform uvTransform = { {1,1,1},{0,0,0},{0,0,0} };


	 //	自機				     scale	 rotate  translate
	Transform transform = { {1,1,1}, {0,0,0}, {0,0,0} };
	DirectX::XMFLOAT2 position = { 0,0 };
	float rotation = 0;
	DirectX::XMFLOAT2 size = { 512,512 };


	DirectX::XMFLOAT2 anchorPoint = { 0.5f,0.5f };

	//左右反転
	bool isFlipX = false;
	//上下反転
	bool isFlipY = false;

	//切り抜き
	//テクスチャ左上座標
	DirectX::XMFLOAT2 textureLeftTop = { 0.0f,0.0f };

	DirectX::XMFLOAT2 textureSize = { 414.0f,400.0f };

	//テクスチャ番号
	uint32_t textureIndex_ = 0;

	//カメラ
	Transform cameraTransform = { {1,1,1}, {0,0,0}, {0,0,-5} };

	



};

