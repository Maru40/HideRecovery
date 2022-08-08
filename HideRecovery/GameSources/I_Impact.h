
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
		enum class ImpacterType {
			Circle,  //�~�͈�
			Eye,     //���E�͈�
		};

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^
		//--------------------------------------------------------------------------------------
		struct ImpactData {
			float dangerValue;				//�댯�x
			float confirmationIntervalTime;	//�Ċm�F�܂ł̃C���^�[�o��
			bool isConfirmation;			//�m�F�ς݂��ǂ���

			ImpactData();
		};

		//--------------------------------------------------------------------------------------
		///	�e����^����҂̃f�[�^
		//--------------------------------------------------------------------------------------
		struct ImpacterData 
		{
			float value = 0.0f;					  //�e���x
			float circleRange = 0.0f;			  //�~�̉e���͈�
			ImpacterType type = ImpacterType(0);  //�e���͈̓^�C�v(���E�����݂���Ȃ�A���̕����Z����B)
		};

		//--------------------------------------------------------------------------------------
		///	�e����^����҂̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_Impacter
		{
			ImpacterData m_impacterData; //�e����^����҂̃f�[�^

		public:

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �e����^����҂̃f�[�^��ݒ�
			/// </summary>
			/// <param name="data">�e���f�[�^</param>
			ImpacterData SetImpacterData(const ImpacterData& data) noexcept { m_impacterData = data; }

			/// <summary>
			/// �e����^����҂̃f�[�^���擾
			/// </summary>
			/// <returns>�e���f�[�^</returns>
			ImpacterData GetImpacterData() const noexcept { return m_impacterData; }

			/// <summary>
			/// �e����^����҂̃f�[�^�̎Q�Ƃ��擾
			/// </summary>
			/// <returns></returns>
			ImpacterData& GetRefImpactData() noexcept { return m_impacterData; }

			/// <summary>
			/// ���̃N���X�����L����Q�[���I�u�W�F�N�g���擾����
			/// </summary>
			/// <returns>���̃N���X�����L����Q�[���I�u�W�F�N�g</returns>
			virtual std::shared_ptr<GameObject> GetImpacterObject() const noexcept = 0;

			/// <summary>
			/// ���E�͈͊Ǘ��N���X���擾
			/// </summary>
			/// <returns>���E�͈͊Ǘ��N���X</returns>
			virtual std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const = 0;

		};

	}
}