
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
			vector<std::weak_ptr<EnemyBase>> m_enemys;

			//�h���̔z��
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

		public :
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnLateStart() override;

		private:
			/// <summary>
			/// �G�l�~�[��S�Ď擾
			/// </summary>
			void SetEnemys();

			/// <summary>
			/// �h���̒ǉ�
			/// </summary>
			/// <returns>���������h��</returns>
			std::shared_ptr<FactionCoordinator> AddFaction();

			/// <summary>
			/// �h���̍폜
			/// </summary>
			/// <param name="removeCoordinator">�폜�������h��</param>
			/// <returns>�폜�ł�����true</returns>
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

		public:
			//�A�N�Z�b�T-------------------------------------------------------------------------------------



		};

	}
}