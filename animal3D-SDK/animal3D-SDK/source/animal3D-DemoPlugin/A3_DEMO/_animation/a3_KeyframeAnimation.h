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
	
	a3_KeyframeAnimation.h
	Data structures for fundamental keyframe animation.

	Modified by Benjamin Davies and Matthew Esslie
*/

#ifndef __ANIMAL3D_KEYFRAMEANIMATION_H
#define __ANIMAL3D_KEYFRAMEANIMATION_H


#include "animal3D-A3DM/a3math/a3vector.h"
#include "animal3D-A3DM/a3math/a3interpolation.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_Transition				a3_Transition;
typedef struct a3_Sample					a3_Sample; 
typedef struct a3_Keyframe					a3_Keyframe; //keyframe describes the gap between two samples
typedef struct a3_KeyframePool				a3_KeyframePool;
typedef struct a3_Clip						a3_Clip;
typedef struct a3_ClipPool					a3_ClipPool;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// constant values
enum
{
	a3keyframeAnimation_nameLenMax = 32,
};

//single generic value at time
struct a3_Sample
{
	a3real time; //the x axis/independent variable
	a3real value; //the y axis/dependent variable
};

// description of single keyframe
// metaphor: moment
struct a3_Keyframe
{
	// index in keyframe pool
	a3ui32 index;

	//duration - how long the keyframe lasts
	a3real duration; //i think it should be a3real based on a function down below

	//duration inverse - the reciprocal of duration
	float durationInverse; //I have it as float because it will be a fraction probably

	//data - the value of the sample from the keyframe, random value from index
	int data; //said a simple integer should suffice so I have that here
	
	//the known sample at the start of the interval
	a3_Sample sample;
};

// pool of keyframe descriptors
struct a3_KeyframePool
{
	// array of keyframes
	a3_Keyframe *keyframe[20];

	// number of keyframes
	a3ui32 count;
};


// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count);

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool);

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x);


//-----------------------------------------------------------------------------

// description of single clip
// metaphor: timeline
struct a3_Clip
{
	// clip name
	a3byte name[a3keyframeAnimation_nameLenMax];

	// index in clip pool
	a3ui32 index;

	//duration - the length of the clip (sum of all keyframe lengths) (cannot be 0)
	a3real duration;

	//duration inverse - reciprocal of duration
	float durationInverse;

	//keyframe count - number of keyframes referenced by clip
	a3ui32 keyframeCount; //a3ui32??

	//first keyframe - index of first keyframe referenced by clip
	a3ui32 firstKeyframe;

	//last keyframe - index of final keyframe referenced by clip
	a3ui32 lastKeyframe;

	//keyframe pool - pointer to a pool of keyframes for those within the clip (clip references keyframes owned elsewhere)
	a3_KeyframePool* keyframePool;
};

// group of clips
struct a3_ClipPool
{
	// array of clips
	a3_Clip* clip[12]; //changing to 12 to match the number of clips in the clip file format application

	// number of clips
	a3ui32 count;
};

struct a3_Transition
{
	//the target clip for the transition
	a3_Clip* targetClip;
	a3_Clip* targetClipReverse; //I assume this is what it means by targets for reverse as well

	//the target index for the transition
	a3index targetIndex;

	//the target clip pool for the transition
	a3_ClipPool* targetClipPool;

	//reinitialization for clip controller (would include ??)
	//a3_clipPool* nextClipPool; //or something to fuel the transitory process
};


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 count, const a3_KeyframePool* keyframePool);

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool);

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex);

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax]);

// calculate clip duration as sum of keyframes' durations
a3i32 a3clipCalculateDuration(a3_Clip* clip);

// calculate keyframes' durations by distributing clip's duration
a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration);

//initialize the transition structure
a3i32 a3transitionInit(a3_Transition* transition_Out, a3_Clip* target, a3_ClipPool* targetPool, a3index targetIndex);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimation.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_H