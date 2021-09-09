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

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL


//-----------------------------------------------------------------------------

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt)
{
	clipCtrl->clipTime += dt;
	clipCtrl->keyframeTime += dt;
	
	//so now i need to resolve time to determine the new keyframe and clip time? (7 cases of resolution?)
	//case 1: stay still and do nothing
	if (dt == 0.0f)
	{
		//I don't think anything really needs to happen in here 
	}
	//case 2: going forward within same keyframe (side note: this feels somewhat off, maybe use clip index)
	//if (clipCtrl->keyframeTime < clipCtrl->clipPool->clip->keyframePool->keyframe->duration)
	
	//case 3: forward into a new keyframe
	//could I call the clipUpdate function or something here to update the keyframe and other vals?

	//case 4: forward into the end of the clip
	//so here I either need to tell it to loop from beginning or go back through in reverse

	//case 5: reverse in the same keyframe

	//case 6: reverse into a new keyframe

	//case 7: reverse into the end of the clip
	//so here I either need to tell it to loop from beginning or go back through forwards again

	return -1;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	clipCtrl->clipPool = clipPool;
	clipCtrl->clip = clipIndex_pool;

	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H