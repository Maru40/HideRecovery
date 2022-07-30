/*!
@file Slime_Pressed.h
@brief Slime_Pressed�Ȃ�
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
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �X���C���̂Ԃ���鎞�̃p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_Pressed_Parametor {
				std::function<void()> m_startAnimation = nullptr;
				std::function<void()> m_endAnimation = nullptr;
			};

			//--------------------------------------------------------------------------------------
			/// �X���C���̂Ԃ���鎞�̃X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Pressed : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Pressed_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;  //�p�����[�^
				std::unique_ptr<GameTimer> m_timer = nullptr; //�^�C���Ǘ�

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Slime_Pressed(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;
			};

		}
	}
}