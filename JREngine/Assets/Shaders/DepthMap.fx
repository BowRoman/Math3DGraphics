//====================================================================================================
// Filename:	PostProcess.fx
// Created by:	Kyle
// Description: PostProcess
//====================================================================================================

cbuffer Constantbuffer : register(b0)
{
	matrix wvp;
	float displacementScale;
}

Texture2D DisplacementMap : register(t0);
SamplerState Sampler : register(s0);

//====================================================================================================
// Vertex Shader
//====================================================================================================
struct VSInput
{
	float4 position	 : POSITION;
	float3 normal	 : NORMAL;
	float3 tangent	 : TANGENT;
	float4 color	 : COLOR;
	float2 texcoord	 : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
};


VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	float DisplacementTexture = DisplacementMap.SampleLevel(Sampler, input.texcoord, 0.0f).x;
	float4 DisplacementPosition = input.position + (float4((input.normal * DisplacementTexture), 0.0f) * displacementScale);
	float4 posProj = mul(input.position, wvp);

	output.position = posProj;
	return output;
}


//====================================================================================================
// Pixel Shader
//====================================================================================================

//float4 PS(VSOutput input) : SV_Target
//{
//	return input;
//}
