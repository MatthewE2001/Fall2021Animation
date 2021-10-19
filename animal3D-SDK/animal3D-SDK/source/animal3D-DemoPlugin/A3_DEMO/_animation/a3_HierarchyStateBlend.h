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

#include <stdio.h>
#include <math.h>


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
	if (u <= 0.0)
	{
		return v0;
	}

	// if blend parameter is 1 (or greater), return the terminal a3vec4
	if (u >= 1.0)
	{
		return v1;
	}

	// if blend parameter is greater than 0 and less than one, return a linear blend of the a3vec4
	a3vec4 out = { 0.0, 0.0, 0.0, 0.0 };
	out.x = (((a3real)1.0 - u) * v0.x) + (u * v1.x);
	out.y = (((a3real)1.0 - u) * v0.y) + (u * v1.y);
	out.z = (((a3real)1.0 - u) * v0.z) + (u * v1.z);
	out.w = (((a3real)1.0 - u) * v0.w) + (u * v1.w);

	return out;
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
	a3vec4 out = { 0.0, 0.0, 0.0, 0.0 };
	out.x = v0.x * (a3real)pow(((a3real)pow(v0.x, (a3real)-1.0) * v1.x), u);
	out.y = v0.y * (a3real)pow(((a3real)pow(v0.y, (a3real)-1.0) * v1.y), u);
	out.z = v0.z * (a3real)pow(((a3real)pow(v0.z, (a3real)-1.0) * v1.z), u);
	out.w = v0.w * (a3real)pow(((a3real)pow(v0.w, (a3real)-1.0) * v1.w), u);

	
	return out;
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

//possibly remove the asterisk from the function type, or just add in return poses everywhere
a3_SpatialPose* a3SpatialPoseSetConstruct(a3vec4 const angles, a3vec4 const scale, a3vec4 const translate);

a3_SpatialPose* a3SpatialPoseReturnCopy(a3_SpatialPose* const copyPose);

a3_SpatialPose* a3SpatialPoseFindInverse(a3_SpatialPose* const invPose);

a3_SpatialPose* a3SpatialPoseConcatenate(a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh);

a3_SpatialPose* a3SpatialPoseNearest(a3_SpatialPose* const p0, a3_SpatialPose* const p1, a3real blendParam);

a3_SpatialPose* a3SpatialPoseBlendLerp(a3_SpatialPose* const p0, a3_SpatialPose* const p1, a3real blendParam);

a3_SpatialPose* a3SpatialPoseCubic(a3_SpatialPose* const prevPose, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3_SpatialPose* const poseNext, a3real blendParam);

a3_SpatialPose* a3SpatialPoseDeConcat(a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh);

a3_SpatialPose* a3SpatialPoseCalcScale(a3real blendParam, a3_SpatialPose* const poseIn);

a3_SpatialPose* a3SpatialPoseTriangular(a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3_SpatialPose* const pose2, a3real scaleParam1, a3real scaleParam2);

a3_SpatialPose* a3SpatialPoseBiNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const initPose0, a3_SpatialPose* const initPose1,
	a3_SpatialPose* const termPose0, a3_SpatialPose* const termPose1, a3real blendParam1, a3real blendParam2, a3real blendParam3);

a3_SpatialPose* a3SpatialPoseBiLerp(a3_SpatialPose* pose_out, a3_SpatialPose* const initPose0, a3_SpatialPose* const initPose1,
	a3_SpatialPose* const termPose0, a3_SpatialPose* const termPose1, a3real blendParam1, a3real blendParam2, a3real blendParam3);

a3_SpatialPose* a3SpatialPoseBiCubic(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3_SpatialPose* const pose2,
	a3_SpatialPose* const pose3, a3_SpatialPose* const pose4, a3_SpatialPose* const pose5, a3_SpatialPose* const pose6, 
	a3_SpatialPose* const pose7, a3_SpatialPose* const pose8, a3_SpatialPose* const pose9, a3_SpatialPose* const pose10, a3_SpatialPose* const pose11,
	a3_SpatialPose* const pose12, a3_SpatialPose* const pose13, a3_SpatialPose* const pose14, a3_SpatialPose* const pose15,
	a3real blendParam1, a3real blendParam2, a3real blendParam3, a3real blendParam4, a3real blendParam5);

#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H