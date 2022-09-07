
/*!
@file OccupancyManager.h
@brief OccupancyManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_Impact.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class GameTimer;

	namespace maru {

		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^�̐�L�l�Ǘ��p�����[�^
		//--------------------------------------------------------------------------------------
		struct OccupancyManager_Parametor {

		};

		//--------------------------------------------------------------------------------------
		///	��L�l�X�V�f�[�^
		//--------------------------------------------------------------------------------------
		struct OccupancyUpdateData {
			float startValue;					//�f�[�^�������̐�L�l
			std::unique_ptr<GameTimer> timer;	//�^�C�}�[
			std::weak_ptr<NavGraphNode> node;	//�m�[�h

			OccupancyUpdateData();

			OccupancyUpdateData(const float intervalTime, const std::shared_ptr<NavGraphNode>& node);

			OccupancyUpdateData(const GameTimer& timer, const std::shared_ptr<NavGraphNode>& node);

			bool operator== (const OccupancyUpdateData& data);
		};

		//--------------------------------------------------------------------------------------
		///	�e���f�[�^�̐�L�l�Ǘ�
		//--------------------------------------------------------------------------------------
		class OccupancyManager
		{
		public:
			using UpdateData = OccupancyUpdateData;

		private:
			std::weak_ptr<maru::ImpactMap> m_impactMap;	//�e���}�b�v

			std::vector<UpdateData> m_updateDatas;		//��L�l�X�V�f�[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="impactMap">�e���}�b�v</param>
			OccupancyManager(const std::shared_ptr<maru::ImpactMap>& impactMap);

			/// <summary>
			/// �X�V����
			/// </summary>
			void OnUpdate();

		private:
			/// <summary>
			/// ��L�l�X�V
			/// </summary>
			void OccupancyValueUpdate();

			/// <summary>
			/// ��L�l�X�V���Ԃ��v�Z���ĕԂ��B
			/// </summary>
			/// <param name="node">�X�V����m�[�h</param>
			float CalculateOccupancyIntervalTime(const std::shared_ptr<NavGraphNode>& node);

			/// <summary>
			/// �f�[�^�z�񂩂�폜����
			/// </summary>
			/// <param name="removeDatas">�폜�������f�[�^�z��</param>
			void RemoveUpdateDatas(const std::vector<UpdateData*>& removeDatas);

		public:
			/// <summary>
			/// ��L�l�X�V����f�[�^�𐶐�
			/// </summary>
			/// <param name="node">�X�V����m�[�h</param>
			void AddOccupancyUpdateData(const std::shared_ptr<NavGraphNode>& node);

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �e���}�b�v�̎擾
			/// </summary>
			/// <returns>�e���}�b�v</returns>
			std::shared_ptr<ImpactMap> GetImpactMap() const { return m_impactMap.lock(); }

		};

	}
}