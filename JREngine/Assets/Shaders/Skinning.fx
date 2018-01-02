//====================================================================================================
// Filename:	SimpleDraw.fx
// Created by:	Jake Roman
// Description: Shader which performs hardware skinning
//====================================================================================================

cbuffer Constantbuffer : register(b0)
{
	matrix wvp;
	//matrix world;
}

cbuffer BoneConstantBuffer : register(b1)
{
	matrix boneTransforms[64];
}

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

//====================================================================================================
// Helpers
//====================================================================================================
matrix GetBoneTransforms(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f)
	{
		return Identity;
	}

	matrix transform;
	transform = boneTransforms[indices[0] * weights[0]];
	transform += boneTransforms[indices[1] * weights[1]];
	transform += boneTransforms[indices[2] * weights[2]];
	transform += boneTransforms[indices[3] * weights[3]];
	return transform;

};
//====================================================================================================
// Vertex Shader
//====================================================================================================
struct VSInput
{
	float4 position : POSITION;
	float4 color : COLOR;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeight : BLENDWEIGHT;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	//float4 worldPos : TEXCOORD0;
	float4 color : COLOR;
};


VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;

	matrix boneTransform = GetBoneTransforms(input.blendIndices, input.blendWeight);

	float4 posBone = input.position;
	float4 posLocal = mul(posBone, boneTransform);
	//float4 posWorld = mul(posLocal, world);
	float4 posProj = mul(posLocal, wvp);

	//output.worldPos = posWorld;
	output.position = posProj;
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
