/*!
@file ParticleManager.h
@brief ParticleManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/HitStopManager.h"

#include "Maruyama/Utility/ReactiveBool.h"
#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BoardObject;
	class GameTimer;

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �p�[�e�B�N���Ǘ��̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct ParticleManager_Parametor
		{
			Builder::VertexPCTParametor vertexPCTParam;    //�p�[�e�B�N���Ɏg�������|���p�̃f�[�^
			Vec2 powerRandomVec;                           //�p���[�̃����_����
			Vec3 shotDirectRandomVec;                      //�͂�����������̃����_����
			Vec2 sizeRandomVec;                            //�|���̑傫���̃����_����
			float liveTime = 1.0f;                         //�������Ă���폜�����܂ł̎���
			float intervalTime = 0.0f;                     //���[�v�������̃C���^�[�o���^�C��
			Vec3 gravityScale = Vec3(0.0f, -9.8f, 0.0f);   //�d�͂̑傫��  
			uint16_t numOneFrameCreate;                    //1�t���[���ɐ��������
			bool isLoop = true;                            //���[�v���邩�ǂ���
			bool isBillBoard = false;                      //�r���{�[�h�����邩�ǂ���
			bool isForwardRotation = false;                //Shot�����ɉ�]���邩�ǂ���
			bool isAlphaFade = false;                      //�������t�F�[�h�������邩�ǂ���
			bool isGravity = false;                        //�d�͂����邩�ǂ���
			bool isRayHitDestory = false;                  //RayHit�ŕǂɂԂ��������̍폜�����邩�ǂ���
			ex_weak_ptr<GameObject> parent = nullptr;      //�y�A�����g�ݒ�

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ParticleManager_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="vertexPCTParam">�p�[�e�B�N���Ɏg�������|���p�̃f�[�^</param>
			ParticleManager_Parametor(const Builder::VertexPCTParametor& vertexPCTParam);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="vertexPCTParam">�p�[�e�B�N���Ɏg�������|���p�̃f�[�^</param>
			/// <param name="powerRandomVec">�p���[�̃����_����</param>
			/// <param name="shotDirectRandomVec">�͂�����������̃����_����</param>
			/// <param name="scaleRandomVec">�|���̑傫���̃����_����</param>
			/// <param name="liveTime">�������Ă���폜�����܂ł̎���</param>
			/// <param name="intervalTime">���[�v�������̃C���^�[�o���^�C��</param>
			/// <param name="numOneFrameCreate">1�t���[���ɐ��������</param>
			/// <param name="isLoop">���[�v���邩�ǂ���</param>
			ParticleManager_Parametor(
				const Builder::VertexPCTParametor& vertexPCTParam,
				const Vec2& powerRandomVec,
				const Vec3& shotDirectRandomVec,
				const Vec2& scaleRandomVec,
				const float& liveTime,
				const float& intervalTime,
				const uint16_t numOneFrameCreate,
				const bool isLoop
			);
		};

		//--------------------------------------------------------------------------------------
		/// �p�[�e�B�N���Ǘ�
		//--------------------------------------------------------------------------------------
		class ParticleManager : public Component
		{
		public:
			using Parametor = ParticleManager_Parametor;

		private:
			Parametor m_param;
			uint16_t m_loopCount = 0;                          //���[�v�J�E���g

			std::vector<ex_weak_ptr<BoardObject>> m_particels; //���������p�[�e�B�O���Q

			std::unique_ptr<GameTimer> m_timer = nullptr;      //�^�C�}�[�Ǘ�

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			ParticleManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			ParticleManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

			void OnDrawActive() override;

			void OnDestroy() override;

		private:
			/// <summary>
			/// �X�V����
			/// </summary>
			void UpdateProcess();

			/// <summary>
			/// ���[�v�X�V
			/// </summary>
			void LoopUpdate();

			/// <summary>
			/// �p�[�e�B�N���̍X�V
			/// </summary>
			void ParticlesUpdate();

			/// <summary>
			/// �p�[�e�B�N���̓����x�X�V
			/// </summary>
			void AlphaFadeUpdate();

			/// <summary>
			/// �p�[�e�B�N���̐���
			/// </summary>
			void CreateParticle();

			/// <summary>
			/// �p�[�e�B�N���𕡐�����
			/// </summary>
			void CreateParticles();

			/// <summary>
			/// RayHit���ɏ��ł���ꍇ�ɂ��鏈��
			/// </summary>
			/// <param name="particle">���������p�[�e�B�N��</param>
			void SettingRayHitDestory(const std::shared_ptr<BoardObject>& particle);

			/// <summary>
			/// �p�[�e�B�N�����΂�����
			/// </summary>
			/// <param name="particle">���������p�[�e�B�N��</param>
			void ShotParticle(const std::shared_ptr<BoardObject>& particle);

			/// <summary>
			/// �p�[�e�B�N���̔�΂��������v�Z
			/// </summary>
			/// <returns>�p�[�e�B�N�����΂�����</returns>
			Vec3 CalculateShotDirect();

		public:
			/// <summary>
			/// ���������p�[�e�B�N���z��̎擾
			/// </summary>
			/// <returns>���������p�[�e�B�N���z��</returns>
			std::vector<ex_weak_ptr<BoardObject>> GetParticles() const;

		};

	}
}