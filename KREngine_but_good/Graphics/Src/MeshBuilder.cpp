#include "Precompiled.h"
#include "MeshBuilder.h"


void Graphics::MeshBuilder::GeneratePlane(Mesh& mesh, const float width, const float length, const float height)
{
	float halfLength = length * 0.5f;
	float halfWidth = width * 0.5f;
	mesh.mVertices = new Vertex[4];
	mesh.mVertexCount = 4;
	mesh.mVertices[0].position = Math::Vector3(-halfWidth, height, halfLength);
	mesh.mVertices[1].position = Math::Vector3(halfWidth, height, halfLength);
	mesh.mVertices[2].position = Math::Vector3(-halfWidth, height, -halfLength);
	mesh.mVertices[3].position = Math::Vector3(halfWidth, height, -halfLength);

	mesh.mIndices = new uint32_t[6];
	mesh.mIndexCount = 6;
	mesh.mIndices[0] = 0;
	mesh.mIndices[1] = 1;
	mesh.mIndices[2] = 2;

	mesh.mIndices[3] = 2;
	mesh.mIndices[4] = 1;
	mesh.mIndices[5] = 3;


	mesh.mVertices[0].uv = Math::Vector2(0.0f, 0.0f);
	mesh.mVertices[1].uv = Math::Vector2(0.0f, 1.0f);
	mesh.mVertices[2].uv = Math::Vector2(1.0f, 0.0f);
	mesh.mVertices[3].uv = Math::Vector2(1.0f, 1.0f);

	mesh.mVertices[0].normal = Math::Vector3(0.0f, 1.0f, 0.0f);
	mesh.mVertices[1].normal = Math::Vector3(0.0f, 1.0f, 0.0f);
	mesh.mVertices[2].normal = Math::Vector3(0.0f, 1.0f, 0.0f);
	mesh.mVertices[3].normal = Math::Vector3(0.0f, 1.0f, 0.0f);

	mesh.mVertices[0].tangent = Math::Vector3(1.0f, 0.0f, 0.0f);
	mesh.mVertices[1].tangent = Math::Vector3(1.0f, 0.0f, 0.0f);
	mesh.mVertices[2].tangent = Math::Vector3(1.0f, 0.0f, 0.0f);
	mesh.mVertices[3].tangent = Math::Vector3(1.0f, 0.0f, 0.0f);

}

void Graphics::MeshBuilder::GenerateStackSliceSphere(Mesh& mesh, const uint32_t maxStacks, const uint32_t maxSlices, const float radius)
{
	const uint32_t kNumVertices = (maxSlices + 1) * maxStacks;
	const uint32_t kNumIndices = maxSlices * (maxStacks - 1) * 6;
	const float kSliceOffset = Math::kTwoPi / maxSlices; //offset between slices
	const float kStackOffset = Math::kPi / (maxStacks - 1); // offset between stacks

	mesh.mVertexCount = kNumVertices;
	mesh.mVertices = new Vertex[mesh.mVertexCount];
	mesh.mIndexCount = kNumIndices;
	mesh.mIndices = new uint32_t[mesh.mIndexCount];

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

void Graphics::MeshBuilder::GenerateStackSliceCylinder(SkinnedMesh& mesh, const uint32_t maxStacks, const uint32_t maxSlices, const float radius, int numBones)
{
	ASSERT(numBones != 0, "[MeshBuilder] StackSliceCylinder requires at least one bone.");
	/*
	o--------o
	|		 |			 |
	|		 |			 |
	|		 |			 |
	o--------o			(B)
	|		 |			 |
	|		 |			 |
	|		 |			 |
	o--------o			(B)
	*/
	const uint32_t kNumVertices = (maxSlices + 1) * maxStacks;
	const uint32_t kNumIndices = maxSlices * (maxStacks - 1) * 6;
	const float kSliceOffset = Math::kTwoPi / maxSlices; // offset between slices
	const float kStackOffset = Math::kPi / (maxStacks - 1); // offset between stacks

	mesh.mVertexCount = kNumVertices;
	mesh.mVertices = new VertexBone[mesh.mVertexCount];
	mesh.mIndexCount = kNumIndices;
	mesh.mIndices = new uint32_t[mesh.mIndexCount];

	// Fill vertex data
	const float uStep = 1.0f / maxSlices;
	const float vStep = 1.0f / maxStacks;
	int currBoneIdx = 0;
	float currBonePercentForStack = 0.0f;
	uint32_t index = 0;

	// TODO: Account for edge case: only one bone.
	// TODO: Adjust weighting as stacks progress to next bone
	/*
	To assign each stack to a correct bone we need to store the percentage distance it is to the next bone. Once the current slice is further
	along its path than that percentage, increment the current bone.
	*/
	float nextBonePercent = static_cast<float>(currBoneIdx) / static_cast<float>(numBones);
	for (uint32_t stack = 0; stack < maxStacks; ++stack)
	{
		// check if the stacks have gone beyond the range of the current bone
		if ((static_cast<float>(stack) / static_cast<float>(maxStacks)) >= nextBonePercent)
		{
			// move to the next bone 
			++currBoneIdx;

			nextBonePercent = static_cast<float>(currBoneIdx) / static_cast<float>(numBones);
		}
		for (uint32_t slice = 0; slice <= maxSlices; ++slice)
		{
			const float theta = slice* kSliceOffset;
			const float y = stack;
			const float r = sqrt(1.0f - (y * y));
			const float s = sin(theta);
			const float c = cos(theta);
			const float x = r * c;
			const float z = r * s;

			mesh.mVertices[index].position = Math::Vector3(x, y, z);
			mesh.mVertices[index].normal = Math::Vector3(x, y, z);
			mesh.mVertices[index].tangent = Math::Vector3(-s, 0.0f, c);
			mesh.mVertices[index].uv = Math::Vector2(slice * uStep, stack * vStep);
			mesh.mVertices[index].boneIndex[0] = currBoneIdx;
			mesh.mVertices[index].boneIndex[1] = currBoneIdx + 1;
			mesh.mVertices[index].boneIndex[2] = 0;
			mesh.mVertices[index].boneIndex[3] = 0;
			mesh.mVertices[index].boneWeight[0] = 1.0f - currBonePercentForStack;
			mesh.mVertices[index].boneWeight[1] = currBonePercentForStack;
			mesh.mVertices[index].boneWeight[2] = 0.0f;
			mesh.mVertices[index].boneWeight[3] = 0.0f;
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

