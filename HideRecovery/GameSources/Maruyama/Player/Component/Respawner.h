
/*!
@file Respawner.h
@brief Respawner�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	/// ���X�|�[����������N���X
	//--------------------------------------------------------------------------------------
	class Respawner : public Component
	{

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		Respawner(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
	};

}