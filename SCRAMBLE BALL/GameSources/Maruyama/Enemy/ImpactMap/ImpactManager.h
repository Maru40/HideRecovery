
/*!
@file ImpactManager.h
@brief ImpactManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_Impact.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	�e���}�b�v�̉e���x�Ǘ�
		//--------------------------------------------------------------------------------------
		class ImpactManager
		{
		private:
			/// <summary>
			/// �m�[�h�̃A�N�e�B�u��Ԃ��X�V
			/// </summary>
			/// <param name="impactMap">�e���}�b�v</param>
			/// <param name="impacter">�e����^�����</param>
			void NodeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

			/// <summary>
			/// �G�b�W�̃A�N�e�B�u��Ԃ��X�V
			/// </summary>
			/// <param name="impactMap">�e���}�b�v</param>
			/// <param name="impacter">�e����^�����</param>
			void EdgeActiveUpdate(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

		public:

			/// <summary>
			/// �����̎��͂ɉe���x���X�V����B
			/// </summary>
			/// <param name="impactMap">�e���}�b�v</param>
			/// <param name="impacter">�e����^�����</param>
			void CalculateAroundImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

			/// <summary>
			/// ���E�͈͂̉e���x���X�V����
			/// </summary>
			/// <param name="impactMap">�e���}�b�v</param>
			/// <param name="impacter">�e����^�����</param>
			void CalculateEyeImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

		public:

			/// <summary>
			/// �e���x���v�Z����B
			/// </summary>
			/// <param name = "impactMap">�e���}�b�v</param>
			/// <param name = "impacter">�e����^�����</param>
			void CalculateAllImpact(const std::shared_ptr<ImpactMap>& impactMap, const std::shared_ptr<I_Impacter>& impacter);

		};

	}
}

//basecross