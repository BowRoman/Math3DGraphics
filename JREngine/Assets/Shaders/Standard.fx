//====================================================================================================
// Filename:	Standard.fx
// Created by:	Kyle
// Description: Texturing
//====================================================================================================

cbuffer Constantbuffer : register(b0)
{
	matrix wvp;
	matrix world;
	float3 cameraPosition;
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float  power;
    float  displacementScale;
}



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
	float3 worldpos : TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float3 tangent	: TEXCOORD2;
	float2 texcoord : TEXCOORD3;
};



Texture2D DiffuseMap : register(t0);
Texture2D SpecularMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D DisplacementMap : register(t3);
SamplerState Sampler : register(s0);

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
    float DisplacementTexture = DisplacementMap.SampleLevel(Sampler, input.texcoord, 0.0f).x;
    float4 DisplacementPosition = input.position + (float4((input.normal * DisplacementTexture), 0.0f) * displacementScale);

    output.position = mul(DisplacementPosition, wvp);
	output.worldpos = mul(input.position, world).xyz;
	output.normal = mul(float4(input.normal, 0.0f), world).xyz;
    output.tangent = mul(float4(input.tangent, 0.0f), world).xyz;
	output.texcoord = input.texcoord;
	return output;
}
//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS( VSOutput input ) : SV_Target
{
    float3 normalTexture = NormalMap.Sample(Sampler, input.texcoord);
    normalTexture = normalize((normalTexture * 2.0f) - 1.0f);
	float3 normal = normalize(input.normal);
    float3 tangent = normalize(input.tangent);
    float3 biNormal = normalize(cross(normal, tangent));
    
	float3x3 TBN = {tangent, biNormal, normal};
    normal = mul(normalTexture, TBN);

	float3 dirToLight = -normalize(lightDirection);
    //return dirToLight.xyzz;

	float3 dirToCamera = normalize(cameraPosition - input.worldpos);


	//ambient
	float4 ambientColor = lightAmbient * materialAmbient;


	//Diffuse
	float diffuse = saturate(dot(dirToLight, normal));
	float4 diffuseColor = diffuse * lightDiffuse * materialDiffuse;

	//Specular
	float3 halfVector = normalize((dirToLight + dirToCamera) * 0.5f);
	float specular = saturate(dot(halfVector, normal));
	float4 specularColor = pow(specular, power) * lightSpecular * materialSpecular;

	//texture values
	float4 diffuseTexture = DiffuseMap.Sample(Sampler, input.texcoord);
    float specularTexture = SpecularMap.Sample(Sampler, input.texcoord).x;

    return (ambientColor + diffuseColor) * diffuseTexture + (specularTexture * specularColor);
}
