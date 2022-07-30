/*!
@file BulletBase.h
@brief BulletBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�e�ۂ̊��N���X
	//--------------------------------------------------------------------------------------
	class BulletBase : public Component
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		BulletBase(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="direct">������</param>
		virtual void Shot(const Vec3& direct) {}

		/// <summary>
		/// ���������I�������^�C�~���O�ŌĂԏ���
		/// </summary>
		virtual void ShotEnd() {}
	};

}

//endbasecross