#include "Sprite.h"
#include <DirectXMath.h>
#include "BufferResource.h"

using namespace DirectX;

using namespace Microsoft::WRL;



void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxCommon_ = dxCommon;

	common_ = common;

	//頂点 情報 
	CreateVertex();

	CreateMaterial();

}

void Sprite::Draw()
{
	
	
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());

	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->DrawInstanced(3, 1,0, 0);


}

void Sprite::CreateVertex()
{
	vertexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4) * 3);

	//頂点バッファビューを作成する

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(XMFLOAT4) * 3;
	//1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(XMFLOAT4);

	//頂点 情報
	XMFLOAT4* vertexDate = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDate));

	//左下	
	vertexDate[0] = { -0.5f, -0.5f, 0.0f, 1.0f };

	//上
	vertexDate[1] = { +0.0f, +0.5f, 0.0f, 1.0f };

	//右下
	vertexDate[2] = { +0.5f, -0.5f, 0.0f, 1.0f };

}

void Sprite::CreateMaterial()
{
	 materialResource= CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

	 XMFLOAT4* materialData = nullptr;

	 materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	
	materialData = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

}

