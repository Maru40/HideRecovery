
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

		//--------------------------------------------------------------------------------------
		/// �e���}�b�v�̒��Ŏ������g�̃m�[�h���Ǘ�����R���|�[�l���g�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct SelfImpactNodeManager_Parametor {
			float range;		//���̋��������ꂽ��A���g�̃m�[�h���Č�������B
			float muchFarRange;	//���݂̃m�[�h��藣�ꂷ�����ꍇ�ɍČ�������m�[�h

			SelfImpactNodeManager_Parametor();
		};

		//--------------------------------------------------------------------------------------
		/// �e���}�b�v�̒��Ŏ������g�̃m�[�h���Ǘ�����R���|�[�l���g
		//--------------------------------------------------------------------------------------
		class SelfImpactNodeManager : public Component
		{
		public:
			using Parametor = SelfImpactNodeManager_Parametor;

		private:
			Parametor m_param;						//�p�����[�^

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

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �����̃m�[�h���Č������鋗����ݒ�
			/// </summary>
			/// <param name="range">�����̃m�[�h���Č������鋗��</param>
			void SetRange(const float range) noexcept { m_param.range = range; }

			/// <summary>
			/// �����̃m�[�h���Č������鋗�����擾
			/// </summary>
			/// <returns>�����̃m�[�h���Č������鋗��</returns>
			float GetRange() const noexcept { return m_param.range; }
		};

	}
}