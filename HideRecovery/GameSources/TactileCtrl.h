/*!
@file TactileCtrl.h
@brief TactileCtrl
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�G�o�R���g���[��
	//--------------------------------------------------------------------------------------
	class TactileCtrl : public Component
	{
		//Update�ŌĂяo���֐��|�C���^�̌^
		using ActionFunc = std::function<void(const std::shared_ptr<TactileCtrl>& tactile, const std::shared_ptr<GameObject>& other)>;

		vector<ActionFunc> m_excuteActions; //�ՓˍX�V���ɌĂяo����������

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		TactileCtrl(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}

		void OnCollisionExcute(std::shared_ptr<GameObject>& other) override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �A�b�v�f�[�g�ŌĂяo���֐��|�C���^���Z�b�g����
		/// </summary>
		/// <param name="excuteAction">�A�b�v�f�[�g�ŌĂяo���֐��|�C���^</param>
		void AddExcuteAction(const ActionFunc& excuteAction) {
			m_excuteActions.push_back(excuteAction);
		}
	};

}

//endbasecross