#include "Sprite.h"
#include <DirectXMath.h>
#include "BufferResource.h"



using namespace Microsoft::WRL;
using namespace DirectX;


void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxCommon_ = dxCommon;

	common_ = common;

	//���_ ��� 
	CreateVertex();
	//�F
	CreateMaterial();
	//�s��
	CreateWVP();
}

void Sprite::Draw()
{
	//Y�����S�ɉ�]
	transform.rotate.y += 0.03f;

	//���[���h
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform. scale));
	XMMATRIX rotateMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translateMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.tlanslate));

	//��]�s��ƃX�P�[���s��̊|���Z
	XMMATRIX rotateAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	//�ŏI�I�ȍs��ϊ�
	XMMATRIX worldMatrix = XMMatrixMultiply(rotateAndScaleMatrix, translateMatrix);

	//
	*wvpData = worldMatrix;


	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());

	//���_���
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�F���
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//�s��
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	
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

	
	 *materialData = color_;
}

void Sprite::CreateWVP()
{
	wvpResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));

	

	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*wvpData = XMMatrixIdentity();

}

