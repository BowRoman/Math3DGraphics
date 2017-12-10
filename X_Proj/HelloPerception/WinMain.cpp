#include <Ai.h>
#include <XEngine.h>


class Bug : public Ai::Agent
{
public:
	Bug(Ai::World& world)
		: Ai::Agent(world)
		, mSteeringModule(nullptr)
		, mSeekBehavior(nullptr)
		, mHealth(0.0f)
	{}
	
	void Load()
	{
		mMaxSpeed = 250.0f;

		mSteeringModule = new Ai::SteeringModule(*this);
		mSeekBehavior = mSteeringModule->AddBehavior<Ai::SeekBehavior>();
	}

	void Unload()
	{
		mSteeringModule->Purge();
		X::SafeDelete(mSteeringModule);
	}

	void Update(float deltaTime)
	{
		if (!IsAlive())
			return;

		X::Math::Vector2 force = mSteeringModule->Calculate();
		X::Math::Vector2 accel = force / 1.0f;	// This should be mass
		mVelocity += accel * deltaTime;
		mPosition += mVelocity * deltaTime;

		if (X::Math::MagnitudeSqr(mVelocity) > 0.1f)
		{
			mHeading = X::Math::Normalize(mVelocity);
		}
	}

	void Draw()
	{
		if (!IsAlive())
			return;

		X::Math::Vector2 side = X::Math::PerpendicularLH(mHeading);
		X::Math::Vector2 a = mPosition + (mHeading * 16.0f);
		X::Math::Vector2 b = mPosition - (mHeading * 16.0f) + (side * 10.0f);
		X::Math::Vector2 c = mPosition - (mHeading * 16.0f) - (side * 10.0f);
		X::DrawScreenLine(a, b, X::Math::Vector4::Magenta());
		X::DrawScreenLine(a, c, X::Math::Vector4::Magenta());
		X::DrawScreenLine(b, c, X::Math::Vector4::Magenta());
	}

	void Spawn()
	{
		const float width = static_cast<float>(X::GetScreenWidth());
		const float height = static_cast<float>(X::GetScreenHeight());

		mMaxSpeed = 100.0f;
		mDestination = { width * 0.5f, height * 0.5f };
		float angle = X::RandomFloat(0.0f, X::Math::kTwoPi);
		X::Math::Vector2 direction(cos(angle), sin(angle));
		mPosition = mDestination + (direction * width * 0.5f);

		mSeekBehavior->SetActive(true);
		mHealth = X::RandomFloat(1.0f, 3.0f);
	}

	void Damage(float damage)
	{
		mHealth -= damage;
		if (mHealth <= 0.0f)
		{
			mSeekBehavior->SetActive(false);
		}
	}

	bool IsAlive() const
	{
		return mHealth > 0.0f;
	}

private:
	Ai::SteeringModule* mSteeringModule;
	Ai::SeekBehavior* mSeekBehavior;
	float mHealth;
};

struct Bullet
{
public:
	Bullet()
		: mBug(nullptr)
		, mActive(false)
	{}

	void Fire(Bug* bug)
	{
		mBug = bug;
		mPos = { (float)X::GetScreenWidth() * 0.5f, (float)X::GetScreenHeight() * 0.5f };
		mActive = true;
	}

	void Update(float deltaTime)
	{
		if (!mActive)
			return;

		auto toBug = mBug->Position() - mPos;
		auto mag = X::Math::Magnitude(toBug);
		if (mag < 10.0f)
		{
			mBug->Damage(1.0f);
			mActive = false;
		}
		else
		{
			mVel = toBug / mag * 500.0f;
			mPos += mVel * deltaTime;
		}
	}

	void Draw()
	{
		if (!mActive)
			return;

		X::DrawScreenLine(mPos, mPos - (X::Math::Normalize(mVel) * 10.0f), X::Math::Vector4::Red());
	}

private:
	Bug* mBug;
	X::Math::Vector2 mPos;
	X::Math::Vector2 mVel;
	bool mActive;
};

void Fire(Bug* bug);
void SpawnBug();

