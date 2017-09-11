//====================================================================================================
// Filename:	PostProcess.fx
// Created by:	Kyle
// Description: PostProcess
//====================================================================================================

cbuffer Constantbuffer : register(b0)
{
    float time;
}


//====================================================================================================
// Vertex Shader
//====================================================================================================
struct VSInput
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD1;
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
    float2 screenSize = float2(1024, 768); // screen resolution hard code  
	float2 center = screenSize * 0.5f;
    float radius = 10.0f; //radius of circle
    float distance = length(input.position.xy - center); //distance in screen space
    float multiplier = 5.0f; //distortion multiplier

    float percent = abs(distance - radius) / radius;
    float theta = percent * percent * multiplier; //angle for the sin and cos waves
    float s = sin(theta); //sin wave angle
    float c = cos(theta); //cos wave angle
    float2 offset = input.position.xy - center; //offset from center
    offset = float2(dot(offset, float2(c, -s)), dot(offset, float2(s, c)));
    offset += center;


    float3 newColor = RenderTargetTexture.Sample(Sampler, offset / screenSize).rgb;
	//newColor = newColor* tan(newColor * 25.0f) * 2.0f;
	return float4(newColor, 1.0f);
}