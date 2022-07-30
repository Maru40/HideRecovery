
/*!
@file I_Impact.h
@brief I_Impact�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�e���͈̓^�C�v
		//--------------------------------------------------------------------------------------
		enum class ImpactType {
			Circle,  //�~�͈�
			Eye,     //���E�͈�
		};

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^
		//--------------------------------------------------------------------------------------
		struct ImpactData 
		{
			float value = 0.0f;               //�e���x
			float circleRange = 0.0f;         //�~�̉e���͈�
			ImpactType type = ImpactType(0);  //�e���͈̓^�C�v(���E�����݂���Ȃ�A���̕����Z����B)
		};

		//--------------------------------------------------------------------------------------
		///	�e���C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_Impacter
		{
			ImpactData m_impactData; //�e���f�[�^

		public:

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �e���f�[�^�̃Z�b�g
			/// </summary>
			/// <param name="data">�e���f�[�^</param>
			ImpactData SetImpactData(const ImpactData& data) noexcept { m_impactData = data; }

			/// <summary>
			/// �e���f�[�^�̎擾
			/// </summary>
			/// <returns>�e���f�[�^</returns>
			ImpactData GetImpactData() const noexcept { return m_impactData; }

			/// <summary>
			/// �e���f�[�^�̎Q�Ƃ��擾
			/// </summary>
			/// <returns></returns>
			ImpactData& GetRefImpactData() noexcept { return m_impactData; }

			/// <summary>
			/// ���̃N���X�����L����Q�[���I�u�W�F�N�g���擾����
			/// </summary>
			/// <returns>���̃N���X�����L����Q�[���I�u�W�F�N�g</returns>
			virtual std::shared_ptr<GameObject> GetImpactObject() const noexcept = 0;

			/// <summary>
			/// ���E�͈͊Ǘ��N���X���擾
			/// </summary>
			/// <returns>���E�͈͊Ǘ��N���X</returns>
			virtual std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const = 0;

		};

	}
}