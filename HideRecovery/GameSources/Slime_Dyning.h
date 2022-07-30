/*!
@file Slime_Dyning.h
@brief Slime_Dyning�Ȃ�
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
			/// �X���C�����S�J�ڃX�e�[�g�̃p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_Dyning_Parametor
			{
				float time;          //���S����܂ł̎���
				float pressPower;    //�ׂ���
				PressData pressData; //PressData

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				Slime_Dyning_Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="time">���S����܂ł̎���</param>
				/// <param name="pressPower">�Ԃ�����</param>
				/// <param name="pressData">�Ԃ����f�[�^</param>
				Slime_Dyning_Parametor(const float& time, const float& pressPower, const PressData& pressData);
			};

			//--------------------------------------------------------------------------------------
			/// �X���C�����S�J�ڃX�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Dyning : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Dyning_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;   //�p�����[�^

				std::unique_ptr<GameTimer> m_timer = nullptr;  //�^�C���Ǘ��N���X

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Slime_Dyning(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// SuccessEffect�𐶐�
				/// </summary>
				void CreateSuccessEffect();
			};

		}
	}
}
