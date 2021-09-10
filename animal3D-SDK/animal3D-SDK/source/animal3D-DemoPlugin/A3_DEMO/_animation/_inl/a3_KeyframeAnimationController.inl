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
	if (dt == 0.0f)
	{
		//setting the clip controller to a paused state
		clipCtrl->playbackDirection = 0;
	}
	//case 2: going forward within same keyframe
	//I think there might not need to be anything specific to do here

	//case 3: forward into a new keyframe
	//updating keyframe value if it passes the duration
	if (clipCtrl->keyframeParam >= 1)
	{
		clipCtrl->keyframe++;
	}

	//case 4: forward into the end of the clip
	//restarting the loop if necessary
	if (clipCtrl->clipParam >= 1)
	{
		clipCtrl->clipTime = 0;
		clipCtrl->keyframe = 0; //making sure the keyframe values are also set to loop
		clipCtrl->keyframeTime = 0;
	}

	//reverse cases could potentially have to do with dropping below durationInverse
	//case 5: reverse in the same keyframe

	//case 6: reverse into a new keyframe

	//case 7: reverse into the end of the clip

	//finally to normalize the time values
	//should I divide by a clip/keyframe durations or just something like 60?
	clipCtrl->clipParam = clipCtrl->clipParam + (clipCtrl->clipTime / clipCtrl->clipPool->clip[clipCtrl->clip]->duration); //I think? cause then when it passes over the 60 frames it will be over 1 and needs to be reset
	
	clipCtrl->keyframeParam = clipCtrl->keyframeParam + (clipCtrl->keyframeTime / clipCtrl->clipPool->
		clip[clipCtrl->clip]->keyframePool[clipCtrl->keyframe].keyframe[clipCtrl->keyframe]->duration); //all in this part of clip?
	

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


		return clipCtrl->keyframeIndex0_clip;
	}

	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H