/*!
@file BigSlime_Dyning.h
@brief BigSlime_Dyning�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	template<class T>
	class TaskList;

	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// ����X���C���̎��S���p�����[�^
			//--------------------------------------------------------------------------------------
			struct BigSlime_Dyning_Parametor
			{
				float time;          //�ׂ�����
				float pressPower;    //�ׂ���
				PressData pressData; //�ׂ��f�[�^

				BigSlime_Dyning_Parametor();
				BigSlime_Dyning_Parametor(const float& time, const float& pressPower, const PressData& pressData);
			};

			//--------------------------------------------------------------------------------------
			///	����X���C���̎��S���X�e�[�g
			//--------------------------------------------------------------------------------------
			class BigSlime_Dyning : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = BigSlime_Dyning_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;   //�p�����[�^�B
				std::unique_ptr<GameTimer> m_timer = nullptr;  //���ԊǗ��N���X�B

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="paramPtr">�p�����[�^</param>
				BigSlime_Dyning(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;
			};

		}
	}
}