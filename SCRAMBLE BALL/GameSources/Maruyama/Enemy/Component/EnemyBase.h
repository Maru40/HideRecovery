
/*!
@file EnemyBase.h
@brief EnemyBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_FactionMember.h"

namespace basecross {

	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		///	�G�l�~�[�̊��N���X
		//--------------------------------------------------------------------------------------
		class EnemyBase : public Component, public I_FactionMember
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			EnemyBase(const std::shared_ptr<GameObject>& objPtr)
				:Component(objPtr)
			{}
		};
	}
}

//endbasecross