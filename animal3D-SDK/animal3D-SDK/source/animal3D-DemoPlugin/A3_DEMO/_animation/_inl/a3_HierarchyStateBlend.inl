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
	pose_out->transformMat = a3mat4_identity;
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

inline a3real a3SpatialPoseIntegrateEuler(a3real x, a3real dxdt, a3real dt)
{
	//Math: xt + dt = xt + x′tdt = xt + (dxt / dt)dt
	a3real eulerIntegrate;
	eulerIntegrate = x + (dxdt / dt) * dt;

	return eulerIntegrate;
}

inline a3real a3SpatialPoseIntegrateKinematic(a3real x, a3real dxdt, a3real dxdt2, a3real dt)
{
	//Math: xt+dt=xt+x′tdt+x′′tdt2/2=xt+(dxt/dt)dt+(d2xt/dt2)dt2/2
	a3real kinematicIntegrate;
	kinematicIntegrate = x + (dxdt / dt) * dt + (dxdt2 / (dt * dt)) * ((dt * dt) / 2);

	return kinematicIntegrate;
}

inline a3real a3SpatialPoseIntegrateLerp(a3real x, a3real xc, a3real u)
{
	//Math: xt+dt=LERPxt,xc(u)=xt+(xc−xt)u
	a3real lerpIntegrate;
	lerpIntegrate = x + (xc - x) * u;

	return lerpIntegrate;
}

inline a3real a3SpatialPoseQuadraticBezier(a3real p0, a3real p1, a3real p2, a3real const t)
{
	//P(t) = P0*(1-t)^2 + P1*2*(1-t)*t + P2*t^2
	a3real bezierQuad;
	bezierQuad = p0 * ((1 - t) * (1 - t)) + p1 * 2 * (1 - t) * t + p2 * (t * t);

	return bezierQuad;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H