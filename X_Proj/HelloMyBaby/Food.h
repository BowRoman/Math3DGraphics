#pragma once

#include <XEngine.h>

class FoodContainer
{
public:
	struct Food
	{
	public:
		X::Math::Vector2 mPosition;
		X::TextureId mTextureId;
		bool active;
		Food(char* textureFilename = "mushroom.png")
			: active(false)
			, mPosition{0.0f,0.0f}
		{
			mTextureId = X::LoadTexture(textureFilename);
		}
	private:
		void Draw()
		{
			X::DrawSprite(mTextureId, mPosition - X::Math::Vector2(16.0f, 16.0));
		}

		friend class FoodContainer;
	};

private:
	std::vector<Food*> FoodVec;


	//static FoodContainer* sFoodContainer;

public:
	FoodContainer(uint32_t size = 50)
	{
		FoodVec.resize(size);
		for (int i = 0; i < size; ++i)
		{
			FoodVec[i] = new Food();
		}
	}
	~FoodContainer()
	{
		for (auto f : FoodVec)
		{
			X::SafeDelete(f);
		}
	}
	/*static void StaticInitialize(uint32_t size = 50)
	{
		sFoodContainer->FoodVec.resize(size);
	}
	static void Staticterminate()
	{
		for (auto f : sFoodContainer->FoodVec)
		{
			X::SafeDelete(f);
		}
	}
	static FoodContainer* Get()
	{
		if (!sFoodContainer)
		{
			sFoodContainer = new FoodContainer();
		}
		return sFoodContainer;
	}*/
	inline Food* const operator[](int i)
	{
		return FoodVec[i]->active ? FoodVec[i] : nullptr;
	}
	Food* At(int i)
	{
		if (i >= 0 && i < FoodVec.size())
		{
			return FoodVec[i];
		}
		else
			return nullptr;
	}
	int GetFood()
	{
		for (int i = 0; i < FoodVec.size(); ++i)
		{
			if (!FoodVec[i]->active)
			{
				FoodVec[i]->active = true;
				return i;
			}
		}
		return -1;
	}
	std::vector<int> GetAllActiveFood()
	{
		std::vector<int> idxs;
		for (int i = 0; i < FoodVec.size(); ++i)
		{
			if (FoodVec[i]->active)
			{
				idxs.push_back(i);
			}
		}
		return idxs;
	}
	void Draw()
	{
		for (auto f : FoodVec)
		{
			if (f->active)
			{
				f->Draw();
			}
		}
	}
}; // class FoodContainer