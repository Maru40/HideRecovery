/*!
@file Slime_Split.h
@brief Slime_Split�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class Pusher;
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �X���C���̕���p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_Split_Parametor 
			{
				PressData pressData;
			};

			//--------------------------------------------------------------------------------------
			/// �X���C���̕���X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Split : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Split_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;  //�p�����[�^

				std::unique_ptr<GameTimer> m_timer = nullptr; //�^�C���Ǘ��N���X

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				Slime_Split(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// �����ʒu�z����v�Z���ĕԂ�
				/// </summary>
				/// <returns>�����ʒu�z��</returns>
				std::vector<Vec3> CalculateCreatePositions();

				/// <summary>
				/// �X���C���𐶐�����
				/// </summary>
				/// <param name="position">�����ʒu</param>
				/// <param name="forward">��������</param>
				/// <returns>���������X���C��</returns>
				std::shared_ptr<GameObject> CreateSlime(const Vec3& position, const Vec3& forward);
			};

		}
	}
}