class Targeting : public Ai::Sensor
{
public:
	void Update(Ai::Agent& agent, Ai::MemoryRecords& memory, float deltaTime) override
	{
		auto neighborhood = agent.GetWorld().GetNeighborhood(X::Math::Circle(agent.Position(), 500.f));
		for (auto neighbor : neighborhood)
		{
			if (neighbor == &agent)
				continue;

			auto iter = std::find_if(memory.begin(), memory.end(), [neighbor](Ai::MemoryRecord* record) {
				return record->agent == neighbor;
			});

			if (!((Bug*)neighbor)->IsAlive())
			{
				if (iter != memory.end())
					memory.erase(iter);
				continue;
			}

			Ai::MemoryRecord* record = nullptr;
			if (iter == memory.end())
			{
				record = new Ai::MemoryRecord();
				memory.push_back(record);
			}
			else
			{
				record = *iter;
			}
			record->agent = neighbor;
			record->lastKnownPosition = neighbor->Position();
			record->lastRecordedTime = X::TimeSeconds();
		}
	}

	const char* GetName() const override
	{
		return "Targeting";
	}
};

class Tower : public Ai::Agent
{
public:
	Tower(Ai::World& world)
		: Ai::Agent(world)
		, mBug(nullptr)
		, mPerceptionModule(nullptr)
		, mCoolDown(0.0f)
	{
	}

	void Load()
	{
		mPosition = { (float)X::GetScreenWidth() * 0.5f, (float)X::GetScreenHeight() * 0.5f };
		mPerceptionModule = new Ai::PerceptionModule(*this);
		mPerceptionModule->AddSensor<Targeting>();
	}

	void Unload()
	{
		mPerceptionModule->Purge();
		X::SafeDelete(mPerceptionModule);
	}

	void Update(float deltaTime)
	{
		if (mBug != nullptr && !mBug->IsAlive())
			mBug = nullptr;

		if (mBug)
		{
			mCoolDown -= deltaTime;
			if (mCoolDown <= 0.0f)
			{
				mCoolDown = 0.1f;
				Fire(mBug);
			}
		}
		else
		{
			mPerceptionModule->Update(deltaTime, [this](Ai::MemoryRecord& record)
			{
				record.importance = 1.0f / X::Math::Distance(mPosition, record.lastKnownPosition);
			});

			auto& records = mPerceptionModule->GetMemoryRecords();
			if (!records.empty())
			{
				mBug = static_cast<Bug*>(records.front()->agent);
			}
		}
	}

	void Draw()
	{
		X::DrawScreenCircle(mPosition, 32.0f, X::Math::Vector4::Green());
	}

private:
	Bug* mBug;
	Ai::PerceptionModule* mPerceptionModule;
	float mCoolDown;
};

Ai::World AiWorld;
Tower* tower = nullptr;
std::vector<Bug*> bugs;
size_t nextBug = 0;
std::vector<Bullet*> bullets;
size_t nextBullet = 0;
float cooldown;

void Fire(Bug* bug)
{
	bullets[nextBullet]->Fire(bug);
	nextBullet = (nextBullet + 1) % bullets.size();
}

void SpawnBug()
{
	bugs[nextBug]->Spawn();
	nextBug = (nextBug + 1) % bugs.size();
}

bool GameLoop(float deltaTime)
{
	cooldown -= deltaTime;
	if (cooldown <= 0.0f)
	{
		cooldown = X::RandomFloat(0.3f, 0.75f);
		SpawnBug();
	}

	tower->Update(deltaTime);
	tower->Draw();

	for (auto b : bugs)
	{
		b->Update(deltaTime);
		b->Draw();
	}
	for (auto b : bullets)
	{
		b->Update(deltaTime);
		b->Draw();
	}

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");

	tower = new Tower(AiWorld);
	tower->Load();
	for (size_t i = 0; i < 500; ++i)
	{
		auto bug = new Bug(AiWorld);
		bug->Load();
		bugs.push_back(bug);
	}
	for (size_t i = 0; i < 500; ++i)
	{
		auto bullet = new Bullet();
		bullets.push_back(bullet);
	}

	X::Run(GameLoop);

	for (size_t i = 0; i < 500; ++i)
	{
		bugs[i]->Unload();
		X::SafeDelete(bugs[i]);
	}
	for (size_t i = 0; i < 500; ++i)
	{
		X::SafeDelete(bullets[i]);
	}
	tower->Unload();
	X::SafeDelete(tower);
	X::Stop();
	return 0;
}