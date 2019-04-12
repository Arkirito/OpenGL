#include "WorldObject.h"

#include "GlobalInstance.h"
#include "World.h"

WorldObject::WorldObject()
{
	GlobalInstance::GetInstance()->GetWorld()->RegisterObject(this);
}


WorldObject::~WorldObject()
{
	GlobalInstance::GetInstance()->GetWorld()->UnregisterObject(this);
}