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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		//printf("%i", hierarchy->numNodes);
		// allocate everything (one malloc)
		poseGroup_out->hierarchy = malloc(sizeof(a3_Hierarchy)); //I think malloc to pose group?
		//hierarchy = malloc(sizeof(hierarchy));

		// set pointers
		poseGroup_out->hierarchy = hierarchy;
		//printf("%i", poseGroup_out->hierarchy->numNodes);
		// reset all data (what do I need to do to reset all data)

		//this will probably need to change in the future
		poseGroup_out->hpose[0] = malloc(sizeof(a3_HierarchyPose));
		poseGroup_out->hpose[0]->spatialPose = malloc(sizeof(a3_SpatialPose));
		a3spatialPoseSetTranslation(poseGroup_out->hpose[0]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetRotation(poseGroup_out->hpose[0]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetScale(poseGroup_out->hpose[0]->spatialPose, 0.0, 0.0, 0.0);

		poseGroup_out->hpose[1] = malloc(sizeof(a3_HierarchyPose));
		poseGroup_out->hpose[1]->spatialPose = malloc(sizeof(a3_SpatialPose));
		a3spatialPoseSetTranslation(poseGroup_out->hpose[1]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetRotation(poseGroup_out->hpose[1]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetScale(poseGroup_out->hpose[1]->spatialPose, 0.0, 0.0, 0.0);

		poseGroup_out->hpose[2] = malloc(sizeof(a3_HierarchyPose));
		poseGroup_out->hpose[2]->spatialPose = malloc(sizeof(a3_SpatialPose));
		a3spatialPoseSetTranslation(poseGroup_out->hpose[2]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetRotation(poseGroup_out->hpose[2]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetScale(poseGroup_out->hpose[2]->spatialPose, 0.0, 0.0, 0.0);

		poseGroup_out->hpose[3] = malloc(sizeof(a3_HierarchyPose));
		poseGroup_out->hpose[3]->spatialPose = malloc(sizeof(a3_SpatialPose));
		a3spatialPoseSetTranslation(poseGroup_out->hpose[3]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetRotation(poseGroup_out->hpose[3]->spatialPose, 0.0, 0.0, 0.0);
		a3spatialPoseSetScale(poseGroup_out->hpose[3]->spatialPose, 0.0, 0.0, 0.0);

		// done
		return 1;
	}
	printf("IF DOES NOT WORKS");
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup); //releasing just the hierarchy?

		// reset pointers
		poseGroup->hierarchy = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements

		// allocate everything (one malloc)
		state_out->hierarchy = malloc(sizeof(a3_Hierarchy));

		// set pointers
		state_out->hierarchy = hierarchy;

		// reset all data

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		// release everything (one free)
		free(state);

		// reset pointers
		state->hierarchy = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->poseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{
		//a3fileStreamOpenRead(, resourceFilePath);

		//poseGroup_out = 
		//hierarchy_out = 
	}

	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->poseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{
		//a3fileStreamOpenRead(, resourceFilePath);

		//poseGroup_out = 
		//hierarchy_out = 
	}

	return -1;
}

//save HTR file, write and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(a3_HierarchyPoseGroup const* poseGroup, a3_Hierarchy* hierarchy, const a3byte* resourceFilePath)
{
	if (poseGroup && !poseGroup->poseCount && hierarchy && !hierarchy->numNodes && resourceFilePath && *resourceFilePath)
	{
		//a3fileStreamOpenWrite(resourseFilePath, 'w');

		return 0;
	}


	return -1;
}

//save BVH file, write and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(a3_HierarchyPoseGroup const* poseGroup, a3_Hierarchy* hierarchy, const a3byte* resourceFilePath)
{
	if (poseGroup && !poseGroup->poseCount && hierarchy && !hierarchy->numNodes && resourceFilePath && *resourceFilePath)
	{
		//a3fileStreamOpenWrite(resourceFilePath, 'w');

		//I think values from the file go to hierarchy

		return 0;
	}

	return -1;
}


//-----------------------------------------------------------------------------
