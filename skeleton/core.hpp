#pragma once
#ifndef __CORE_HPP__
#define __CORE_HPP__

#define PI 3.141592653589793
enum Solid_Type { STATIC, DYNAMIC };
enum Shape_Type { SPHERE, BOX, BOX_PIECE, LONG_PIECE, GROUND, PLATFORM, PLANE };

#include "../common/Common/Print.h"
#include "../common/Common/PVD.h"
#include "../common/Utils/Utils.h"

#include "Render/Render.h"
#include "Render/Camera.h"

#include <vector>

using namespace Snippets;

typedef physx::PxVec3 Vector3;
typedef physx::PxVec4 Vector4;


#endif