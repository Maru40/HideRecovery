/*!
@file EnemyObjectBase.h
@brief EnemyObjectBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		/// �G�I�u�W�F�N�g�̊��N���X
		//--------------------------------------------------------------------------------------
		class EnemyObjectBase : public GameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X����������X�e�[�W</param>
			EnemyObjectBase(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		protected:

			virtual void CreateModel();
		};
	}
}

//endbasecross