
/*!
@file SelfImpactNodeManager.h
@brief SelfImpactNodeManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------

	namespace maru {

		class I_Impacter;

		struct SelfImpactNodeManager_Parametor {
			float range;	//���̋��������ꂽ��A���g�̃m�[�h���Č�������B

		};

		//--------------------------------------------------------------------------------------
		/// �e���}�b�v�̒��Ŏ������g�̃m�[�h���Ǘ�����R���|�[�l���g
		//--------------------------------------------------------------------------------------
		class SelfImpactNodeManager : public Component
		{
		public:
			using Parametor = SelfImpactNodeManager_Parametor;

		private:
			Parametor m_param;

			std::weak_ptr<I_Impacter> m_impacter;	//�e����^�����

		public:
			SelfImpactNodeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<I_Impacter>& impacter);

			void OnUpdate() override;

		private:
			/// <summary>
			/// �������g�̃m�[�h�X�V
			/// </summary>
			void SelfNodeUpdate();

			/// <summary>
			/// �������g�̃m�[�h���X�V����B
			/// </summary>
			void SettingSelfNode();

		};

	}
}