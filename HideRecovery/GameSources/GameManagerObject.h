/*!
@file GameManagerObject.h
@brief GameManagerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SlowTimerManager;

	//--------------------------------------------------------------------------------------
	/// �Q�[���I�u�W�F�N�g�}�l�[�W���[
	//--------------------------------------------------------------------------------------
	class GameManagerObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		GameManagerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}

//endbasecross