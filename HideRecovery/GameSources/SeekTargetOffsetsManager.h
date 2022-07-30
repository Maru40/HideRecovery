/*!
@file SeekTargetOffsetsManager.h
@brief SeekTargetOffsetsManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class GameTimer;
	class SeekTarget;

	namespace Metaball {
		class ChildrenRender;
	}

	//--------------------------------------------------------------------------------------
	///	�Ǐ]�I�t�Z�b�g�Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class SeekTargetOffsetsManager 
	{
	public:
		/// <summary>
		/// �^�X�N�^�C�v
		/// </summary>
		enum class TaskEnum {
			CenterMove,   //���S�Ɉړ����鏈��
			OuterMove,    //�O���Ɉړ����鏈��
			ReturnMove,   //���ɖ߂鏈��
			TargetMove,   //�^�[�Q�b�g�Ɍ���������
		};

		/// <summary>
		/// �Ǐ]�I�t�Z�b�g�Ǘ��p�����[�^
		/// </summary>
		struct Parametor
		{
			float time;   //�ǂ̂��炢�̎��Ԃňړ����邩
			float range;  //�k�ދ���
			Vec3 offset;  //�����̃I�t�Z�b�g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="time">�ǂ̂��炢�̎��Ԃňړ����邩</param>
			/// <param name="range">�k�ދ���</param>
			/// <param name="offset">�����̃I�t�Z�b�g</param>
			Parametor(const float& time, const float& range, const Vec3& offset = Vec3(0.0f));
		};

		/// <summary>
		/// ��{�̒Ǐ]�f�[�^
		/// </summary>
		struct DefaultSeekData
		{
			Vec3 offset; //�Ǐ]�I�t�Z�b�g
			float range; //�Ǐ]����

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			DefaultSeekData();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="offset">�Ǐ]�I�t�Z�b�g</param>
			/// <param name="range">�Ǐ]����</param>
			DefaultSeekData(const Vec3& offset, const float& range);
		};

	private:
		ex_weak_ptr<GameObject> m_owner;                //�������g�����L����Q�[���I�u�W�F�N�g

		std::unique_ptr<GameTimer> m_timer;             //���ԊǗ��N���X
		std::unique_ptr<TaskList<TaskEnum>> m_taskList; //�^�X�N�Ǘ��N���X

		Parametor m_centerMoveParam;                    //���S�Ɍ������ē����p�����[�^
		Parametor m_returnMoveParam;                    //���̈ʒu�ɖ߂鎞�̃p�����[�^

		vector<DefaultSeekData> m_defaultSeekData;      //�Ǐ]�f�[�^�̃f�t�H���g�l

		vector<ex_weak_ptr<SeekTarget>> m_seekTargets;  //�Ǐ]�p�����[�^�Q

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="seekTargets">�Ǐ]�R���|�[�l���g�Q</param>
		SeekTargetOffsetsManager(const std::shared_ptr<GameObject>& owner, const vector<ex_weak_ptr<SeekTarget>>& seekTargets);

		//�O������Ăяo��
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
		/// �Ǐ]�̃f�t�H���g�l��ݒ肷��B
		/// </summary>
		void SetDefaultSeekOffset();

		/// <summary>
		/// ��Ԃ��ꂽ�ړ��ꏊ���v�Z���ĕԂ��B
		/// </summary>
		/// <param name="startPosition">�J�n�ꏊ</param>
		/// <param name="endPosition">�I���ꏊ</param>
		/// <param name="seek">�Ǐ]�R���|�[�l���g</param>
		/// <param name="parametor">�Ǐ]�p�����[�^</param>
		/// <returns>�⊮���ꂽ�ړ��ꏊ</returns>
		Vec3 CalculateLerpMovePosition(const Vec3& startPosition, const Vec3& endPosition,
			const std::shared_ptr<SeekTarget>& seek, const Parametor& parametor);

		/// <summary>
		/// �ړ��ꏊ���v�Z���ĕԂ��B
		/// </summary>
		/// <param name="startPosition">�J�n�ꏊ</param>
		/// <param name="endPosition">�I���ꏊ</param>
		/// <param name="seek">�Ǐ]�R���|�[�l���g</param>
		/// <param name="parametor">�Ǐ]�p�����[�^</param>
		/// <returns>�ړ��ꏊ</returns>
		Vec3 CalculateTransformMovePosition(const Vec3& startPosition, const Vec3& endPosition,
			const std::shared_ptr<SeekTarget>& seek, const Parametor& parametor);

		/// <summary>
		/// ���S�����ւ̈ړ��J�n
		/// </summary>
		void StartCenterMove();

		/// <summary>
		/// ���S�����ւ̈ړ���
		/// </summary>
		/// <returns>�ړ�������������true</returns>
		bool UpdateCenterMove();

		/// <summary>
		/// ���̏ꏊ�Ɉړ��J�n
		/// </summary>
		void StartReturnMove();

		/// <summary>
		/// ���̏ꏊ�Ɉړ���
		/// </summary>
		/// <returns>�ړ�������������true</returns>
		bool UpdateReturnMove();

		/// <summary>
		/// ���̏ꏊ�Ɉړ�������̏���
		/// </summary>
		void ExitReturnMove();

	public:
		/// <summary>
		/// �ړ����J�n����
		/// </summary>
		/// <param name="task">�^�X�N�^�C�v</param>
		/// <param name="parametor">�Ǐ]�Ǘ��p�����[�^</param>
		void StartMove(const TaskEnum& task, const Parametor& parametor);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�X�N�̏I���𔻒f
		/// </summary>
		/// <returns>�^�X�N�̏I�����Ă�����true</returns>
		bool IsEndTask();

		/// <summary>
		///	�������g�����L����Q�[���I�u�W�F�N�g�̎擾
		/// </summary>
		/// <returns>�������g�����L����Q�[���I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> GetGameObject();

	};

}

//endbasecross