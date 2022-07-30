/*!
@file WaterCollisionManager.h
@brief ���̏Փ˔���
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------
	/// �W�F�b�g�|���v�̏Փ˔���R���|�[�l���g�N���X
	//------------------------------------------------------------------------------------------------
	class WaterCollisionManager : public Component
	{
	public:
		//-----------------------------------------------------------------------------------------------
		/// �R���X�g���N�^
		WaterCollisionManager(const std::shared_ptr<GameObject>& ptrObj)
			:Component(ptrObj)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCollisionEnter(const CollisionPair& pair) override;
		void OnCollisionExcute(const CollisionPair& pair) override;
		//-----------------------------------------------------------------------------------------------
	};
}