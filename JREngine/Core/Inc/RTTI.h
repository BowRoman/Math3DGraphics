#pragma once

#define REGISTER_TYPE(TypeId)\
	static int StaticGetType()	{ return #@TypeId; }\
	virtual int GetType() const { return StaticGetType(); }
