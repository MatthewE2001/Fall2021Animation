#include "..\a3_SpatialPose.h"
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
	
	a3_SpatialPose.inl
	Implementation of inline spatial pose operations.
*/


#ifdef __ANIMAL3D_SPATIALPOSE_H
#ifndef __ANIMAL3D_SPATIALPOSE_INL
#define __ANIMAL3D_SPATIALPOSE_INL


//-----------------------------------------------------------------------------

// set rotation values for a single node pose
inline a3i32 a3spatialPoseSetRotation(a3_SpatialPose* spatialPose, const a3f32 rx_degrees, const a3f32 ry_degrees, const a3f32 rz_degrees)
{
	if (spatialPose)
	{
		spatialPose->rotate_euler.x = rx_degrees;
		spatialPose->rotate_euler.y = ry_degrees;
		spatialPose->rotate_euler.z = rz_degrees;

		return 1;
	}

	return -1;
}

// scale
inline a3i32 a3spatialPoseSetScale(a3_SpatialPose* spatialPose, const a3f32 sx, const a3f32 sy, const a3f32 sz)
{
	if (spatialPose)
	{
		spatialPose->scale.x = sx;
		spatialPose->scale.y = sy;
		spatialPose->scale.z = sz;

		return 1;
	}

	return -1;
}

// translation
inline a3i32 a3spatialPoseSetTranslation(a3_SpatialPose* spatialPose, const a3f32 tx, const a3f32 ty, const a3f32 tz)
{
	if (spatialPose)
	{
		spatialPose->translate.x = tx;
		spatialPose->translate.y = ty;
		spatialPose->translate.z = tz;

		return 1;
	}

	return -1;
}


//-----------------------------------------------------------------------------

// reset single node pose
inline a3i32 a3spatialPoseReset(a3_SpatialPose* spatialPose)
{
	if (spatialPose)
	{
		spatialPose->transform = a3mat4_identity;
		spatialPose->rotate_quat =  
		spatialPose->rotate_euler = a3vec3_zero; //this and translate are 0 because it is addition based
		spatialPose->translate = a3vec3_zero;
		spatialPose->scale = a3vec3_one; //this is one cause it is multiplication based (also transform is multiply based)

		//finished
		return 0;
	}

	return -1;
}

// convert single node pose to matrix
inline a3i32 a3spatialPoseConvert(a3mat4* mat_out, const a3_SpatialPose* spatialPose_in, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (mat_out && spatialPose_in)
	{
		a3mat4 m;
		//order of rotations depends on the order passed in from the parameter spatialPoseEulerOrder
		// M = T * ((R * R * R) * S)

		//a3mat4 M = (_ _ _ tx
				//_ RS _ ty
				//_ _ _ tz
				//0 0 0 1)
		
		a3mat4 transform = spatialPose_in->transform;

		//if I need to make any of these mat4s I will do 0, 0, 0, 1 for the last 4 vals
		a3mat3 scale;
		a3real3x3Set(scale.m, spatialPose_in->scale.x, 0, 0, 0, spatialPose_in->scale.y, 0, 0, 0, spatialPose_in->scale.z);

		//maybe should not be euler come back to check this
		a3mat3 rotateX;
		a3real3x3Set(rotateX.m, 1, 0, 0, 0, cos(spatialPose_in->rotate_euler.x), -sin(spatialPose_in->rotate_euler.x), 0, sin(spatialPose_in->rotate_euler.x), cos(spatialPose_in->rotate_euler.x));
							 
		a3mat3 rotateY;
		a3real3x3Set(rotateY.m, cos(spatialPose_in->rotate_euler.y), 0, sin(spatialPose_in->rotate_euler.y), 0, 1, 0, -sin(spatialPose_in->rotate_euler.y), 0, cos(spatialPose_in->rotate_euler.y));
							 
		a3mat3 rotateZ;
		a3real3x3Set(rotateZ.m, cos(spatialPose_in->rotate_euler.z), -sin(spatialPose_in->rotate_euler.z), 0, sin(spatialPose_in->rotate_euler.z), cos(spatialPose_in->rotate_euler.z), 0, 0, 0, 1);

		//setting up the correct formula for each possible order
		if (order == a3poseEulerOrder_xyz)
		{
			//m = transform.m * (rotateX.m * rotateY.m * rotateZ.m) * scale.m;
		}
		else if (order == a3poseEulerOrder_yzx)
		{
			//m = transform.m * (rotateY.m * rotateZ.m * rotateX.m) * scale.m;
		}
		else if (order == a3poseEulerOrder_zxy)
		{
			//m = transform.m * (rotateZ.m * rotateX.m * rotateY.m) * scale.m;
		}
		else if (order == a3poseEulerOrder_yxz)
		{
			//m = transform.m * (rotateY.m * rotateX.m * rotateZ.m) * scale.m;
		}
		else if (order == a3poseEulerOrder_xzy)
		{
			//m = transform.m * (rotateX.m * rotateZ.m * rotateY.m) * scale.m;
		}
		else if (order == a3poseEulerOrder_zyx)
		{
			//m = transform.m * (rotateZ.m * rotateY.m * rotateX.m) * scale.m;
		}

		mat_out = &m;  //(I assume this is the case)
	}

	return -1;
}

