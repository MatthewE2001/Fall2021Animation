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

	a3_DemoMode1_Animation-idle-input.c
	Demo mode implementations: animation scene.

	********************************************
	*** INPUT FOR ANIMATION SCENE MODE       ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"

#include <stdio.h>
#include <math.h>


//-----------------------------------------------------------------------------
// CALLBACKS

// main demo mode callback
void a3animation_input_keyCharPress(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state)
{
	switch (asciiKey)
	{
		// toggle render program
		//a3demoCtrlCasesLoop(demoMode->render, animation_render_max, 'k', 'j');

		// toggle display program
		//a3demoCtrlCasesLoop(demoMode->display, animation_display_max, 'K', 'J');

		// toggle active camera
		//a3demoCtrlCasesLoop(demoMode->activeCamera, animation_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, animation_pipeline_max, ']', '[');

		// toggle target
		a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');

		// toggle pass to display
		a3demoCtrlCasesLoop(demoMode->pass, animation_pass_max, ')', '(');

		// toggle control target
		a3demoCtrlCasesLoop(demoMode->ctrl_target, animation_ctrlmode_max, '\'', ';');

		// toggle position input mode
		a3demoCtrlCasesLoop(demoMode->ctrl_position, animation_inputmode_max, '=', '-');

		// toggle rotation input mode
		a3demoCtrlCasesLoop(demoMode->ctrl_rotation, animation_inputmode_max, '+', '_');

	}
}

void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state)
{
//	switch (asciiKey)
//	{
//
//	}
}


//-----------------------------------------------------------------------------

void a3demo_input_controlObject(
	a3_DemoState* demoState, a3_DemoSceneObject* object,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed);
void a3demo_input_controlProjector(
	a3_DemoState* demoState, a3_DemoProjector* projector,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed, a3real ctrlZoomSpeed);

