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

	pose_out->translation = a3vec4Lerp(pose0->translation, pose1->translation, u);
	pose_out->angles = a3vec4Lerp(pose0->angles, pose1->angles, u);
	pose_out->scale = a3vec4Lerp(pose0->scale, pose1->scale, u);

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
	// set pose-out to the identity matrix
	pose_out->pose->transform = a3mat4_identity;
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u)
{
	pose_out->pose->translation = a3vec4Lerp(pose0->pose->translation, pose1->pose->translation, u);
	// done
	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseSetConstruct(a3_SpatialPose* pose_out, a3vec4 const angles, a3vec4 const scale, a3vec4 const translate)
{
	pose_out->angles = angles;
	pose_out->scale = scale;
	pose_out->translation = translate;

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseReturnCopy(a3_SpatialPose* pose_out, a3_SpatialPose* const copyPose)
{
	pose_out = copyPose;

	return pose_out; //every function should have return pose_out
}

inline a3_SpatialPose* a3SpatialPoseFindInverse(a3_SpatialPose* pose_out, a3_SpatialPose* const invPose)
{
	//negation here or not?
	pose_out->angles.x = -invPose->angles.x;
	pose_out->angles.y = -invPose->angles.y;
	pose_out->angles.z = -invPose->angles.z;

	pose_out->scale.x = invPose->scale.x;
	pose_out->scale.y = invPose->scale.y;
	pose_out->scale.z = invPose->scale.z;

	pose_out->translation.x = -invPose->translation.x;
	pose_out->translation.y = -invPose->translation.y;
	pose_out->translation.z = -invPose->translation.z;

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseConcatenate(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh)
{
	a3spatialPoseConcat(pose_out, pose_lh, pose_rh);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const p0, a3_SpatialPose* const p1, a3real blendParam)
{
	if (blendParam < 0.5)
	{
		pose_out = p0;
	}
	else if (blendParam >= 0.5)
	{
		pose_out = p1;
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseBlendLerp(a3_SpatialPose* pose_out, a3_SpatialPose* const p0, a3_SpatialPose* const p1, a3real blendParam)
{
	a3spatialPoseLerp(pose_out, p0, p1, blendParam);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseCubic(a3_SpatialPose* pose_out, a3_SpatialPose* const prevPose, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1,
	a3_SpatialPose* const poseNext, a3real blendParam)
{
	//give cubic memory size

	//catmull rom interpolation
	pose_out->translation.x = (a3real)0.5 * ((-blendParam + 2 * (blendParam * blendParam) - (blendParam * blendParam * blendParam)) * prevPose->translation.x
		+ (2 - 5 * (blendParam * blendParam) + 3 * (blendParam * blendParam * blendParam)) * pose0->translation.x
		+ (blendParam + 4 * (blendParam * blendParam) - 3 * (blendParam * blendParam * blendParam)) * pose1->translation.x
		+ (-(blendParam * blendParam) + (blendParam * blendParam * blendParam))) * poseNext->translation.x;

	pose_out->translation.y = (a3real)0.5 * ((-blendParam + 2 * (blendParam * blendParam) - (blendParam * blendParam * blendParam)) * prevPose->translation.y
		+ (2 - 5 * (blendParam * blendParam) + 3 * (blendParam * blendParam * blendParam)) * pose0->translation.y
		+ (blendParam + 4 * (blendParam * blendParam) - 3 * (blendParam * blendParam * blendParam)) * pose1->translation.y
		+ (-(blendParam * blendParam) + (blendParam * blendParam * blendParam))) * poseNext->translation.y;

	pose_out->translation.z = (a3real)0.5 * ((-blendParam + 2 * (blendParam * blendParam) - (blendParam * blendParam * blendParam)) * prevPose->translation.z
		+ (2 - 5 * (blendParam * blendParam) + 3 * (blendParam * blendParam * blendParam)) * pose0->translation.z
		+ (blendParam + 4 * (blendParam * blendParam) - 3 * (blendParam * blendParam * blendParam)) * pose1->translation.z
		+ (-(blendParam * blendParam) + (blendParam * blendParam * blendParam))) * poseNext->translation.z;

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseDeConcat(a3_SpatialPose* pose_out, a3_SpatialPose* const pose_lh, a3_SpatialPose* const pose_rh)
{
	pose_out->translation.x = pose_lh->translation.x - pose_rh->translation.x;
	pose_out->translation.y = pose_lh->translation.y - pose_rh->translation.y;
	pose_out->translation.z = pose_lh->translation.z - pose_rh->translation.z;

	pose_out->scale.x = pose_lh->scale.x / pose_rh->scale.x;
	pose_out->scale.y = pose_lh->scale.y / pose_rh->scale.y;
	pose_out->scale.z = pose_lh->scale.z / pose_rh->scale.z;

	pose_out->angles.x = a3trigValid_sind(pose_lh->angles.x - pose_rh->angles.x);
	pose_out->angles.y = a3trigValid_sind(pose_lh->angles.y - pose_rh->angles.y);
	pose_out->angles.z = a3trigValid_sind(pose_lh->angles.z - pose_rh->angles.z);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseCalcScale(a3_SpatialPose* pose_out, a3real blendParam, a3_SpatialPose* const poseIn)
{
	if (blendParam == 0)
	{
		//scale is identity pose
		a3spatialPoseReset(pose_out);
	}
	else if (blendParam == 1)
	{
		pose_out = poseIn;
	}
	else
	{
		//have it set to gradient of sorts between the control and identity poses
		pose_out->scale.x = 1 * blendParam + poseIn->scale.x * (1 - blendParam);
		pose_out->scale.y = 1 * blendParam + poseIn->scale.y * (1 - blendParam);
		pose_out->scale.z = 1 * blendParam + poseIn->scale.z * (1 - blendParam);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseTriangular(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, a3_SpatialPose* const pose2, a3real scaleParam1, a3real scaleParam2)
{
	//interpolate to find pose within triangle of three control poses
	//then scale the initial pose to that interpolation I believe
	a3real scaleParam0 = 1 - scaleParam1 - scaleParam2;

	a3real4Lerp(pose_out->translation.v, pose0->translation.v, pose1->translation.v, scaleParam0);
	a3real4Lerp(pose_out->translation.v, pose1->translation.v, pose2->translation.v, scaleParam1);
	a3real4Lerp(pose_out->translation.v, pose2->translation.v, pose0->translation.v, scaleParam2);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseBiNearest(a3_SpatialPose* pose_out, a3_SpatialPose* const initPose0, a3_SpatialPose* const initPose1, a3_SpatialPose* const termPose0, a3_SpatialPose* const termPose1, a3real blendParam1, a3real blendParam2, a3real blendParam3)
{
	//run two nearest tests
	a3_SpatialPose* nearest1;
	a3_SpatialPose* nearest2;

	nearest1 = a3SpatialPoseNearest(nearest1, initPose0, termPose0, blendParam1);
	nearest2 = a3SpatialPoseNearest(nearest2, initPose1, termPose1, blendParam2);

	pose_out = a3SpatialPoseNearest(pose_out, nearest1, nearest2, blendParam3);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseBiLerp(a3_SpatialPose* pose_out, a3_SpatialPose* const initPose0, a3_SpatialPose* const initPose1, a3_SpatialPose* const termPose0, a3_SpatialPose* const termPose1, a3real blendParam1, a3real blendParam2, a3real blendParam3)
{
	a3_SpatialPose* lerp1;
	a3_SpatialPose* lerp2;

	a3spatialPoseLerp(lerp1, initPose0, termPose0, blendParam1);
	a3spatialPoseLerp(lerp2, initPose1, termPose1, blendParam2);

	a3spatialPoseLerp(pose_out, lerp1, lerp2, blendParam3);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseBiCubic(a3_SpatialPose* pose_out, a3_SpatialPose* const pose0, a3_SpatialPose* const pose1, 
	a3_SpatialPose* const pose2, a3_SpatialPose* const pose3, a3_SpatialPose* const pose4, 
	a3_SpatialPose* const pose5, a3_SpatialPose* const pose6, a3_SpatialPose* const pose7, 
	a3_SpatialPose* const pose8, a3_SpatialPose* const pose9, a3_SpatialPose* const pose10, 
	a3_SpatialPose* const pose11, a3_SpatialPose* const pose12, a3_SpatialPose* const pose13, 
	a3_SpatialPose* const pose14, a3_SpatialPose* const pose15, 
	a3real blendParam1, a3real blendParam2, a3real blendParam3, a3real blendParam4, a3real blendParam5)
{
	a3_SpatialPose* cubic1;
	a3_SpatialPose* cubic2;
	a3_SpatialPose* cubic3;
	a3_SpatialPose* cubic4;

	cubic1 = a3SpatialPoseCubic(cubic1, pose0, pose1, pose2, pose3, blendParam1);
	cubic2 = a3SpatialPoseCubic(cubic2, pose4, pose5, pose6, pose7, blendParam2);
	cubic3 = a3SpatialPoseCubic(cubic3, pose8, pose9, pose10, pose11, blendParam3);
	cubic4 = a3SpatialPoseCubic(cubic4, pose12, pose13, pose14, pose15, blendParam4);
	
	a3SpatialPoseCubic(pose_out, cubic1, cubic2, cubic3, cubic4, blendParam5);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseSmoothStep(a3_SpatialPose* pose_out, a3_SpatialPose const* initPose, a3_SpatialPose const* termPose, a3real const blendParam)
{
	//an easing interpolation for poses
	pose_out->translation.x = a3clamp(initPose->translation.x, termPose->translation.x, blendParam);
	pose_out->translation.y = a3clamp(initPose->translation.y, termPose->translation.y, blendParam);
	pose_out->translation.z = a3clamp(initPose->translation.z, termPose->translation.z, blendParam);

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseDescale(a3_SpatialPose* pose_out, a3_SpatialPose* const controlPose, a3real const blendParam)
{
	//calculate the inverse pose
	//a3SpatialPoseFindInverse()

	//calculate the descaled pose
	if (blendParam == 0)
	{
		//scale is identity pose
		a3spatialPoseReset(pose_out);
	}
	else if (blendParam == 1)
	{
		pose_out = controlPose;
	}
	else
	{
		//double check that this is changed to descale values
		pose_out->scale.x = 1 * controlPose->scale.x - 1 * (1 - blendParam);
		pose_out->scale.y = 1 * controlPose->scale.y - 1 * (1 - blendParam);
		pose_out->scale.z = 1 * controlPose->scale.z - 1 * (1 - blendParam);
	}

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseConvert(a3_SpatialPose* pose_out)
{
	//perform convert step for spatial pose
	//would that mean changing the transform matrix to identity or what exactly
	if (pose_out)
	{
		/*a3mat4 rx, ry, rz, r;
		a3real4x4SetRotateX(rx.m, pose_out->angles.x);
		a3real4x4SetRotateY(ry.m, pose_out->angles.y);
		a3real4x4SetRotateZ(rz.m, pose_out->angles.z);
		
		switch (order)
		{
		case a3poseEulerOrder_xyz:
			a3real4x4Product(r.m, rx.m, ry.m);
			a3real4x4Product(spatialPose->transform.m, r.m, rz.m);
			break;
		case a3poseEulerOrder_yzx:
			a3real4x4Product(r.m, ry.m, rz.m);
			a3real4x4Product(spatialPose->transform.m, r.m, rx.m);
			break;
		case a3poseEulerOrder_zxy:
			a3real4x4Product(r.m, rz.m, rx.m);
			a3real4x4Product(spatialPose->transform.m, r.m, ry.m);
			break;
		case a3poseEulerOrder_yxz:
			a3real4x4Product(r.m, ry.m, rx.m);
			a3real4x4Product(spatialPose->transform.m, r.m, rz.m);
			break;
		case a3poseEulerOrder_xzy:
			a3real4x4Product(r.m, rx.m, rz.m);
			a3real4x4Product(spatialPose->transform.m, r.m, ry.m);
			break;
		case a3poseEulerOrder_zyx:
			a3real4x4Product(r.m, rz.m, ry.m);
			a3real4x4Product(spatialPose->transform.m, r.m, rx.m);
			break;*/
		}

		a3real3MulS(pose_out->transform.v0.v, pose_out->scale.x);
		a3real3MulS(pose_out->transform.v1.v, pose_out->scale.y);
		a3real3MulS(pose_out->transform.v2.v, pose_out->scale.z);
		pose_out->transform.v3 = pose_out->translation;

	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseRevert(a3_SpatialPose* pose_out)
{
	//revert/restore the pose back to a previous state
	//reverse the process from before?


	return pose_out;
}

inline a3_SpatialPose* a3SpatialPoseForwardKinematics(a3_Hierarchy const* hierarchy, a3_SpatialPose* object_pose, a3_SpatialPose* local_pose)
{
	//perform forward kinematics

	//this might be the way to set up the kinematics work
	const a3_HierarchyNode* itr = hierarchy->nodes;
	
	if (itr->parentIndex >= 0)
		a3real4x4Product(object_pose[itr->index].transform.m,
			object_pose[itr->parentIndex].transform.m,
			local_pose[itr->index].transform.m);
	else
		object_pose[itr->index] = local_pose[itr->index];

	return object_pose;
}

inline a3_SpatialPose* a3SpatialPoseInverseKinematics(a3_Hierarchy const* hierarchy, a3_SpatialPose* object_pose, a3_SpatialPose* local_pose)
{
	//perform inverse kinematics

	const a3_HierarchyNode* itr = hierarchy->nodes;
	const a3_HierarchyNode* const end = itr + hierarchy->numNodes;;

	a3_SpatialPose* objectInv;

	a3SpatialPoseFindInverse(objectInv, object_pose);
	
	for (; itr < end; ++itr)
	{
		if (itr->parentIndex >= 0)
			a3real4x4Product(local_pose[itr->index].transform.m,
				objectInv[itr->parentIndex].transform.m,
				object_pose[itr->index].transform.m);
		else
			local_pose[itr->index] = object_pose[itr->index];
	}

	return local_pose;
}

//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H