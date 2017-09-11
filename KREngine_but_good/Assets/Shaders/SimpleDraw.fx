//====================================================================================================
// Filename:	SimpleDraw.fx
// Created by:	Kyle
// Description: Simple Draw
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
	float4 color : COLOR;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(input.position, wvp);
	output.color = input.color;
	return output;
}


//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS( VSOutput input ) : SV_Target
{
	return input.color;
}
