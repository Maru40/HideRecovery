/*!
@file Slime_Find.h
@brief Slime_Find�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class TargetManager;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �X���C���̓G�����������̃X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Find : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				Slime_Find(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override {};
				bool OnUpdate() override { return true; };
				void OnExit() override {};
 			};

		}
	}
}
