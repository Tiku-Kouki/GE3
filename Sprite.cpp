#include "Sprite.h"
#include <DirectXMath.h>
#include "BufferResource.h"

using namespace DirectX;

using namespace Microsoft::WRL;



void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxCommon_ = dxCommon;

	common_ = common;

	//���_ ��� 
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

	//���_�o�b�t�@�r���[���쐬����

	//���\�[�X�̐擪�̃A�h���X����g��
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	vertexBufferView.SizeInBytes = sizeof(XMFLOAT4) * 3;
	//1���_������̃T�C�Y
	vertexBufferView.StrideInBytes = sizeof(XMFLOAT4);

	//���_ ���
	XMFLOAT4* vertexDate = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDate));

	//����	
	vertexDate[0] = { -0.5f, -0.5f, 0.0f, 1.0f };

	//��
	vertexDate[1] = { +0.0f, +0.5f, 0.0f, 1.0f };

	//�E��
	vertexDate[2] = { +0.5f, -0.5f, 0.0f, 1.0f };

}

void Sprite::CreateMaterial()
{
	 materialResource= CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

	 XMFLOAT4* materialData = nullptr;

	 materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	
	materialData = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

}

