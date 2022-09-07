/*!
@file MaruAnimationHelper.h
@brief MaruAnimationHelper�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/StateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	���[�V�����C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_Motion
		{
		public:
			/// <summary>
			/// �J�n����
			/// </summary>
			virtual void OnStart() = 0;

			/// <summary>
			/// �X�V����
			/// </summary>
			/// <param name="speed">�X�V�X�s�[�h(Default == 1.0f)</param>
			/// <returns>�X�V���I�����Ă���Ȃ�true</returns>
			virtual bool OnUpdate(const float& speed = 1.0f) = 0;

			/// <summary>
			/// �I������
			/// </summary>
			virtual void OnExit() = 0;

			/// <summary>
			/// �A�j���[�V�����̍Đ��ɂ����鎞�Ԃ��擾
			/// </summary>
			/// <returns>�A�j���[�V�����̍Đ��ɂ����鎞��</returns>
			virtual float GetTime() const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	���[�V�������N���X
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class MotionBase : public I_Motion 
		{
		private:
			float m_time = 0.0f;            //���[�V�����̑S�̎���
			ex_weak_ptr<OwnerType> m_owner; //���L��

		public:
			/// <summary>
			/// �R���X�g���N�^��
			/// </summary>
			/// <param name="owner">���L�҂̃|�C���^</param>
			MotionBase(const std::shared_ptr<OwnerType>& owner)
				:m_owner(owner), m_time(0.0f)
			{}

		protected:
			/// <summary>
			/// �A�j���[�V�����ɂ����鎞�Ԃ��Z�b�e�B���O����B
			/// </summary>
			/// <param name="params">�A�j���[�V�����Đ��p�̃f�[�^</param>
			void SettingTime(const vector<basecross::Metaball::Task::SeekOffsetMove_Parametor>& params) {
				m_time = 0.0f;
				for (const auto& param : params) {
					m_time += param.time;
				}
			}

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ���L�҂̎擾
			/// </summary>
			/// <returns>���L��</returns>
		 	std::shared_ptr<OwnerType> GetOwner() const {
				return m_owner.GetShard();
			}

			float GetTime() const override final{
				return m_time;
			}
		};

		//--------------------------------------------------------------------------------------
		///	���^�{�[�����[�V�����̏����f�[�^
		//--------------------------------------------------------------------------------------
		struct MetaballMotion_InitializeData {
			float radius;               //���a
			std::vector<Vec3> offsets;  //�I�t�Z�b�g�z��
		};

		//--------------------------------------------------------------------------------------
		///	���^�{�[���𗘗p�������[�V�����̊��N���X
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class MetaballMotionBase : public MotionBase<OwnerType>
		{
		private:
			//������Ԃ̃��f���f�[�^��ۑ�
			MetaballMotion_InitializeData m_initializeData;

		protected:
			//���[�V�����Ǘ��N���X
			std::unique_ptr<basecross::Metaball::MotionManager> m_motionManager = nullptr;

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���L�҂̃|�C���^</param>
			MetaballMotionBase(const std::shared_ptr<OwnerType>& owner)
				:MotionBase(owner), m_motionManager(new Metaball::MotionManager(owner->GetGameObject()))
			{
				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (render) { //nullCheck
					//�I�t�Z�b�g�̎擾
					m_initializeData.radius = render->GetChildren()[0]->GetRadius();
					m_initializeData.offsets = render->CalculateChildrenSeekOffsets();
				}
			}

			void OnStart() override final {
				//���[�V�����Ǘ��N���X�Ƀ��[�V�����f�[�^��n���B
				auto motionParams = CalculateMotionParametors();
				for (const auto& motionParam : motionParams) {
					m_motionManager->AddTask(Metaball::MotionManager::TaskEnum::Move, motionParam);
				}
			}

			bool OnUpdate(const float& speed = 1.0f) override {
				m_motionManager->OnUpdate();
				return m_motionManager->IsEnd();
			}

			void OnExit() override final{
				m_motionManager->ForceStop(true);
			}

		protected:

			/// <summary>
			/// ���[�V�����Đ��p�̃p�����[�^�Q��Ԃ��B
			/// </summary>
			/// <returns>���[�V�����Đ��p�̃p�����[�^�Q</returns>
			virtual vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() = 0;

			/// <summary>
			/// �������a�̎擾
			/// </summary>
			/// <returns>�������a</returns>
			float GetInitializeRadius() const { return m_initializeData.radius; }

			/// <summary>
			/// �����I�t�Z�b�g�z����擾
			/// </summary>
			/// <returns>�����I�t�Z�b�g�z��</returns>
			std::vector<Vec3> GetInitializeOffsets() const { return m_initializeData.offsets; }
		};

		//--------------------------------------------------------------------------------------
		///	�A�j���[�V�����C�x���g�\����
		//--------------------------------------------------------------------------------------
		struct AnimationTimeEvent
		{
			float time;                   //�Ăяo������
			std::function<void()> event;  //�Ăяo���C�x���g�B
			bool isEnd;                   //�Ăяo���ꂽ�ォ�ǂ����𔻒f����B

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�Ăяo������</param>
			/// <param name="event">�Ăяo���C�x���g</param>
			AnimationTimeEvent(const float& time, const std::function<void()> event);
		};

		//--------------------------------------------------------------------------------------
		///	�A�j���[�V�����N���b�v�N���X
		//--------------------------------------------------------------------------------------
		class AnimationClip : public I_StateNode
		{
			std::shared_ptr<I_Motion> m_motion;                //���[�V�����f�[�^
			bool m_isLoop;                                     //���[�v���邩�ǂ���
			float m_timeSpeed;                                 //�A�j���[�V�����̍Đ��X�s�[�h
			vector<AnimationTimeEvent> m_timeEvents;           //�A�j���[�V�����̃^�C���C�x���g
			vector<std::shared_ptr<I_StateNode>> m_stateNodes; //�A�j���[�V�����N���b�v�Ɍ�t���ŏ�����t�^���������p

			std::unique_ptr<GameTimer> m_timer = nullptr;      //�^�C�}�[�Ǘ��N���X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="motion">���[�V�����C���^�[�t�F�[�X</param>
			AnimationClip(const std::shared_ptr<I_Motion>& motion);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="motion">���[�V�����C���^�[�t�F�[�X</param>
			/// <param name="isLoop">���[�v���f</param>
			AnimationClip(const std::shared_ptr<I_Motion>& motion, const bool isLoop);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="motion">���[�V�����C���^�[�t�F�[�X</param>
			/// <param name="isLoop">���[�v���f</param>
			/// <param name="timeSpeed">���[�V�����Đ��X�s�[�h</param>
			AnimationClip(const std::shared_ptr<I_Motion>& motion, const bool isLoop, const float& timeSpeed);

			virtual void OnStart() override;
			virtual bool OnUpdate() override;
			virtual void OnExit() override;

		private:
			/// <summary>
			/// ���[�V�����̃A�b�v�f�[�g
			/// </summary>
			void MotionUpdate();

			/// <summary>
			/// �^�C���C�x���g�Ǘ�
			/// </summary>
			void TimeEventManager();

			/// <summary>
			/// ���Ԃ��o�߂���������Ăяo������
			/// </summary>
			void TimerEndFunction(); 

			/// <summary>
			/// ���ԏI�����ɌĂяo����Ă��Ȃ������^�C���C�x���g���Ăԏ���
			/// </summary>
			void ResetTimeEvents();

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �^�C���C�x���g�̒ǉ�
			/// </summary>
			/// <param name="time">�Ăяo������</param>
			/// <param name="event">�Ăяo���C�x���g</param>
			void AddTimeEvent(const float& time, const std::function<void()>& event);

			/// <summary>
			/// ��t���X�e�[�g�m�[�h�̒ǉ�
			/// </summary>
			/// <param name="stateNode">�ǉ��������X�e�[�g�m�[�h</param>
			void AddStateNode(const std::shared_ptr<I_StateNode>& stateNode);

			/// <summary>
			/// ���[�v���f
			/// </summary>
			/// <returns>���[�v����Ȃ�true</returns>
			bool IsLoop() const noexcept;

			/// <summary>
			/// �I�����f
			/// </summary>
			/// <returns>�I�����Ă���Ȃ�true</returns>
			bool IsEnd() const;

		};

		//--------------------------------------------------------------------------------------
		///	�A�j���[�V�����X�e�[�g�m�[�h�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_AnimationStateNode : public I_StateNode
		{
		public:
			/// <summary>
			/// �A�j���[�V�����N���b�v�̎擾
			/// </summary>
			/// <returns>�A�j���[�V�����N���b�v</returns>
			virtual std::shared_ptr<AnimationClip> GetAnimationClip() const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	�A�j���[�V�����X�e�[�g�m�[�h�̊��N���X
		//--------------------------------------------------------------------------------------
		class AnimationStateNodeBase : public I_AnimationStateNode
		{
			//�A�j���[�V�����N���b�v
			std::shared_ptr<AnimationClip> m_animationClip = nullptr;

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="animationClip">�A�j���[�V�����N���b�v</param>
			AnimationStateNodeBase(const std::shared_ptr<AnimationClip>& animationClip);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

			std::shared_ptr<AnimationClip> GetAnimationClip() const noexcept override;
		};

	}
	
}