void a3animation_input(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3_DemoProjector* projector = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject* sceneObject;

	// right click to ray pick
	if (a3mouseGetState(demoState->mouse, a3mouse_right) == a3input_down)
	{
		// get window coordinates
		a3i32 const x = a3mouseGetX(demoState->mouse) + demoState->frameBorder;
		a3i32 const y = a3mouseGetY(demoState->mouse) + demoState->frameBorder;

		// transform to NDC
		a3vec4 coord = a3vec4_one;
		coord.x = +((a3real)x * demoState->frameWidthInv * a3real_two - a3real_one);
		coord.y = -((a3real)y * demoState->frameHeightInv * a3real_two - a3real_one);
		coord.z = -a3real_one;

		// transform to view space
		a3real4Real4x4Mul(projector->projectionMatInv.m, coord.v);
		a3real4DivS(coord.v, coord.w);

		// transform to world space
		a3real4Real4x4Mul(projector->sceneObject->modelMat.m, coord.v);
	}
	
	// Lines below are temporarily commented out to avoid build errors
	// a3mat4 worldTJoint; //for procedural grabbing
	// a3mat4 lookAtMatrix; //for procedural looking
	//a3mat4 grabPoint; //need something to grab towards
	//a3real4x4SetMajors(grabPoint.m);

	// choose control target
	switch (demoMode->ctrl_target)
	{
	case animation_ctrl_camera:
		// move camera
		a3demo_input_controlProjector(demoState, projector,
			dt, projector->ctrlMoveSpeed, projector->ctrlRotateSpeed, projector->ctrlZoomSpeed);
		break;
	//A lot of stuff here that needs to get done
	
	case animation_ctrl_neckLookat:
		//need a look at matrix for the neck transformation
		//ik pipeline done to bring joint back to a delta pose
			//set the transform to a point for character to look at?

		if (a3XboxControlIsConnected(demoState->xcontrol))
		{
			a3f64 lJoystick[2], rJoystick[2];
			a3XboxControlGetJoysticks(demoState->xcontrol, lJoystick, rJoystick);

			switch (demoMode->ctrl_position)
			{
				// Direct value
			case animation_input_direct:
			{
				// The 2D vector from your locomotion input(WASD or joystick) directly maps to the character's
				// position in world-space (AD or horizontal tilt for WS or vertical tilt for Y axis).
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.x += ((a3real)lJoystick[0] * a3real_pi);
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.y += ((a3real)lJoystick[1] * a3real_pi);
				break;
			}
			// Control Velocity
			case animation_input_euler:
			{
				// The 2D vector from your locomotion input directly maps to the character's velocity in world space.
				// Integrate this into position using Euler's method.
				a3real direction = 0.0;

				if (a3keyboardGetState(demoState->keyboard, a3key_J))
				{
					direction -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_L))
				{
					direction += 1.0;
				}

				// Velocity coming from JL key input
				a3real dxdt = (a3real)direction;
				// Time change (locked to 1.0)
				a3real dt = 1.0;
				// Euler method integration and assignment
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->euler.z = a3SpatialPoseIntegrateEuler(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z,
					dxdt, dt);

				break;
			}
			// Control Acceleration
			case animation_input_kinematic:
			{
				// The 2D vector from your locomotion input directly maps to the character's acceleration in world space.
				// Integrate both the current velocity and this acceleration into position using kinematic integration,
				// then integrate acceleration into velocity using Euler's method.
				break;
			}
			// Fake Velocity
			case animation_input_interpolate1:
			{
				// The 2D vector from your locomotion input directly maps to the character's target
				// position in world space.  Integrate using interpolation.
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.x = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.x,
					a3real_pi, (a3real)lJoystick[0]);
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.y = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.y,
					a3real_pi, (a3real)lJoystick[1]);

				break;
			}
			// Fake Acceleration
			case animation_input_interpolate2:
			{
				// The 2D vector from your locomotion input directly maps to the character's target
				// velocity in world space.  Integrate the current velocity into position using Euler's
				// method, then integrate velocity using interpolation.
				break;
			}
			}
		}
		else
		{
			switch (demoMode->ctrl_position)
			{
				// Direct assignment from keyboard input
			case animation_input_direct:
			{

				// Speed multiplier
				a3real multiplier = 2.0;

				// Get the position by normalizing the WASD inputs (if possible)
				a3real2 direction = { 0.0, 0.0 };

				if (a3keyboardGetState(demoState->keyboard, a3key_W))
				{
					direction[1] += 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_S))
				{
					direction[1] -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_A))
				{
					direction[0] -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_D))
				{
					direction[0] += 1.0;
				}

				// Checking to avoid a divide by zero error during normalization
				if (direction[0] == 0.0) {}
				else if (direction[1] == 0.0) {}
				else { a3real2Normalize(direction); }

				// Assign the normalized values to the character position
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.x += ((a3real)direction[0] * multiplier);
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.y += ((a3real)direction[1] * multiplier);

				break;
			}
			case animation_input_euler:
				break;

			case animation_input_kinematic:
				break;

				// Fake Velocity
			case animation_input_interpolate1:
			{
				// Get the position by normalizing the WASD inputs (if possible)

				a3real2 direction = { 0.0, 0.0 };

				if (a3keyboardGetState(demoState->keyboard, a3key_W))
				{
					direction[1] += 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_S))
				{
					direction[1] -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_A))
				{
					direction[0] -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_D))
				{
					direction[0] += 1;
				}

				// Checking to avoid a divide by zero error during normalization
				if (direction[0] == 0.0) {}
				else if (direction[1] == 0.0) {}
				else { a3real2Normalize(direction); }

				// Set the position based on the normalized vector
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.x += ((a3real)direction[0] * a3real_pi);
				demoState->demoMode1_animation->obj_skeleton_neckLookat_ctrl->position.y += ((a3real)direction[1] * a3real_pi);
				break;

			}
			case animation_input_interpolate2:
				break;
			}
		}

	case animation_ctrl_wristEffector_r: //calculate magnitude of vector between two bones to find length between bones
		//set target locator in the world
		if (a3XboxControlIsConnected(demoState->xcontrol))
		{
			a3f64 lJoystick[2], rJoystick[2];
			a3XboxControlGetJoysticks(demoState->xcontrol, lJoystick, rJoystick);

			switch (demoMode->ctrl_position)
			{
				// Direct value
			case animation_input_direct:
			{
				// The 2D vector from your locomotion input(WASD or joystick) directly maps to the character's
				// position in world-space (AD or horizontal tilt for WS or vertical tilt for Y axis).
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.x += ((a3real)lJoystick[0] * a3real_pi);
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.y += ((a3real)lJoystick[1] * a3real_pi);
				break;
			}
			// Control Velocity
			case animation_input_euler:
			{
				// The 2D vector from your locomotion input directly maps to the character's velocity in world space.
				// Integrate this into position using Euler's method.

				break;
			}
			// Control Acceleration
			case animation_input_kinematic:
			{
				// The 2D vector from your locomotion input directly maps to the character's acceleration in world space.
				// Integrate both the current velocity and this acceleration into position using kinematic integration,
				// then integrate acceleration into velocity using Euler's method.
				break;
			}
			// Fake Velocity
			case animation_input_interpolate1:
			{
				// The 2D vector from your locomotion input directly maps to the character's target
				// position in world space.  Integrate using interpolation.
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.x = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.x,
					a3real_pi, (a3real)lJoystick[0]);
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.y = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.y,
					a3real_pi, (a3real)lJoystick[1]);

				break;
			}
			// Fake Acceleration
			case animation_input_interpolate2:
			{
				// The 2D vector from your locomotion input directly maps to the character's target
				// velocity in world space.  Integrate the current velocity into position using Euler's
				// method, then integrate velocity using interpolation.
				break;
			}
			}
		}
		else
		{
			switch (demoMode->ctrl_position)
			{
				// Direct assignment from keyboard input
			case animation_input_direct:
			{

				// Speed multiplier
				a3real multiplier = 2.0;

				// Get the position by normalizing the WASD inputs (if possible)
				a3real2 direction = { 0.0, 0.0 };

				if (a3keyboardGetState(demoState->keyboard, a3key_W))
				{
					direction[1] += 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_S))
				{
					direction[1] -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_A))
				{
					direction[0] -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_D))
				{
					direction[0] += 1.0;
				}

				// Checking to avoid a divide by zero error during normalization
				if (direction[0] == 0.0) {}
				else if (direction[1] == 0.0) {}
				else { a3real2Normalize(direction); }

				// Assign the normalized values to the character position
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.x += ((a3real)direction[0] * multiplier);
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.y += ((a3real)direction[1] * multiplier);

				break;
			}
			case animation_input_euler:
				break;

			case animation_input_kinematic:
				break;

				// Fake Velocity
			case animation_input_interpolate1:
			{
				// Get the position by normalizing the WASD inputs (if possible)

				a3real2 direction = { 0.0, 0.0 };

				if (a3keyboardGetState(demoState->keyboard, a3key_W))
				{
					direction[1] += 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_S))
				{
					direction[1] -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_A))
				{
					direction[0] -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_D))
				{
					direction[0] += 1;
				}

				// Checking to avoid a divide by zero error during normalization
				if (direction[0] == 0.0) {}
				else if (direction[1] == 0.0) {}
				else { a3real2Normalize(direction); }

				// Set the position based on the normalized vector
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.x += ((a3real)direction[0] * a3real_pi);
				demoState->demoMode1_animation->obj_skeleton_wristEffector_r_ctrl->position.y += ((a3real)direction[1] * a3real_pi);
				break;

			}
			case animation_input_interpolate2:
				break;
			}
		}
		//character grabs it with chain IK solver
		//1) Solve Positions
			// Find vector from base to the end/grab effector
		a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "skel:shoulder_r");
		a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "skel:elbow_r");
		a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "skel:forearm_r");
		a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "skel:wrist_r");

		
		//a3real4Length() //need to get the joint vector for this
			// Distance check (to make sure arm does not extend longer than the arm has length)
			// Also need a constraint locator (cause plane has 3 points)
			//p-> = p0-> + Dd^ + Hh^ (arrows should be above p/p0 and ^ is adove d/h)
		//2) Solve Rotations
			// know normal basis from solving positions and we know tangent of each bone
			// Frenet-Serret frame (assemble global matrix from three basis vectors in common space)
			// world T joint (4x4) = [t^ b^ n^ p->	0 0 0 1]
		//3) Solve Local
			// Apply IK formula for affected joint chain only (rest of skeleton already solved)
		//a3kinematicsSolveInverseSingle(); //single to keep it to the affected joint only
	
	case animation_ctrl_wristConstraint_r:
		sceneObject = demoMode->obj_skeleton_ctrl + demoMode->ctrl_target - animation_ctrl_character;
		a3demo_input_controlObject(demoState, sceneObject, dt, a3real_one, a3real_zero);
	
	case animation_ctrl_character:
		//what needs to happen in this spot
		if (a3XboxControlIsConnected(demoState->xcontrol))
		{
			// Right joystick controls rotation of the character, left controls movement
			// These values get updated continuously
			a3f64 lJoystick[2], rJoystick[2];
			a3XboxControlGetJoysticks(demoState->xcontrol, lJoystick, rJoystick);

			// Update the rotation of the character node
			switch (demoMode->ctrl_rotation)
			{
				// Direct value
			case animation_input_direct:
			{
				// Old ethod using atan2 to approximate joystick rotation
				//a3real angle_raw = demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z - a3atan2d((a3real)rJoystick[0], (a3real)rJoystick[1]);

				// New method using direct input from the horizontal axis of the joystick
				// Multiplier for speed
				a3real multiplier = 2.0;
				// Raw angle based on current angle and horizontal joystick input
				a3real angle_raw = demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z - ((a3real)rJoystick[0] * multiplier);
				// Angle correction and assignment
				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z = (a3real)fmod(angle_raw, (a3real)360.0);
				

				break;
			}
			// Control Velocity
			case animation_input_euler:
			{
				// Velocity coming from horizontal joystick input
				a3real dxdt = (a3real)rJoystick[0];
				// Time change
				a3real dt = (a3real)demoState->dt_timer;
				// Euler method integration and assignment
				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z = a3SpatialPoseIntegrateEuler(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z,
					dxdt, dt);

				break;
			}
			// Control Acceleration
			case animation_input_kinematic:
			{
				// The horizontal axis of your orientation input directly maps to the character's angular
				// acceleration about the world's "up" axis.  Integrate both the current angular velocity
				// and this angular acceleration into rotation using kinematic integration, then integrate
				// angular acceleration into angular velocity using Euler's method.

				//printf("%f\n", demoState->dt_timer);

				// Initial velocity coming from horizontal joystick input
				a3real dxdt = (a3real)rJoystick[0];
				a3real dxdt2 = (a3real)(rJoystick[0]); //d2x / dt2
				printf("dxdt: %f\n", dxdt);
				printf("dxdt2: %f\n", dxdt2);
				// Time change
				a3real dt = (a3real)demoState->dt_timer;
				// Euler method integration and assignment
				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z = a3SpatialPoseIntegrateKinematic(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z,
					dxdt, dxdt2, dt);


				break;
			}
			// Fake velocity
			case animation_input_interpolate1:
			{
				// The horizontal axis of your orientation input directly maps to the character's target
				// rotation about the world's "up" axis.  Integrate using interpolation.
				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z,
					(a3real)180.0, (a3real)rJoystick[0]);
				break;
			}
			// Fake Acceleration
			case animation_input_interpolate2:
			{
				// The horizontal axis of your orientation input directly maps to the character's target
				// angular velocity about the world's "up" axis.  Integrate the current angular velocity
				// into rotation using Euler's method, then integrate angular velocity using interpolation.
				break;
			}
			}

			// Update the position of the character
			switch (demoMode->ctrl_position)
			{
				// Direct value
			case animation_input_direct:
			{
				// The 2D vector from your locomotion input(WASD or joystick) directly maps to the character's
				// position in world-space (AD or horizontal tilt for WS or vertical tilt for Y axis).
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.x += ((a3real)lJoystick[0] * a3real_pi);
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.y += ((a3real)lJoystick[1] * a3real_pi);
				break;
			}
			// Control Velocity
			case animation_input_euler:
			{
				// The 2D vector from your locomotion input directly maps to the character's velocity in world space.
				// Integrate this into position using Euler's method.

				break;
			}
			// Control Acceleration
			case animation_input_kinematic:
			{
				// The 2D vector from your locomotion input directly maps to the character's acceleration in world space.
				// Integrate both the current velocity and this acceleration into position using kinematic integration,
				// then integrate acceleration into velocity using Euler's method.

				break;
			}
			// Fake Velocity
			case animation_input_interpolate1:
			{
				// The 2D vector from your locomotion input directly maps to the character's target
				// position in world space.  Integrate using interpolation.
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.x = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_ctrl->position.x,
					a3real_pi, (a3real)lJoystick[0]);
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.y = a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_ctrl->position.y,
					a3real_pi, (a3real)lJoystick[1]);

				break;
			}
			// Fake Acceleration
			case animation_input_interpolate2:
			{
				// The 2D vector from your locomotion input directly maps to the character's target
				// velocity in world space.  Integrate the current velocity into position using Euler's
				// method, then integrate velocity using interpolation.
				break;
			}
			}


		}
		else
		{
			// Update rotation of the character
			switch (demoMode->ctrl_rotation)
			{
			case animation_input_direct:
			{
				// Get the rotation from JL input
				a3real direction = 0.0;

				if (a3keyboardGetState(demoState->keyboard, a3key_J))
				{
					direction -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_L))
				{
					direction += 1.0;
				}

				// Multiplier for speed
				a3real multiplier = 2.0;
				// Raw angle based on current angle and horizontal joystick input
				a3real angle_raw = demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z - (direction * multiplier);
				// Angle correction and assignment
				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z = (a3real)fmod(angle_raw, (a3real)360.0);


				break;
			}
			case animation_input_euler:
			{
				// Get the rotation from JL input
				a3real direction = 0.0;

				if (a3keyboardGetState(demoState->keyboard, a3key_J))
				{
					direction -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_L))
				{
					direction += 1.0;
				}

				// Velocity coming from JL key input
				a3real dxdt = (a3real)direction;
				// Time change (locked to 1.0)
				a3real dt = 1.0;
				// Euler method integration and assignment
				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z = a3SpatialPoseIntegrateEuler(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z,
					dxdt, dt);

				break;
			}
			case animation_input_kinematic:
			{
				a3real directionXKin = 0.0;
				a3real directionYKin = 0.0;

				if (a3keyboardGetState(demoState->keyboard, a3key_I))
				{
					directionYKin += 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_K))
				{
					directionYKin -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_J))
				{
					directionXKin -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_L))
				{
					directionXKin += 1;
				}

				// Checking to avoid a divide by zero error during normalization
				if (directionXKin == 0.0) {}
				else if (directionYKin == 0.0) {}

				demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z =
					a3SpatialPoseIntegrateKinematic(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z,
						directionXKin, directionYKin, (a3real)demoState->dt_timer);
				//have some values stored for position, velocity, angular acceleration, and others
				//then we can assign them to what they should be at the end of the process

				break;
			}
			case animation_input_interpolate1:
			{
				a3real directionXLerp = 0.0;
				a3real directionYLerp = 0.0;

				if (a3keyboardGetState(demoState->keyboard, a3key_I))
				{
					directionYLerp += 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_K))
				{
					directionYLerp -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_J))
				{
					directionXLerp -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_L))
				{
					directionXLerp += 1;
				}

				// Checking to avoid a divide by zero error during normalization
				if (directionXLerp == 0.0) {}
				else if (directionYLerp == 0.0) {}

				a3SpatialPoseIntegrateLerp(demoState->demoMode1_animation->obj_skeleton_ctrl->euler.z, a3atan2d(directionXLerp, directionYLerp), (a3real)demoState->dt_timer);

				break;
			}
			case animation_input_interpolate2:
				break;
			}

			// Update position of the character
			switch (demoMode->ctrl_position)
			{
				// Direct assignment from keyboard input
			case animation_input_direct:
			{

				// Speed multiplier
				a3real multiplier = 2.0;

				// Get the position by normalizing the WASD inputs (if possible)
				a3real2 direction = { 0.0, 0.0 };

				if (a3keyboardGetState(demoState->keyboard, a3key_W))
				{
					direction[1] += 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_S))
				{
					direction[1] -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_A))
				{
					direction[0] -= 1.0;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_D))
				{
					direction[0] += 1.0;
				}

				// Checking to avoid a divide by zero error during normalization
				if (direction[0] == 0.0) {}
				else if (direction[1] == 0.0) {}
				else { a3real2Normalize(direction); }

				// Assign the normalized values to the character position
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.x += ((a3real)direction[0] * multiplier);
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.y += ((a3real)direction[1] * multiplier);

				break;
			}
			case animation_input_euler:
				break;

			case animation_input_kinematic:
				break;

				// Fake Velocity
			case animation_input_interpolate1:
			{
				// Get the position by normalizing the WASD inputs (if possible)

				a3real2 direction = { 0.0, 0.0 };

				if (a3keyboardGetState(demoState->keyboard, a3key_W))
				{
					direction[1] += 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_S))
				{
					direction[1] -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_A))
				{
					direction[0] -= 1;
				}
				if (a3keyboardGetState(demoState->keyboard, a3key_D))
				{
					direction[0] += 1;
				}

				// Checking to avoid a divide by zero error during normalization
				if (direction[0] == 0.0) {}
				else if (direction[1] == 0.0) {}
				else { a3real2Normalize(direction); }

				// Set the position based on the normalized vector
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.x += ((a3real)direction[0] * a3real_pi);
				demoState->demoMode1_animation->obj_skeleton_ctrl->position.y += ((a3real)direction[1] * a3real_pi);
				break;

			}
			case animation_input_interpolate2:
				break;
			}
		}
	break;
	}

	// allow the controller, if connected, to change control targets
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_right))
			a3demoCtrlIncLoop(demoMode->ctrl_target, animation_ctrlmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_left))
			a3demoCtrlDecLoop(demoMode->ctrl_target, animation_ctrlmode_max);

		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_B))
			a3demoCtrlIncLoop(demoMode->ctrl_position, animation_inputmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_X))
			a3demoCtrlDecLoop(demoMode->ctrl_position, animation_inputmode_max);

		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_Y))
			a3demoCtrlIncLoop(demoMode->ctrl_rotation, animation_inputmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_A))
			a3demoCtrlDecLoop(demoMode->ctrl_rotation, animation_inputmode_max);
	}
}


//-----------------------------------------------------------------------------
