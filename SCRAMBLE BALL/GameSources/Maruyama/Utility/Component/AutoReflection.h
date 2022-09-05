/*!
@file AutoReflection.h
@brief AutoReflection
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	///	��������
	//--------------------------------------------------------------------------------------
	class AutoReflection : public Component
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		AutoReflection(const std::shared_ptr<GameObject>& objPtr);

		void OnCollisionEnter(const CollisionPair& pair) override;
	};

}