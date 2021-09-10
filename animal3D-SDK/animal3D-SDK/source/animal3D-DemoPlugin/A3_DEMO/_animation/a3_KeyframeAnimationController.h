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
	Keyframe animation clip controller. Basically a frame index manager. Very 
	limited in what one can do with this; could potentially be so much more.

	Modified by Benjamin Davies and Matthew Esslie
*/

#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H


#include "a3_KeyframeAnimation.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_ClipController			a3_ClipController;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// clip controller
// metaphor: playhead
struct a3_ClipController
{
	a3byte name[a3keyframeAnimation_nameLenMax];

	//clip - index of clip to control in referenced clip pool (what clip is being played by controller)
	a3ui32 clip; //a3ui32 since it says index?

	//clip time - current time relative to start of clip (value between 0 and full clip duration)
	a3real clipTime; //a3real cause duration is a3real but perhaps it should be otherwise?

	//clip parameter - normalized keyframe time (always between 0 and 1)
	float clipParam; //since it needs to be between 0 and 1 thats basically all float vals (or double?)

	//keyframe - index of current keyframe in referenced keyframe pool
	a3ui32 keyframe;

	//keyframe time - current time relative to current keyframe (between 0 and keyframe duration)
	float keyframeTime; //float??

	//keyframe parameter - normalized keyframe time
	float keyframeParam;

	//playback direction - the active behavior of playback (-1 reverse, 0 pause, 1 forward)
	int playbackDirection;

	//clip pool - pointer/reference to the pool of clips that the controller will ultimately control
	//replaced with constant to avoid warning when assigning a clip pool
	const a3_ClipPool* clipPool;
};


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool);

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt);

// set clip to play
a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimationController.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H