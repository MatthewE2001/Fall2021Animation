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
	pose_out->pose->translation = a3vec4Lerp(pose0->pose->translation, pose1->pose->translation, u);
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

	inverse->translation.x = -invPose->translation.x;
	inverse->translation.y = -invPose->translation.y;
	inverse->translation.z = -invPose->translation.z;

	return inverse;
}

inline a3_SpatialPose* a3SpatialPoseConcatenate(a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh)
{
	a3_SpatialPose* concat;

	//a3spatialPoseOpIdentity(concat);

	//initialize concat memory here
	//also could recreate the concatenation step here

	a3spatialPoseConcat(concat, pose_lh, pose_rh);

	return concat;
}

inline a3_SpatialPose* a3SpatialPoseNearest(a3_SpatialPose* const p0, a3_SpatialPose* const p1, a3real blendParam)
{
	a3_SpatialPose* nearest;

	if (blendParam < 0.5)
	{
		nearest = p0;
	}
	else if (blendParam >= 0.5)
	{
		nearest = p1;
	}

	return nearest;
}

inline a3_SpatialPose* a3SpatialPoseBlendLerp(a3_SpatialPose* const p0, a3_SpatialPose* const p1, a3real blendParam)
{
	a3_SpatialPose* lerp;

	//a3spatialPoseReset(lerp);
	//initialize lerp pose memory

	a3spatialPoseLerp(lerp, p0, p1, blendParam);

	return lerp;
}

inline a3_SpatialPose* a3SpatialPoseCubic(a3_SpatialPose* const prevPose, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, 
	a3_SpatialPose* const poseNext, a3real blendParam)
{
	a3_SpatialPose* cubic;

	//give cubic memory size

	//catmull rom interpolation
	cubic->translation.x = (a3real)0.5 * ((-blendParam + 2 * (blendParam * blendParam) - (blendParam * blendParam * blendParam)) * prevPose->translation.x
		+ (2 - 5 * (blendParam * blendParam) + 3 * (blendParam * blendParam * blendParam)) * pose0->translation.x
		+ (blendParam + 4 * (blendParam * blendParam) - 3 * (blendParam * blendParam * blendParam)) * pose1->translation.x
		+ (-(blendParam * blendParam) + (blendParam * blendParam * blendParam))) * poseNext->translation.x;

	cubic->translation.y = (a3real)0.5 * ((-blendParam + 2 * (blendParam * blendParam) - (blendParam * blendParam * blendParam)) * prevPose->translation.y
		+ (2 - 5 * (blendParam * blendParam) + 3 * (blendParam * blendParam * blendParam)) * pose0->translation.y
		+ (blendParam + 4 * (blendParam * blendParam) - 3 * (blendParam * blendParam * blendParam)) * pose1->translation.y
		+ (-(blendParam * blendParam) + (blendParam * blendParam * blendParam))) * poseNext->translation.y;

	cubic->translation.z = (a3real)0.5 * ((-blendParam + 2 * (blendParam * blendParam) - (blendParam * blendParam * blendParam)) * prevPose->translation.z
		+ (2 - 5 * (blendParam * blendParam) + 3 * (blendParam * blendParam * blendParam)) * pose0->translation.z
		+ (blendParam + 4 * (blendParam * blendParam) - 3 * (blendParam * blendParam * blendParam)) * pose1->translation.z
		+ (-(blendParam * blendParam) + (blendParam * blendParam * blendParam))) * poseNext->translation.z;

	return cubic;
}

inline a3_SpatialPose* a3SpatialPoseDeConcat(a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh)
{
	a3_SpatialPose* deconcat;

	deconcat->translation.x = pose_lh->translation.x - pose_rh->translation.x;
	deconcat->translation.y = pose_lh->translation.y - pose_rh->translation.y;
	deconcat->translation.z = pose_lh->translation.z - pose_rh->translation.z;

	deconcat->scale.x = pose_lh->scale.x / pose_rh->scale.x;
	deconcat->scale.y = pose_lh->scale.y / pose_rh->scale.y;
	deconcat->scale.z = pose_lh->scale.z / pose_rh->scale.z;

	deconcat->angles.x = a3trigValid_sind(pose_lh->angles.x - pose_rh->angles.x);
	deconcat->angles.y = a3trigValid_sind(pose_lh->angles.y - pose_rh->angles.y);
	deconcat->angles.z = a3trigValid_sind(pose_lh->angles.z - pose_rh->angles.z);

	return deconcat;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H