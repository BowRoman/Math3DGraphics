//====================================================================================================
// Filename:	DoNothing.fx
// Created by:	Peter Chan
// Description: Basic shader for colour
//====================================================================================================

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS( VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = input.Pos;
	output.Color = input.Color;
	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS( VS_OUTPUT input ) : SV_Target
{
	return input.Color;
}
