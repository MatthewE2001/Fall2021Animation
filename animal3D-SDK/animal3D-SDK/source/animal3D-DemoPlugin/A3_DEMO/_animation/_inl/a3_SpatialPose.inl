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
		spatialPose->rotate_quat = a3vec4_zero;
		spatialPose->rotate_euler = a3vec3_zero; //this and translate are 0 because it is addition based
		spatialPose->translate = a3vec3_zero;
		spatialPose->scale = a3vec3_one; //this is one cause it is multiplication based (also transform is multiply based)

		//finished
		return 1;
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

		//do I even need to fill out m? probably I assume
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
		a3real cosEulerX = a3cosr(spatialPose_in->rotate_euler.x);
		a3real negSinEulerX = -a3trigValid_sinr(spatialPose_in->rotate_euler.x);
		a3real sinEulerX = a3trigValid_sinr(spatialPose_in->rotate_euler.x);
		a3real3x3Set(rotateX.m, 1, 0, 0, 0, cosEulerX, negSinEulerX, 0, sinEulerX, cosEulerX);
							 
		a3mat3 rotateY;
		a3real cosEulerY = a3cosr(spatialPose_in->rotate_euler.y);
		a3real negSinEulerY = -a3trigValid_sinr(spatialPose_in->rotate_euler.y);
		a3real sinEulerY = a3trigValid_sinr(spatialPose_in->rotate_euler.y);
		a3real3x3Set(rotateY.m, cosEulerY, 0, sinEulerY, 0, 1, 0, negSinEulerY, 0, cosEulerY);
							 
		a3mat3 rotateZ;
		a3real cosEulerZ = a3cosr(spatialPose_in->rotate_euler.z);
		a3real negSinEulerZ = -a3trigValid_sinr(spatialPose_in->rotate_euler.z);
		a3real sinEulerZ = a3trigValid_sinr(spatialPose_in->rotate_euler.z);
		a3real3x3Set(rotateZ.m, cosEulerZ, negSinEulerZ, 0, sinEulerZ, cosEulerZ, 0, 0, 0, 1);

		//setting up the correct formula for each possible order
		if (order == a3poseEulerOrder_xyz)
		{
			//this is the way to multiply everything in the order that is needed I believe
			a3mat3 rotateXYZ; 
			a3real3x3Product(rotateXYZ.m, rotateX.m, rotateY.m);
			a3real3x3Product(rotateXYZ.m, rotateXYZ.m, rotateZ.m);
			a3real3x3Product(rotateXYZ.m, rotateXYZ.m, scale.m);

			//I think this is the way I need to transfer this over into another mat4
			a3mat4 combinedMat;
			a3real4x4Set(combinedMat.m, rotateXYZ.m00, rotateXYZ.m01, rotateXYZ.m02, 0,
				rotateXYZ.m10, rotateXYZ.m11, rotateXYZ.m12, 0, rotateXYZ.m20, rotateXYZ.m21, rotateXYZ.m22,
				0, 0, 0, 0, 1);

			a3real4x4Product(combinedMat.m, transform.m, combinedMat.m);

			m = combinedMat;
		}
		else if (order == a3poseEulerOrder_yzx)
		{
			a3mat3 rotateYZX;
			a3real3x3Product(rotateYZX.m, rotateY.m, rotateZ.m);
			a3real3x3Product(rotateYZX.m, rotateYZX.m, rotateX.m);
			a3real3x3Product(rotateYZX.m, rotateYZX.m, scale.m);

			a3mat4 combinedMat;
			a3real4x4Set(combinedMat.m, rotateYZX.m00, rotateYZX.m01, rotateYZX.m02, 0,
				rotateYZX.m10, rotateYZX.m11, rotateYZX.m12, 0, rotateYZX.m20, rotateYZX.m21, rotateYZX.m22,
				0, 0, 0, 0, 1);

			a3real4x4Product(combinedMat.m, transform.m, combinedMat.m);

			m = combinedMat;
		}
		else if (order == a3poseEulerOrder_zxy)
		{
			a3mat3 rotateZXY;
			a3real3x3Product(rotateZXY.m, rotateZ.m, rotateX.m);
			a3real3x3Product(rotateZXY.m, rotateZXY.m, rotateY.m);
			a3real3x3Product(rotateZXY.m, rotateZXY.m, scale.m);

			a3mat4 combinedMat;
			a3real4x4Set(combinedMat.m, rotateZXY.m00, rotateZXY.m01, rotateZXY.m02, 0,
				rotateZXY.m10, rotateZXY.m11, rotateZXY.m12, 0, rotateZXY.m20, rotateZXY.m21, rotateZXY.m22,
				0, 0, 0, 0, 1);

			a3real4x4Product(combinedMat.m, transform.m, combinedMat.m);

			m = combinedMat;
		}
		else if (order == a3poseEulerOrder_yxz)
		{
			a3mat3 rotateYXZ;
			a3real3x3Product(rotateYXZ.m, rotateY.m, rotateX.m);
			a3real3x3Product(rotateYXZ.m, rotateYXZ.m, rotateZ.m);
			a3real3x3Product(rotateYXZ.m, rotateYXZ.m, scale.m);

			a3mat4 combinedMat;
			a3real4x4Set(combinedMat.m, rotateYXZ.m00, rotateYXZ.m01, rotateYXZ.m02, 0,
				rotateYXZ.m10, rotateYXZ.m11, rotateYXZ.m12, 0, rotateYXZ.m20, rotateYXZ.m21, rotateYXZ.m22,
				0, 0, 0, 0, 1);

			a3real4x4Product(combinedMat.m, transform.m, combinedMat.m);

			m = combinedMat;
		}
		else if (order == a3poseEulerOrder_xzy)
		{
			a3mat3 rotateXZY;
			a3real3x3Product(rotateXZY.m, rotateX.m, rotateZ.m);
			a3real3x3Product(rotateXZY.m, rotateXZY.m, rotateY.m);
			a3real3x3Product(rotateXZY.m, rotateXZY.m, scale.m);

			//I think this is the way I need to transfer this over into another mat4
			a3mat4 combinedMat;
			a3real4x4Set(combinedMat.m, rotateXZY.m00, rotateXZY.m01, rotateXZY.m02, 0,
				rotateXZY.m10, rotateXZY.m11, rotateXZY.m12, 0, rotateXZY.m20, rotateXZY.m21, rotateXZY.m22,
				0, 0, 0, 0, 1);

			a3real4x4Product(combinedMat.m, transform.m, combinedMat.m);

			m = combinedMat;
		}
		else if (order == a3poseEulerOrder_zyx)
		{
			a3mat3 rotateZYX;
			a3real3x3Product(rotateZYX.m, rotateZ.m, rotateY.m);
			a3real3x3Product(rotateZYX.m, rotateZYX.m, rotateX.m);
			a3real3x3Product(rotateZYX.m, rotateZYX.m, scale.m);

			a3mat4 combinedMat;
			a3real4x4Set(combinedMat.m, rotateZYX.m00, rotateZYX.m01, rotateZYX.m02, 0,
				rotateZYX.m10, rotateZYX.m11, rotateZYX.m12, 0, rotateZYX.m20, rotateZYX.m21, rotateZYX.m22,
				0, 0, 0, 0, 1);

			a3real4x4Product(combinedMat.m, transform.m, combinedMat.m);

			m = combinedMat;
		}

		mat_out = &m;  //(I assume this is the case)

		return 1;
	}

	return -1;
}

