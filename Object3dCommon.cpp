#include "Object3dCommon.h"

void Object3dCommon::Initialize()
{
}

void Object3dCommon::rootSignature()
{
    HRESULT result = {};


    // ���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams; //���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = _countof(rootParams);        //���[�g�p�����[�^��
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;
    // ���[�g�V�O�l�`���̃V���A���C�Y
    ID3DBlob* rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(result));
    result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    assert(SUCCEEDED(result));
    rootSigBlob->Release();
    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc.pRootSignature = rootSignature.Get();
    // �p�C�v�����X�e�[�g�̐���
    ComPtr<ID3D12PipelineState> pipelineState;
    result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));

}