// copy operation for single node pose
inline a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		spatialPose_out = spatialPose_in; //I assume this is what I have to copy

		//or maybe copy each part individually

		return 1;
	}

	return -1;
}

//concat/combine
inline a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lh, const a3_SpatialPose* spatialPose_rh, const a3boolean usingQuaternions)
{
	if (spatialPose_out && spatialPose_lh && spatialPose_rh)
	{
		if (usingQuaternions) //then put quaternions to use
		{
			spatialPose_out->rotate_quat; //Quat - (lh * rh) = (w_l + v_l)(w_r + v_r)
										// = (w_l * w_r - v_l . v_r) + (w_l * v_r + w_r * v_l + v_l x v_r) //the x is cross
		}
		else //and this is the euler info
		{
			spatialPose_out->rotate_euler; // Euler - validate(lh + rh) (constrain sum to rotational domain)
		}
		
		//spatialPose_out->transform; //no need to do transform no data is there yet (comes at later step in full process)

		//this seems like the best way to do component wise since the system does not like me making an a3vec3
		spatialPose_out->scale.x = spatialPose_lh->scale.x * spatialPose_rh->scale.x;
		spatialPose_out->scale.y = spatialPose_lh->scale.y * spatialPose_rh->scale.y;
		spatialPose_out->scale.z = spatialPose_lh->scale.z * spatialPose_rh->scale.z;
		
		//split the translate up into each part of the vector in order to add it
		spatialPose_out->translate.x = spatialPose_lh->translate.x + spatialPose_rh->translate.x;
		spatialPose_out->translate.y = spatialPose_lh->translate.y + spatialPose_rh->translate.y;
		spatialPose_out->translate.z = spatialPose_lh->translate.z + spatialPose_rh->translate.z;
		
		return 0;
	}

	return -1;
}

inline a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3real u, const a3boolean usingQuaternions)
{
	//could I perhaps have to make a lerp but with vectors?
	if (spatialPose_out && spatialPose0 && spatialPose1) //these if statements are not needed forever
	{
		if (usingQuaternions) //then put quaternions to use
		{
			spatialPose_out->rotate_quat; //Quat = slerp(q0, q1, u)
											// = (sin([1 - t]y)q0 + sin([t]y)q1) / sin(y)
											// y = acos(q0 . q1)
										// 2: lerp = non-unit-length -> uniform scale
											// s = |q|^2
										// 3: nlerp = normalize(lerp) 
		}
		else //and this is the euler info
		{
			//rotate lerp functions with euler
			spatialPose_out->rotate_euler.x = a3lerpFunc(spatialPose0->rotate_euler.x, spatialPose1->rotate_euler.x, u);
			spatialPose_out->rotate_euler.y = a3lerpFunc(spatialPose0->rotate_euler.y, spatialPose1->rotate_euler.y, u);
			spatialPose_out->rotate_euler.z = a3lerpFunc(spatialPose0->rotate_euler.z, spatialPose1->rotate_euler.z, u);
		}

		//spatialPose_out->transform; //no again because interpolations just full transform creates issues with object

		//scale lerp functions (what will become the exponential part?)
		spatialPose_out->scale; // exp() //exponential equation for lerp (p1(po^-1))^u * p0 (this equation here may be a bit off)

		//hopefully this should lerp for translate
		spatialPose_out->translate.x = a3lerpFunc(spatialPose0->translate.x, spatialPose1->translate.x, u);
		spatialPose_out->translate.y = a3lerpFunc(spatialPose0->translate.y, spatialPose1->translate.y, u);
		spatialPose_out->translate.z = a3lerpFunc(spatialPose0->translate.z, spatialPose1->translate.z, u);

		return 0;
	}

	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SPATIALPOSE_INL
#endif	// __ANIMAL3D_SPATIALPOSE_H