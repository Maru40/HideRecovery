
#pragma once
/*!
@file UtilityStartCamera.h
@brief UtilityStartCamera�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"
#include "Maruyama/Camera/StartCamera.h"

namespace basecross {
	
	class UtilityStartCamera
	{
		static map<wstring, vector<StartCamera::ShowParametor>> sm_startCameraParams; //�J�������o�p�����[�^�}�b�v

	public:

		/// <summary>
		/// �X�^�[�g�J�������o�p�̃p�����[�^�Q���擾����B
		/// </summary>
		/// <param name="name">�X�e�[�W�̖��O</param>
		/// <returns>�X�^�[�g�J�������o�p�̃p�����[�^�Q</returns>
		static vector<StartCamera::ShowParametor> GetStartCameraShowParams(const wstring& name) {
			if (sm_startCameraParams.count(name) == 0) {
				return vector<StartCamera::ShowParametor>();
			}

			return sm_startCameraParams.at(name);
		}

	};

}

//endbasecross