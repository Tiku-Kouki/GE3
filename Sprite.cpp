#include "Sprite.h"
#include <DirectXMath.h>
#include "BufferResource.h"
#include "ImGuiManager.h"

#include"externals/imgui/imgui.h"


using namespace Microsoft::WRL;
using namespace DirectX;


void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxCommon_ = dxCommon;

	common_ = common;

	//画像の読み取り
	DirectX::ScratchImage mipImages = common->Loadtexture(L"Resources/mario.jpg");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTexureResource(dxCommon_->GetDevice(),metaData);
	common_->UploadTextureData(textureResource, mipImages);
	
	
	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	////SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);

	



	//頂点 情報 
	CreateVertex();
	//色
	CreateMaterial();
	//行列
	CreateWVP();
}

void Sprite::Update()
{
	ImGui::Begin("texture");

	ImGui::DragFloat3("pos", &transform.tlanslate.x, 0.01f);

	ImGui::End();


}

void Sprite::Draw()
{
	//Y軸中心に回転
	transform.rotate.y += 0.06f;

	//ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform. scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translateMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.tlanslate));

	//回転行列とスケール行列の掛け算
	XMMATRIX rotateAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	//最終的な行列変換
	XMMATRIX worldMatrix = XMMatrixMultiply(rotateAndScaleMatrix, translateMatrix);

	//カメラ
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslateMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.tlanslate));

	//回転行列とスケール行列の掛け算
	XMMATRIX cameraRotateAndScaleMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);
	//最終的な行列変換
	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotateAndScaleMatrix, cameraTranslateMatrix);

	//View
	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.f),
		(float)WinApp::window_width / (float)WinApp::window_height,
		0.1f,
		100.f

	);
	//WVP
	XMMATRIX worldViewProjectionMatrix = worldMatrix * (view * proj);


	//
	*wvpData = worldViewProjectionMatrix;


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
	vertexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 3);

	//頂点バッファビューを作成する

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	//1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点 情報
	VertexData* vertexDate = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDate));

	//左下	
	vertexDate[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
	vertexDate[0].texcoord = { 0.0f, 1.0f };
	//上
	vertexDate[1].position = { +0.0f, +0.5f, 0.0f, 1.0f };
	vertexDate[1].texcoord = { 0.5f, 0.0f };

	//右下
	vertexDate[2].position = { +0.5f, -0.5f, 0.0f, 1.0f };
	vertexDate[2].texcoord = { 1.0f, 1.0f };

}

void Sprite::CreateMaterial()
{
	 materialResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

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

