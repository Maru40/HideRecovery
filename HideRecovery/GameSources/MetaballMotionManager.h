/*!
@file MetaballMotionManager.h
@brief MetaballMotionManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class GameTimer;

	namespace Enemy {
		class SlimeCore;
	}

	namespace Metaball {

		class ChirdlenRender;

		namespace Task {

			//--------------------------------------------------------------------------------------
			///	�Ǐ]�I�t�Z�b�g�ړ��p�����[�^
			//--------------------------------------------------------------------------------------
			struct SeekOffsetMove_Parametor {
				float time;                                //�ǂ̂��炢���Ԃ��|���đJ�ڂ��邩
				float radius;                              //�J�ڂ���X�t�B�A�̑傫��
				vector<SeekTarget::Parametor> seekParams;  //�J�ڂ���p�����[�^
				SeekTarget::Parametor coreSeekParam;       //�R�A�̒Ǐ]�p�����[�^
				float firstSpeed;                          //�����X�s�[�h

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				SeekOffsetMove_Parametor();
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="time">�ړ�����</param>
				/// <param name="radius">���a</param>
				SeekOffsetMove_Parametor(const float& time, const float& radius);
			};

			//--------------------------------------------------------------------------------------
			///	��Ԉړ��p�̏����l��ԃf�[�^
			//--------------------------------------------------------------------------------------
			struct SaveLerpData {
				float radius;      //���a
				Vec3 startOffset;  //�����I�t�Z�b�g�l
				Vec3 velocity;     //�������x

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				SaveLerpData()
					:SaveLerpData(1.0f, Vec3(0.0f))
				{}

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="radius">���a</param>
				/// <param name="startOffset">�����I�t�Z�b�g</param>
				SaveLerpData(const float& radius, const Vec3& startOffset)
					:radius(radius), startOffset(startOffset), velocity(Vec3(0.0f))
				{}
			};

			//--------------------------------------------------------------------------------------
			///	�Ǐ]�I�t�Z�b�g�ړ�
			//--------------------------------------------------------------------------------------
			class SeekOffsetMove : public TaskNodeBase<GameObject>
			{
			public:
				using Parametor = SeekOffsetMove_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;     //�p�����[�^
				vector<SaveLerpData> m_saveLerpDatas;            //Lerp�ړ��p�̏����l�ۑ�
				SaveLerpData m_coreSaveLerpData;                 //Core��Lerp�ړ��p�̏����l�ۑ�

				vector<ex_weak_ptr<SeekTarget>> m_seekTargets;   //�Ǐ]�R���|�[�l���g�Q
				ex_weak_ptr<Enemy::SlimeCore> m_core;            //Core�f�[�^
				std::unique_ptr<GameTimer> m_timer = nullptr;    //�^�C�}�[�Ǘ�

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
				/// <param name="paramPtr">�p�����[�^�̃|�C���^</param>
				SeekOffsetMove(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// Core�f�[�^�������o�ϐ��Ɏ擾����B
				/// </summary>
				void SettingCore();

				/// <summary>
				/// �Ǐ]�p�����[�^�Q�������o�ϐ��Ɏ擾����B
				/// </summary>
				void SettingSeekTargets();

				/// <summary>
				/// �Ǐ]�I�t�Z�b�g�̃A�b�v�f�[�g�Ǘ�
				/// </summary>
				/// <param name="seek">�Ǐ]�R���|�[�l���g</param>
				/// <param name="seekParam">�Ǐ]�p�����[�^</param>
				/// <param name="saveLerpData">Lerp�p�̕ۑ��f�[�^</param>
				void SeekOffsetUpdate(const std::shared_ptr<SeekTarget>& seek,
					const SeekTarget::Parametor& seekParam,
					const SaveLerpData& saveLerpData);

				/// <summary>
				/// �Ǐ]�I�t�Z�b�g�̕�ԏ�������ꂽ�A�b�v�f�[�g�Ǘ�
				/// </summary>
				/// <param name="seek">�Ǐ]�R���|�[�l���g</param>
				/// <param name="seekParam">�Ǐ]�p�����[�^</param>
				/// <param name="saveLerpData">Lerp�p�̕ۑ��f�[�^</param>
				void SeekOffsetLerpUpdate(const std::shared_ptr<SeekTarget>& seek,
					const SeekTarget::Parametor& seekParam,
					const SaveLerpData& saveLerpData);

				/// <summary>
				/// ���^�{�[���̔��a���A�b�v�f�[�g�ŕω������鏈���B
				/// </summary>
				/// <param name="render">���^�{�[�������_�[�p�R���|�[�l���g</param>
				/// <param name="radius">���^�{�[���̔��a</param>
				/// <param name="speed">�ό`����X�s�[�h</param>
				/// <param name="startRadius">�������a</param>
				void RadiusUpdate(const std::shared_ptr<basecross::Metaball::ChildrenRender>& render, 
					const float& radius, const float& speed, const float& startRadius);

				/// <summary>
				/// �⊮���ꂽ�ړI�n��Ԃ�
				/// </summary>
				/// <param name="startPosition">�J�n�ʒu</param>
				/// <param name="endPosition">�I���ʒu</param>
				/// <returns>��Ԃ��ꂽ�ړI�n</returns>
				Vec3 CalcuLerpMovePosition(const Vec3& startPosition, const Vec3& endPosition);

				/// <summary>
				/// �X�V���x��Ԃ��B
				/// </summary>
				/// <returns>�X�V���x</returns>
				float CalculateUpdateTimeSpeed();

				/// <summary>
				/// Lerp�p�̏����f�[�^��ۑ�����B
				/// </summary>
				void SaveLerpDatas();

				/// <summary>
				/// �I�����f
				/// </summary>
				/// <returns>�I�����Ă���Ȃ�true</returns>
				bool IsEnd();
			};
		}

		//--------------------------------------------------------------------------------------
		///	���[�V�����Ǘ��N���X
		//--------------------------------------------------------------------------------------
		class MotionManager
		{
		public:
			/// <summary>
			/// �^�X�N�^�C�v
			/// </summary>
			enum class TaskEnum {
				Move, //�ړ�
			};

		private:
			ex_weak_ptr<GameObject> m_owner = nullptr; //���L��

			std::unique_ptr<TaskList<TaskEnum>> m_taskList = nullptr;                                //�^�X�N���X�g�Ǘ��N���X
			std::shared_ptr<Task::SeekOffsetMove::Parametor> m_currentSeekOffetParametor = nullptr;  //���݂̒Ǐ]�p�����[�^
			std::queue<Task::SeekOffsetMove::Parametor> m_seekOffsetParametors;                      //�Ǐ]�p�����[�^�L���[

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			MotionManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �O������̌Ăяo������X�V����
			/// </summary>
			void OnUpdate();

		private:
			/// <summary>
			/// �^�X�N�̒�`
			/// </summary>
			void DefineTask();

			/// <summary>
			/// �^�X�N�̑I��
			/// </summary>
			/// <param name="task">�I�������^�X�N</param>
			void SelectTask(const TaskEnum& task);

			/// <summary>
			/// ��̃^�X�N���I�������ꍇ�ɌĂяo������
			/// </summary>
			void OneTaskEndFunction();

			/// <summary>
			/// ���݂̒Ǐ]�p�����[�^��ύX����B
			/// </summary>
			/// <returns>�ύX�ł�����true</returns>
			bool ChangeCurrentParametor();

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �������g�����L����Q�[���I�u�W�F�N�g�̎擾
			/// </summary>
			/// <returns>�������g�����L����Q�[���I�u�W�F�N�g</returns>
			std::shared_ptr<GameObject> GetOwner() const { return m_owner.GetShard(); }

			/// <summary>
			/// �^�X�N�̒ǉ�
			/// </summary>
			/// <param name="taskEnum">�ǉ�����^�X�N�̃^�C�v</param>
			/// <param name="parametor">�Ǐ]�I�t�Z�b�g�p�����[�^</param>
			void AddTask(const TaskEnum& taskEnum, const Task::SeekOffsetMove_Parametor& parametor);

			/// <summary>
			/// �����X�g�b�v
			/// </summary>
			/// <param name="isExitFunc">�I�����̊֐����Ăяo�����ǂ���</param>
			void ForceStop(const bool isExitFunction = true);

			/// <summary>
			/// �I�����f
			/// </summary>
			/// <returns>�I�����Ă���Ȃ�true</returns>
			bool IsEnd();

		};

	}

}