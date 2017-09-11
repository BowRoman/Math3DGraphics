#pragma once



#define CLASS_ID(ClassType, FOURCC)\
static void* CreateInstance() {return new ClassType();}\
static const uint32_t ClassId = FourCC;\
virtual uint32_t GetClassId() const{return ClassId;}

namespace Network
{
	class ObjectFactory
	{
		std::unordered_map<uint32_t, std::function<void*()>> mCreators;
	public:
		template<class T>
		void Register()
		{
			mCreators[T::ClassId] = T::CreateInstance;
		}

		void* CreateInstance(uint32_t classId)
		{
			ASSERT(mCreators.find(classId) != mCreators.end(), "[ObjectFactory] Class Id %d not registered", classId);
			return mCreators[classId]();
		}
	};
}