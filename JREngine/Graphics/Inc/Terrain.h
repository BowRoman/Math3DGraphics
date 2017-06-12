#ifndef INCLUDED_GRAPHICS_TERRAIN_H
#define INCLUDED_GRAPHICS_TERRAIN_H

namespace Graphics
{

class Terrain
{
public:

	Terrain();
	~Terrain();

	void Initialize(char* rawFile, char* terrainTexture);
	void Terminate();

	void Render();

private:
	MeshBuffer mMeshBuffer;
	Texture mTexture;
	Sampler mSampler;

	UCHAR* mHeight;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_TERRAIN_H