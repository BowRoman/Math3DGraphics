//====================================================================================================
// Filename:	SkyBox.fx
// Created by:	Kyle Roblin
// Description: Creates SkyBox
//====================================================================================================

cbuffer Constantbuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    float3 cameraPos;
}

//====================================================================================================
// Vertex Shader
//====================================================================================================
struct VSInput
{
	float4 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD0;
};


TextureCube SkyBoxCube : register(t0);
sampler cubeSampler
{
    texture = SkyBoxCube;
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    float4 worldPosition = mul(input.position, world);
    float4 viewPosition = mul(worldPosition, view);
    output.position = mul(viewPosition, projection);

    float4 vertexPosition = mul(input.position, world);
    output.texCoord = vertexPosition.xyz - cameraPos;
	return output;
}


//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS( VSOutput input ) : SV_Target
{
	return texCUBE(cubeSampler, normalize(input.texCoord));
}
