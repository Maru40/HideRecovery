
/*!
@file Slime_Union.h
@brief Slime_Union�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �X���C���̍��̃p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_Union_Parametor
			{
				float time = 1.0f;  //Union�Ɋ|���鎞��
			};

			//--------------------------------------------------------------------------------------
			/// �X���C���̍��̃X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Union : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Union_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;   //�p�����[�^

				std::unique_ptr<GameTimer> m_timer;  //�^�C���Ǘ��N���X
				
				Vec3 m_startPosition;                //�X�e�[�g�J�n�ʒu
				
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Slime_Union(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// �^�[�Q�b�g�Ɏ����̃��^�{�[�������n����B
				/// </summary>
				void TranserChildren();

				/// <summary>
				/// ���̃A�b�v�f�[�g
				/// </summary>
				void UpdateUnion();

				/// <summary>
				/// ���̐�������
				/// </summary>
				void SccessUnion();

				/// <summary>
				/// ���̎��s����
				/// </summary>
				void MissUnion();
			};

		}
	}

}