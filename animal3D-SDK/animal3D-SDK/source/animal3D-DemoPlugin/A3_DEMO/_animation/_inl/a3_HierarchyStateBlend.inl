#include "..\a3_HierarchyStateBlend.h"
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
	
	a3_HierarchyStateBlend.inl
	Implementation of inline hierarchical blend operations.
*/


#ifdef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_INL
#define __ANIMAL3D_HIERARCHYSTATEBLEND_INL


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	pose_out->transform = a3mat4_identity;
	// ...

	// done
	return pose_out;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{

	// done
	return pose_out;
}


//-----------------------------------------------------------------------------

// data-based reset/identity
inline a3_SpatialPose a3spatialPoseDOpIdentity()
{
	a3_SpatialPose const result = { a3mat4_identity /*, ...*/ };
	return result;
}

// data-based LERP
inline a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out)
{

	// done
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u)
{

	// done
	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseSetConstruct(a3vec4 const angles, a3vec4 const scale, a3vec4 const translate)
{
	a3_SpatialPose* newPose;

	//do I need to initialize memory here potentially
	//newPose = memsize(sizeof(a3_SpatialPose)); //memsize is prob wrong

	newPose->angles = angles;
	newPose->scale = scale;
	newPose->translation = translate;

	return newPose;
}

inline a3_SpatialPose* a3SpatialPoseReturnCopy(a3_SpatialPose* const copyPose)
{
	a3_SpatialPose* copy;

	copy = copyPose;

	return copy;
}

inline a3_SpatialPose* a3SpatialPoseFindInverse(a3_SpatialPose* const invPose)
{
	a3_SpatialPose* inverse;

	//allocate new memory for inverse here

	//negation here or not?
	inverse->angles.x = -invPose->angles.x;
	inverse->angles.y = -invPose->angles.y;
	inverse->angles.z = -invPose->angles.z;

	inverse->scale.x = invPose->scale.x;
	inverse->scale.y = invPose->scale.y;
	inverse->scale.z = invPose->scale.z;

	return inverse;
}

inline a3_SpatialPose* a3SpatialPoseConcatenate()
{
	a3_SpatialPose* concat;



	return concat;
}

inline a3_SpatialPose* a3SpatialPoseNearest(a3real blendParam)
{
	a3_SpatialPose* nearest;

	return nearest;
}

inline a3_SpatialPose* a3SpatialPoseLerp(a3real blendParam)
{
	a3_SpatialPose* lerp;

	return lerp;
}

inline a3_SpatialPose* a3SpatialPoseCubic(a3real blendParam)
{
	a3_SpatialPose* cubic;

	return cubic;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H