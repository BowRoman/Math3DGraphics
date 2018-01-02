#include "Precompiled.h"
#include "MeshBuilder.h"

#include "ConstantBuffer.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace Graphics;

void MeshBuilder::CreatePlane(Mesh & mesh, uint32_t rows, uint32_t columns, float scale)
{
}

void MeshBuilder::CreateSphere(Mesh& mesh, uint32_t slices, uint32_t rings)
{
}

void Graphics::MeshBuilder::GenerateStackSliceSphere(Mesh & mesh, const uint32_t maxStacks, const uint32_t maxSlices, const float radius)
{

	const uint32_t kNumVertices = (maxSlices + 1) * maxStacks;
	const uint32_t kNumIndices = maxSlices * (maxStacks - 1) * 6;
	const float kSliceOffset = Math::kTwoPi / maxSlices; //offset between slices
	const float kStackOffset = Math::kPi / (maxStacks - 1); // offset between stacks

	mesh.mNumVertices = kNumVertices;
	mesh.mVertices = new Vertex[mesh.mNumVertices];
	mesh.mNumIndices = kNumIndices;
	mesh.mIndices = new uint32_t[mesh.mNumIndices];

	//Fill vertex data
	float uStep = 1.0f / maxSlices;
	float vStep = 1.0f / maxStacks;
	uint32_t index = 0;

	for (uint32_t stack = 0; stack < maxStacks; ++stack)
	{
		const float phi = stack * kStackOffset;
		for (uint32_t slice = 0; slice <= maxSlices; ++slice)
		{
			const float theta = slice* kSliceOffset;
			const float y = cos(phi);
			const float r = sqrt(1.0f - (y * y));
			const float s = sin(theta);
			const float c = cos(theta);
			const float x = r * c;
			const float z = r * s;

			mesh.mVertices[index].position = Math::Vector3(x, y, z);
			mesh.mVertices[index].normal = Math::Vector3(x, y, z);
			mesh.mVertices[index].tangent = Math::Vector3(-s, 0.0f, c);
			mesh.mVertices[index].uv = Math::Vector2(slice * uStep, stack * vStep);
			++index;
		}
	}
	//index data
	index = 0;
	for (uint32_t j = 0; j < maxStacks - 1; ++j)
	{
		for (uint32_t i = 0; i < maxSlices; ++i)
		{
			const uint32_t a = i % (maxSlices + 1);
			const uint32_t b = (i + 1) % (maxSlices + 1);
			const uint32_t c = j * (maxSlices + 1);
			const uint32_t d = (j + 1) * (maxSlices + 1);

			mesh.mIndices[index++] = a + c;
			mesh.mIndices[index++] = b + c;
			mesh.mIndices[index++] = a + d;
			mesh.mIndices[index++] = b + c;
			mesh.mIndices[index++] = b + d;
			mesh.mIndices[index++] = a + d;
		}
	}
}
