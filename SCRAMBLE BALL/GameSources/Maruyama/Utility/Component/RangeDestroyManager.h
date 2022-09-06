/*!
@file RangeDestroyManager.h
@brief RangeDestroyManager
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	��苗����i�񂾂�폜����R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class RangeDestoryManager : public Component
	{
	private:
		float m_maxRange;          //�ő勗��
		Vec3 m_initializePosition; //�����|�W�V����

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="maxRange">�ő勗��</param>
		RangeDestoryManager(const std::shared_ptr<GameObject>& objPtr, const float& maxRange);

		void OnCreate() override;
		void OnUpdate() override;
	};

}

//basecross