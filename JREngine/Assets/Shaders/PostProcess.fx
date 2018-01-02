//====================================================================================================
// Filename:	PostProcess.fx
// Created by:	Kyle
// Description: PostProcess
//====================================================================================================

cbuffer Constantbuffer : register(b0)
{
	matrix wvp;
}


//====================================================================================================
// Vertex Shader
//====================================================================================================
struct VSInput
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};


VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = input.position;
	output.texcoord = input.texcoord;
	return output;
}

Texture2D RenderTargetTexture : register(t0);
SamplerState Sampler : register(s0);

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VSOutput input) : SV_Target
{
	return RenderTargetTexture.Sample(Sampler, input.texcoord);
}