// copy operation for single node pose
inline a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		spatialPose_out = spatialPose_in; //just copying the thing right over

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
			 //Quat: (lh * rh) = (w_l + v_l)(w_r + v_r)
					// = (w_l * w_r - v_l . v_r) + (w_l * v_r + w_r * v_l + v_l x v_r) //the x is cross product
			//w is left side
			//v is right side
			//l is w
			//r is xyz
			a3vec4 tmp;

			tmp.w = (spatialPose_lh->rotate_quat.w + spatialPose_rh->rotate_quat.w);
			tmp.x = (spatialPose_lh->rotate_quat.x + spatialPose_rh->rotate_quat.x);
			tmp.y = (spatialPose_lh->rotate_quat.y + spatialPose_rh->rotate_quat.y);
			tmp.z = (spatialPose_lh->rotate_quat.z + spatialPose_rh->rotate_quat.z);

			spatialPose_out->rotate_quat = tmp;

			a3real realTmp, realTmp2, realTmp3;
			realTmp = spatialPose_lh->rotate_quat.w * (spatialPose_lh->rotate_quat.x * spatialPose_lh->rotate_quat.y * spatialPose_lh->rotate_quat.z);
			realTmp2 = a3real4LengthSquared(spatialPose_rh->rotate_quat.v);
			realTmp -= realTmp2;

			realTmp2 = spatialPose_lh->rotate_quat.w * (spatialPose_lh->rotate_quat.x * spatialPose_lh->rotate_quat.y * spatialPose_lh->rotate_quat.z);
			realTmp2 += spatialPose_lh->rotate_quat.x;
			realTmp2 += spatialPose_lh->rotate_quat.y;
			realTmp2 += spatialPose_lh->rotate_quat.z;

			a3real3Cross(tmp.v, spatialPose_lh->rotate_quat.v, spatialPose_rh->rotate_quat.v);
			realTmp3 = spatialPose_lh->rotate_quat.w;
			realTmp3 += tmp.w;

			realTmp2 *= realTmp3;

			realTmp += realTmp2;

			spatialPose_out->rotate_quat.w = realTmp;
		}
		else //and this is the euler info
		{
			// Euler - validate(lh + rh) (constrain sum to rotational domain)
			spatialPose_out->rotate_euler.x = a3trigValid_sind(spatialPose_lh->rotate_euler.x + spatialPose_rh->rotate_euler.x);
			spatialPose_out->rotate_euler.y = a3trigValid_sind(spatialPose_lh->rotate_euler.y + spatialPose_rh->rotate_euler.y);
			spatialPose_out->rotate_euler.z = a3trigValid_sind(spatialPose_lh->rotate_euler.z + spatialPose_rh->rotate_euler.z);
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
			//Quat = slerp(q0, q1, u)
			a3quatSlerp(spatialPose_out->rotate_quat.v, spatialPose0->rotate_quat.v, spatialPose1->rotate_quat.v, u); 
											// = (sin([1 - t]y)q0 + sin([t]y)q1) / sin(y)
											// y = acos(q0 . q1)
			// 2: lerp = non-unit-length -> uniform scale
			spatialPose_out->rotate_quat.x = a3lerpFunc(spatialPose0->rotate_quat.x, spatialPose1->rotate_quat.x, u);
			spatialPose_out->rotate_quat.y = a3lerpFunc(spatialPose0->rotate_quat.y, spatialPose1->rotate_quat.y, u);
			spatialPose_out->rotate_quat.z = a3lerpFunc(spatialPose0->rotate_quat.z, spatialPose1->rotate_quat.z, u);
											// s = |q|^2
			// 3: nlerp = normalize(lerp) 
			a3real2Normalize(spatialPose_out->rotate_quat.v);
			
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

inline a3i32 a3spatialPoseStep(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3boolean usingQuaternions)
{
	//for step/nearest 
		//nothing special occurs (same as Lab1)
	if (usingQuaternions)
	{

	}
	else
	{

	}

	return -1;
}

inline a3i32 a3spatialPoseNearest(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3real u, const a3boolean usingQuaternions)
{
	//1 - nearest
		// if (u < 0.5) then k0, else k1 (u param, k0 value and k1 value)
	if (usingQuaternions)
	{

	}
	else
	{

	}

	return -1;
}

inline a3i32 a3spatialPoseSmoothStep(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose0, const a3_SpatialPose* spatialPose1, const a3boolean usingQuaternions)
{
	//for smoothstep:
	if (usingQuaternions)
	{

	}
	else
	{

	}

	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SPATIALPOSE_INL
#endif	// __ANIMAL3D_SPATIALPOSE_H