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
	
	a3_KeyframeAnimationController.h
	inline definitions for keyframe animation controller.
*/

/*
Adjustments to the code made by Matthew Esslie and Benjamin Davies, this is primarily code from
Daniel S. Buckstein
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL


//-----------------------------------------------------------------------------

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt)
{
	//for step/nearest 
		//nothing special occurs (same as Lab1)
	//for lerp 
		//if end, k0 <- k1, calc new k1
		//if beginning, k1 <- k0, calc new k0
	//for catmull
		//if over, kp <- k0 <- k1 <- kn, calc new kn
		//if begin, kn <- k1 <- k0 <- kp, calc new kp


	clipCtrl->clipTime += dt;
	clipCtrl->keyframeTime += dt;

	//so now I need to resolve time to determine the new keyframe and clip time? (7 cases of resolution?)
	//case 1: stay still and do nothing
	if (dt == 0.0f) //move check in front of time iteration
	{
		//setting the clip controller to a paused state
		clipCtrl->playbackDirection = 0;
	}
	//case 2: going forward within same keyframe
	//I think there might not need to be anything specific to do here

	//case 3: forward into a new keyframe
	//updating keyframe value if it passes the duration
	if (clipCtrl->keyframeTime > clipCtrl->keyframePtr0->duration) //use time for check (if (time > duration)
	{
		clipCtrl->keyframeIndex0_clip++;
		clipCtrl->keyframeIndex1_clip++;
	}

	//case 4: forward into the end of the clip
	//restarting the loop if necessary
	if (clipCtrl->clipTime > clipCtrl->clipPtr->duration) //same as with case 3
	{
		//I want this to switch using the transition parameters
		clipCtrl->clipIndex_pool = 0;
		clipCtrl->keyframe = 0; //making sure the keyframe values are also set to loop
		clipCtrl->keyframeTime = 0;
		clipCtrl->clipTime = 0;
	}

	//reverse cases could potentially have to do with dropping below durationInverse
	//case 5: reverse in the same keyframe

	//case 6: reverse into a new keyframe
	if (clipCtrl->keyframeTime < clipCtrl->keyframePtr0->durationInverse) //I think duration inverse?
	{
		clipCtrl->keyframeIndex0_clip--;
		clipCtrl->keyframeIndex1_clip--;
	}

	//case 7: reverse into the end of the clip
	if (clipCtrl->clipTime < clipCtrl->clipPtr->durationInverse)
	{
		//I want this to switch using the transition parameters
		clipCtrl->clipIndex_pool = clipCtrl->clipPool->count; //need to move it back to the max value somehow
		//clipCtrl->keyframe = clipCtrl->clipPool->clip->keyframePool->count; //??
		clipCtrl->keyframeTime = clipCtrl->keyframePtr0->duration; //possibly
	}

	//finally to normalize the time values (double check this later)
	clipCtrl->clipParam = clipCtrl->clipTime * clipCtrl->clipPtr->durationInverse;
	clipCtrl->keyframeParam = clipCtrl->keyframeTime * clipCtrl->keyframePtr0->durationInverse;

	return -1;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	clipCtrl->clipPool = clipPool;
	clipCtrl->clip = clipIndex_pool;

	return -1;
}

inline a3i32 a3clipControllerEvaluate(a3_ClipController const* clipCtrl, a3_Sample* sample_out)
{
	if (clipCtrl && clipCtrl->clipPtr && sample_out)
	{
		//0 - no interpolation (step function)
		//*sample_out = clipCtrl->keyframePtr0->sample;

		//1 - nearest
		// if (u < 0.5) then k0, else k1 (u param, k0 value and k1 value)

		//2 - lerp
		// k = k0 + (k1 - k0)u
		sample_out->time = clipCtrl->keyframeTime;
		sample_out->value = a3lerp(clipCtrl->keyframePtr0->sample.value, 
			clipCtrl->keyframePtr1->sample.value,
			clipCtrl->keyframeParam);

		//3 - Catmull-Rom/Cubic Hermite (would need increased info in clip controller for these)
		//specifically a previous and a next step along with the k0 and k1

		return clipCtrl->keyframeIndex0_clip;
	}

	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H