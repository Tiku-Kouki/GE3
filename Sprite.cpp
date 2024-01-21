#include "Sprite.h"
#include <DirectXMath.h>
#include "BufferResource.h"



using namespace Microsoft::WRL;
using namespace DirectX;


void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxCommon_ = dxCommon;

	common_ = common;

	//頂点 情報 
	CreateVertex();
	//色
	CreateMaterial();
	//行列
	CreateWVP();
}

void Sprite::Draw()
{
	//Y軸中心に回転
	transform.rotate.y += 0.03f;

	//ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform. scale));
	XMMATRIX rotateMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translateMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.tlanslate));

	//回転行列とスケール行列の掛け算
	XMMATRIX rotateAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	//最終的な行列変換
	XMMATRIX worldMatrix = XMMatrixMultiply(rotateAndScaleMatrix, translateMatrix);

	//
	*wvpData = worldMatrix;


	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());

	//頂点情報
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//色情報
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//行列
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	
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

	
	 *materialData = color_;
}

void Sprite::CreateWVP()
{
	wvpResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));

	

	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*wvpData = XMMatrixIdentity();

}

