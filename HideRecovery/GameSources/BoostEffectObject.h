/*!
@file BoostEffectObject.h
@brief BoostEffectObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BoostCenterObject;

	//--------------------------------------------------------------------------------------
	/// �u�[�X�g�G�t�F�N�g�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class BoostEffectObject : public GameObject
	{
	private:
		ex_weak_ptr<BoostCenterObject> m_center;        //�u�[�X�g�̒��S�I�u�W�F�N�g

		std::vector<ex_weak_ptr<GameObject>> m_objects; //���������G�t�F�N�g�I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		BoostEffectObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
		void OnUpdate() override;

		void OnDestroy() override;

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���������G�t�F�N�g�I�u�W�F�N�g�̔z����擾
		/// </summary>
		/// <returns>���������G�t�F�N�g�I�u�W�F�N�g�̔z��</returns>
		std::vector<ex_weak_ptr<GameObject>> GetEffectObjects() const noexcept {
			return m_objects;
		}

	};

}