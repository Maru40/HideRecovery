/*!
@file UtilityStartCamera.cpp
@brief UtilityStartCamera�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Camera/StartCamera.h"
#include "UtilityStartCamera.h"

namespace basecross {
	map<wstring, vector<StartCamera::ShowParametor>> UtilityStartCamera::sm_startCameraParams = {
		{L"Stage1.csv",
			{
				StartCamera::ShowParametor(
					CameraMoveParametor(Vec3(-21.0f, +1.0f, -21.0f),Vec3(-21.0f, +1.0f, -21.0f)),
					CameraMoveParametor(Vec3(-17.0f, +2.0f, -17.0f),Vec3(-19.0f, +2.0f, -13.0f), 1.0f),
					3.0f),

				StartCamera::ShowParametor(
					CameraMoveParametor(Vec3(-21.0f, +1.0f, -21.0f),Vec3(-21.0f, +1.0f, -21.0f)),
					CameraMoveParametor(Vec3(-19.0f, +2.0f, -13.0f),Vec3(-13.0f, +2.0f, -19.0f), 0.5f),
					4.5f)
			},
		},
		{L"StageS1_Copy.csv",
			{
				StartCamera::ShowParametor(
				CameraMoveParametor(Vec3(-21.0f, +1.0f, -21.0f),Vec3(-21.0f, +1.0f, -21.0f)),
				CameraMoveParametor(Vec3(-17.0f, +2.0f, -17.0f),Vec3(-19.0f, +2.0f, -13.0f), 1.0f),
				3.0f),
			},
		},
		{L"StageS2.csv",
			{
			//StartCamera::ShowParam(
			//	LiveMoveParam(Vec3(2.0f,0.0f,0.0f),Vec3(+4.0f,+0.0f,+0.0f)),
			//	LiveMoveParam(Vec3(0.0f,0.0f,0.0f),Vec3(-3.0f,-3.0f,-3.0f)))
		},
	},
	};
}

//endbasecross