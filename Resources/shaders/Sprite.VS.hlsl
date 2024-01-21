#include "Sprite.hlsli"

struct TransformationMatrix
{

float4x4 WVP;

};
ConstantBuffer<TransformationMatrix>gTransmationMatrix : register(b0);

struct VertexShaderInput {
	float4 position : POSITION0;
	
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTransmationMatrix.WVP);
	
	return output;
}
