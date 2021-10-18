/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_HierarchyStateBlend.h
	Hierarchy blend operations.
*/

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H


#include "a3_HierarchyState.h"

#include "a3_Kinematics.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	
//use this file I assume to do a lot of blend stuff
	//Writing a lot of functions

//blend operation function pointer
typedef a3vec4(*a3_BlendOpLerp)(a3vec4 const v0, a3vec4 const v1, a3real const u);

typedef struct a3_SpatialPoseBlendOpLerp
{
	a3_BlendOpLerp opOrientation, opAngles, opScale, opTranslation;
} a3_SpatialPoseBlendOpLerp;

//Possibly could have been done elsewhere but here works
	//probably would just move to hierarchy state blend.inl if it does move

// Linear Interpolation
// Formats: a3vec4Lerp(v0, v1, u)
// Return: linear blend between values
// Controls (2): initial (v0) and terminal (v1) a3vec4s
// Inputs (1): blend parameter (u)
inline a3vec4 a3vec4Lerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	// if blend parameter is 0 (or less), return the initial a3vec4
	if (u <= 0.0):
		return v0;

	// if blend parameter is 1 (or greater), return the terminal a3vec4
	if (u >= 1.0) :
		return v1;

	// if blend parameter is greater than 0 and less than one, return a linear blend of the a3vec4
	return [((1.0 - u) * v0.x) + (t * v1.x),
			((1.0 - u) * v0.y) + (t * v1.y),
			((1.0 - u) * v0.z) + (t * v1.z),
			((1.0 - u) * v0.w) + (t * v1.w)]
}

// Logarithmic Linear Interpolation
// Formats: a3vec4LogLerp(v0, v1, u)
// Return: logarithmic linear blend between values
// Controls (2): initial (v0) and terminal (v1) a3vec4s
// Inputs (1): blend parameter (u)
inline a3vec4 a3vec4LogLerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	//implement logarithmic interpolation

	return v0;
}

// Spherical Linear Interpolation
// Formats: a3vec4Slerp(v0, v1, u)
// Return: spherical linear blend between values
// Controls (2): initial (v0) and terminal (v1) a3vec4s
// Inputs (1): blend parameter (u)
inline a3vec4 a3vec4Slerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	//implement spherical linear interpolation
	
	
	return v0;
}

// Normalized Linear Interpolation
// Formats: a3vec4NLerp(v0, v1, u)
// Return: normalized linear blend between values
// Controls (2): initial (v0) and terminal (v1) a3vec4s
// Inputs (1): blend parameter (u)
inline a3vec4 a3vec4Nlerp(a3vec4 const v0, a3vec4 const v1, a3real const u)
{
	//implement normalized linear interpolation

	return v0;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------

// data-based reset/identity
a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out);

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------

typedef struct a3_SpatialPoseConstruct
{
	a3vec4 constructAngles, constructScale, constructTranslate;
} a3_SpatialPoseConstruct;

a3_SpatialPose* a3SpatialPoseSetConstruct();

struct a3_SpatialPoseCopy //should this be typedef struct
{
	a3_SpatialPose* copyPose;
}; //a3_SpatialPoseCopy;

a3_SpatialPose* a3SpatialPoseReturnCopy();

//do I need to make a struct for inverse here as well

a3_SpatialPose* a3SpatialPoseFindInverse();

a3_SpatialPose* a3SpatialPoseConcatenate();

a3_SpatialPose* a3SpatialPoseNearest(a3real blendParam);

a3_SpatialPose* a3SpatialPoseLerp(a3real blendParam);

a3_SpatialPose* a3SpatialPoseCubic(a3real blendParam);

#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H