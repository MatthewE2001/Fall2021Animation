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
		spatialPose->rotate.x = rx_degrees;
		spatialPose->rotate.y = ry_degrees;
		spatialPose->rotate.z = rz_degrees;

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
		spatialPose->rotate = a3vec3_zero; //this and translate are 0 because it is addition based
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
		//order of rotations depends on the order passed in from the parameter spatialPoseEulerOrder
		// M = T * ((R * R * R) * S)

		//a3mat4 M = (_ _ _ tx
				//_ RS _ ty
				//_ _ _ tz
				//0 0 0 1

		//a3mat3 S =  a3mat3 (spatialPose_in->scale.x, 0, 0,
					//0, spatialPose_in->scale.y, 0,
					//0, 0, spatialPose_in->scale.z);

		//a3mat3 Rx = a3mat3 (1, 0, 0
					//0, cos(spatialPose_in->rotate.x) -sin(spatialPose_in->rotate.x)
					//0 +sin(spatialPose_in->rotate.x) cos(spatialPose_in->rotate.x));
							 
		//a3mat3 Ry = a3mat3 (cos(spatialPose_in->rotate.y) 0 +sin(spatialPose_in->rotate.y) 
					//0 1 0	 
					//-sin(spatialPose_in->rotate.y) 0 cos(spatialPose_in->rotate.y));
							 
		//a3mat3 Rz = a3mat3 (cos(spatialPose_in->rotate.z) -sin(spatialPose_in->rotate.z) 0 
					//+sin(spatialPose_in->rotate.z) cos(spatialPose_in->rotate.z) 0 
					//0 0 1);

		//M = T * (Rx + Ry * Rz) * S

		// mat_out = M  // (I assume this is the case)
	}

	return -1;
}

// copy operation for single node pose
inline a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		spatialPose_out = spatialPose_in; //I assume this is what I have to copy

		return 1;
	}

	return -1;
}

//concat/combine
inline a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lh, const a3_SpatialPose* spatialPose_rh)
{
	//concat means ______

	if (spatialPose_out && spatialPose_lh && spatialPose_rh)
	{
		//spatialPose_out->transform; //no need to do transform no data is there yet (comes at later step in full process)
		spatialPose_out->rotate; // Euler - validate(lh + rh) (constrain sum to rotational domain)

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

inline a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3real u)
{
	//could I perhaps have to make a lerp but with vectors?
	if (spatialPose_out && spatialPose0 && spatialPose1) //these if statements are not needed forever
	{
		//spatialPose_out->transform; //no again because interpolations just full transform creates issues with object
		spatialPose_out->scale; // exp() //exponential equation for lerp (p1(po^-1))^u * p0 (this equation here may be a bit off)

		//rotate lerp functions
		spatialPose_out->rotate.x = a3lerpFunc(spatialPose0->rotate.x, spatialPose1->rotate.x, u);
		spatialPose_out->rotate.y = a3lerpFunc(spatialPose0->rotate.y, spatialPose1->rotate.y, u);
		spatialPose_out->rotate.z = a3lerpFunc(spatialPose0->rotate.z, spatialPose1->rotate.z, u);

		//scale lerp functions (what will become the exponential part?)
		

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