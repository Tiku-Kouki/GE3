#include "Sprite.hlsli"



struct VertexShaderInput {
	float4 position : POSITION0;
	

};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = input.position;
	
	return output;
}
