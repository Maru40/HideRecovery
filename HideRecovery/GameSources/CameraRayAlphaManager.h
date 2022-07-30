/*!
@file CameraRayAlphaManager.h
@brief CameraRayAlphaManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�J���������Ray�ŃI�u�W�F�N�g��Alpha��ύX����R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CameraRayAlphaManager : public Component
	{
		ex_weak_ptr<GameObject> m_target;   //�^�[�Q�b�g
		float m_range;						//�������͈�

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <param name="range">�������͈�</param>
		CameraRayAlphaManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const float& range = 4.0f);

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// �^�[�Q�b�g�̐ݒ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) {
			m_target = target;
		}
	};

}

//endbasecross