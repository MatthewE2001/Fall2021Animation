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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

/*
Adjustments to the code made by Matthew Esslie and Benjamin Davies, this is primarily code from
Daniel S. Buckstein
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	// loop to assign the name
	for (a3ui32 i = 0; i < a3keyframeAnimation_nameLenMax; i++)
	{
		clipCtrl_out->name[i] = ctrlName[i];
	}

	clipCtrl_out->clipPool = clipPool;
	clipCtrl_out->clip = clipIndex_pool; //this maybe should be different

	clipPool->clip[clipIndex_pool]->duration = 10;
	clipCtrl_out->clipPtr = clipPool->clip[clipIndex_pool];
	clipCtrl_out->keyframePtr0 = clipCtrl_out->clipPool->clip[clipIndex_pool]->keyframePool->keyframe[0];
	clipCtrl_out->keyframePtr1 = clipCtrl_out->clipPool->clip[clipIndex_pool]->keyframePool->keyframe[1];

	return -1;
}


//-----------------------------------------------------------------------------
