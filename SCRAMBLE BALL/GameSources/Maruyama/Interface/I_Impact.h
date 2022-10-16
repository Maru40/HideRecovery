
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
	class NavGraphNode;

	namespace maru {
		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	�e���͈̓^�C�v
		//--------------------------------------------------------------------------------------
		enum class ImpacterType {
			Circle,  //�~�͈�
			Eye,     //���E�͈�
		};

		//--------------------------------------------------------------------------------------
		///	�����������̂̃^�C�v
		//--------------------------------------------------------------------------------------
		enum class OccupancyType {
			Empty,		//��
			Enemy,		//�G
			HideItem,	//�B���A�C�e��
		};

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^
		//--------------------------------------------------------------------------------------
		struct ImpactData {
			float dangerValue;				//�댯�x
			float occupancyValue;			//��L�l(0.0f �`�@1.0f : 0.5f���m�F�O�A0.0f���m�F�ς݁A1.0f�͔�������)
			OccupancyType occupancyType;	//��L�^�C�v
			float occupacyRecoverySpeed;	//��L�l�̉񕜎���
			int areaIndex;					//�G���A�C���f�b�N�X

			ImpactData();
			ImpactData(const int areaIndex);

			virtual ~ImpactData();
		};

		//--------------------------------------------------------------------------------------
		///	�e����^����҂̃f�[�^
		//--------------------------------------------------------------------------------------
		struct ImpacterData 
		{
			float value;							//�e���x
			float circleRange;						//�~�̉e���͈�
			ImpacterType type;						//�e���͈̓^�C�v(���E�����݂���Ȃ�A���̕����Z����B)
			std::weak_ptr<NavGraphNode> selfNode;	//��������������m�[�h

			ImpacterData();
		};

		//--------------------------------------------------------------------------------------
		///	�e����^����҂̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_Impacter
		{
			ImpacterData m_impacterData; //�e����^����҂̃f�[�^

		public:
			virtual ~I_Impacter() = default;

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
			/// <returns>�e����^����҂̃f�[�^</returns>
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

			/// <summary>
			/// �e���}�b�v�̎擾
			/// </summary>
			/// <returns>�e���}�b�v</returns>
			virtual std::shared_ptr<ImpactMap> GetImpactMap() const = 0;

			/// <summary>
			/// ��������m�[�h�̐ݒ�
			/// </summary>
			/// <param name="node">�e���f�[�^</param>
			void SetSelfNode(const std::shared_ptr<NavGraphNode>& node);

			/// <summary>
			/// ��������������m�[�h�̎擾
			/// </summary>
			/// <returns>��������������m�[�h</returns>
			std::shared_ptr<NavGraphNode> GetSelfNode() const;

		};

	}
}