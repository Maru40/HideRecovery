
/*!
@file SoleImpactMap.h
@brief SoleImpactMap�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class ImpactMap;
		class I_Impacter;

		//--------------------------------------------------------------------------------------
		///	�p�����[�^
		//--------------------------------------------------------------------------------------
		struct SoleImpactMap_Parametor 
		{
			Factory_ImpactMap_Parametor factoryParam;  //�e���}�b�v�����p�p�����[�^
		};

		//--------------------------------------------------------------------------------------
		///	�����𒆐S�ɉe���}�b�v��W�J����R���|�[�l���g
		//--------------------------------------------------------------------------------------
		class SoleImpactMap : public Component
		{
		public:
			using Parametor = SoleImpactMap_Parametor;

		private:
			Parametor m_param;						//�p�����[�^

			std::shared_ptr<ImpactMap> m_impactMap;	//�e���}�b�v

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			SoleImpactMap(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			SoleImpactMap(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// �e���}�b�v�̐���
			/// </summary>
			void CreateImpactMap();

		public:
			/// <summary>
			/// �e���x���v�Z���ĕԂ��B
			/// </summary>
			/// <param name = "impacter">�e����^�����</param>
			void CalculateImpact(const std::shared_ptr<I_Impacter>& impacter);

		};

	}
}