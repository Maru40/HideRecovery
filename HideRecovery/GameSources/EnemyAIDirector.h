
/*!
@file EnemyAIDirector.h
@brief EnemyAIDirector�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// �t�B�[���h�S�̂̐���
		//--------------------------------------------------------------------------------------
		class AIDirector : public maru::SingletonComponent<AIDirector>
		{
			//�S�ẴG�l�~�[
			vector<std::weak_ptr<EnemyBase>> m_enemys;

			//�O���[�v�Ǘ��̔z��
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

		public :
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnLateStart() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// �����z�u���ꂽ�G�l�~�[��S�Ď擾
			/// </summary>
			void SettingStartAllEnemys();

			/// <summary>
			/// �O���[�v�Ǘ��̐���
			/// </summary>
			/// <returns>���������h��</returns>
			std::shared_ptr<FactionCoordinator> CreateFaction();

			/// <summary>
			/// �O���[�v�Ǘ��̍폜
			/// </summary>
			/// <param name="removeCoordinator">�폜�������h��</param>
			/// <returns>�폜�ł�����true</returns>
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �G�l�~�[�̒ǉ�
			/// </summary>
			/// <param name="enemy">�G�l�~�[</param>
			void AddEnemy(const std::shared_ptr<EnemyBase>& enemy);

			/// <summary>
			/// �O���[�v�Ǘ��̎擾
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator(const int index) const;

		};

	}
}