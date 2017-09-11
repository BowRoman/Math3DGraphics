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
    VSOutput output = (VSOutput) 0;
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
    float DeltaU = 1.0f / 1024;
    float DeltaV = 1.0f / 768;
    float2 offset[9] =
    {
        float2(-DeltaU, -DeltaV),
      float2(0.0f, -DeltaV),
      float2(DeltaU, -DeltaV),
      float2(-DeltaU, 0.0f),
      float2(0.0f, 0.0f),
      float2(-DeltaU, 0.0f),
      float2(-DeltaU, DeltaV),
      float2(0.0f, DeltaV),
      float2(-DeltaU, DeltaV),
    };
    float kernalX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    float kernalY[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

    float3 valX = (float3) 0;
    float3 valY = (float3) 0;
    for (int i = 0; i < 9; ++i)
    {
        float4 texColor = RenderTargetTexture.Sample(Sampler, input.texcoord + offset[i]);
        valX += kernalX[i] = texColor.xyz;
        valY += kernalY[i] = texColor.xyz;
    }
    float4 color = RenderTargetTexture.Sample(Sampler, input.texcoord);;
    color *= (length((valX * valX) + (valY * valY)) > 0.3f) ? 0.0f : 1.0f;

    return color;
}
