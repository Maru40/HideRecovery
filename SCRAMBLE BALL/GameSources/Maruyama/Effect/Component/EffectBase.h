/*!
@file EffectBase.h
@brief EffectBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/UI/BuilderVertexPCT.h"
#include "AuraBase.h"

#include "ParticleManager.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BoardObject;

	namespace Effect {
		class AuraObjectBase;
		struct Aura_Parametor;

		//--------------------------------------------------------------------------------------
		/// �I�t�Z�b�g�f�[�^
		//--------------------------------------------------------------------------------------
		struct OffsetData {
			Vec3 position;  //�ʒu
			Vec3 rotation;  //��]
			Vec3 scale;     //�傫��

			OffsetData();
		};

		//--------------------------------------------------------------------------------------
		/// �G�t�F�N�g�{�[�h�����p�̃f�[�^
		//--------------------------------------------------------------------------------------
		struct BoardData {
			Builder::VertexPCTParametor vertexPCTParam;
			Vec3 rotation;

			BoardData(const Builder::VertexPCTParametor& vertexPCTParam, const Vec3& rotation);
		};

		//--------------------------------------------------------------------------------------
		/// �I�[�������p�̃f�[�^
		//--------------------------------------------------------------------------------------
		struct CreateAuraData
		{
			Aura_Parametor auraParam;
			OffsetData offsetData;

			CreateAuraData(const Aura_Parametor& auraParam, const OffsetData offsetData = OffsetData());
		};

		//--------------------------------------------------------------------------------------
		/// �p�[�e�B�N�������p�̃f�[�^
		//--------------------------------------------------------------------------------------
		struct ParticleData
		{
			maru::ParticleManager::Parametor particleParam;
			OffsetData offsetData;

			ParticleData(const maru::ParticleManager::Parametor particleParam, const OffsetData& offsetData = OffsetData());
		};

		//--------------------------------------------------------------------------------------
		/// �G�t�F�N�g�̊��N���X
		//--------------------------------------------------------------------------------------
		class Base : public Component
		{
		private:
			std::vector<ex_weak_ptr<GameObject>> m_effectObjects;                //�S�ẴG�t�F�N�g�I�u�W�F�N�g
			std::vector<ex_weak_ptr<BoardObject>> m_boardObjects;                //�{�[�h�I�u�W�F�N�g
			std::vector<ex_weak_ptr<AuraObjectBase>> m_auraObjects;              //�I�[���I�u�W�F�N�g
			std::vector<ex_weak_ptr<maru::ParticleManager>> m_particleManagers;  //�p�[�e�B�N���}�l�[�W���\

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Base(const std::shared_ptr<GameObject>& objPtr);

			virtual void OnCreate() override {};

		public:
			/// <summary>
			/// �{�[�h�G�t�F�N�g����
			/// </summary>
			/// <param name="data">�{�[�h�f�[�^</param>
			/// <returns>���������{�[�h</returns>
			std::shared_ptr<BoardObject> CreateBoardEffect(const BoardData& data);

			/// <summary>
			/// �{�[�h�G�t�F�N�g�̕�������
			/// </summary>
			/// <param name="datas">�{�[�h�f�[�^�z��</param>
			/// <returns>���������S�Ẵ{�[�h</returns>
			std::vector<std::shared_ptr<BoardObject>> CreateBoardEffects(const std::vector<BoardData>& datas);

			/// <summary>
			/// �I�[���G�t�F�N�g����
			/// </summary>
			/// <param name="data">�I�[���G�t�F�N�g�f�[�^</param>
			/// <returns>���������I�[���G�t�F�N�g</returns>
			std::shared_ptr<AuraObjectBase> CreateAuraObject(const CreateAuraData& data);

			/// <summary>
			/// �I�[���G�t�F�N�g�̕�������
			/// </summary>
			/// <param name="datas">�I�[���G�t�F�N�g�̃f�[�^�z��</param>
			/// <returns>���������S�ẴI�[���I�u�W�F�N�g</returns>
			std::vector<std::shared_ptr<AuraObjectBase>> CreateAuraObjects(const std::vector<CreateAuraData>& datas);

			/// <summary>
			/// �p�[�e�B�N������
			/// </summary>
			/// <param name="data">�p�[�e�B�N���f�[�^</param>
			/// <returns>���������p�[�e�B�N��</returns>
			std::shared_ptr<maru::ParticleManager> CreateParticle(const ParticleData& data);

			/// <summary>
			/// �p�[�e�B�N���̕�������
			/// </summary>
			/// <param name="datas">�p�[�e�B�N���̃f�[�^�z��</param>
			/// <returns>���������S�Ẵp�[�e�B�N��</returns>
			std::vector<std::shared_ptr<maru::ParticleManager>> CreateParticles(const std::vector<ParticleData>& datas);

		protected:
			/// <summary>
			/// �G�t�F�N�g�I�u�W�F�N�g�̒ǉ�
			/// </summary>
			/// <param name="objPtr">�ǉ�����I�u�W�F�N�g</param>
			/// <param name="offsetData">�I�t�Z�b�g�f�[�^</param>
			void AddEffectObject(const std::shared_ptr<GameObject>& objPtr, const OffsetData& offsetData = OffsetData());

			/// <summary>
			/// �G�t�F�N�g�I�u�W�F�N�g�̔z����擾
			/// </summary>
			/// <returns>�G�t�F�N�g�I�u�W�F�N�g�̔z��</returns>
			std::vector<ex_weak_ptr<GameObject>> GetEffectObjects() const;

			/// <summary>
			/// �I�[���I�u�W�F�N�g�̒ǉ�
			/// </summary>
			/// <param name="auraObject">�ǉ�����I�[���I�u�W�F�N�g</param>
			/// <param name="offsetData">�I�t�Z�b�g�f�[�^</param>
			void AddAuraObject(const std::shared_ptr<AuraObjectBase>& auraObject, const OffsetData& offsetData);

			/// <summary>
			/// �I�[���I�u�W�F�N�g�̔z����擾
			/// </summary>
			/// <returns>�I�[���I�u�W�F�N�g�̔z��</returns>
			std::vector<ex_weak_ptr<AuraObjectBase>> GetAuraObjects() const;

			/// <summary>
			/// �p�[�e�B�N���}�l�[�W���[�̒ǉ�
			/// </summary>
			/// <param name="particleManager">�p�[�e�B�N���}�l�[�W���[</param>
			/// <param name="offsetData">�I�t�Z�b�g�f�[�^</param>
			void AddParticleManager(const std::shared_ptr<maru::ParticleManager>& particleManager, const OffsetData& offsetData);

			/// <summary>
			/// �p�[�e�B�N���}�l�[�W���[�̎擾
			/// </summary>
			/// <returns>�p�[�e�B�N���}�l�[�W���[</returns>
			std::vector<ex_weak_ptr<maru::ParticleManager>> GetParticleManagers() const;

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �G�t�F�N�g�̕`������邩�ݒ�
			/// </summary>
			/// <param name="isDraw">�G�t�F�N�g��`�悷�邩�ǂ���</param>
			void SetEffectsDrawActive(const bool isDraw);
		};

	}
}