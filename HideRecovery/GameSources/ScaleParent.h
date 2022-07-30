/*!
@file ScaleParent.h
@brief ScaleParent
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�傫����ݒ肵���e�I�u�W�F�N�g�ɍ��킹��R���|�\�l���g
	//--------------------------------------------------------------------------------------
	class ScaleParent : public Component
	{
		ex_weak_ptr<GameObject> m_parent;	//�y�A�����g
		Vec3 m_beforeScale = Vec3(0.0f);	//1�t���[���O�̑傫��
		Vec3 m_firstScale = Vec3(0.0f);		//�����̑傫��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parent">�e�I�u�W�F�N�g</param>
		ScaleParent(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& parent);

		void OnCreate() override;
		void OnUpdate() override;
	};

}