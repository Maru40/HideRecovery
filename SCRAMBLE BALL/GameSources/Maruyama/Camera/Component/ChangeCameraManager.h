/*!
@file ChangeCameraManager.h
@brief ChangeCameraManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "I_BrackBoard.h"
#include "SingletonComponent.h"

namespace basecross {

	//�O���錾
	class GameTimer;

	template<class NodeType, class EnumType, class TransitionMember>
	class EnemyMainStateMachine;

	//--------------------------------------------------------------------------------------
	///	�J�����؂�ւ��Ǘ��̃u���b�N�{�[�h�p�����[�^
	//--------------------------------------------------------------------------------------
	struct ChangeCameraManager_BlackBoardParametor
	{
		ex_weak_ptr<GameObject> lookAtTarget;   //��������^�[�Q�b�g
	};

	//--------------------------------------------------------------------------------------
	///	�J�����؂�ւ��Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class ChangeCameraManager : public maru::SingletonComponent<ChangeCameraManager>, public I_BrackBoard<ChangeCameraManager_BlackBoardParametor>
	{
	public:
		/// <summary>
		/// �X�e�[�g�}�V���̃X�e�[�g�^�C�v
		/// </summary>
		enum class State {
			TPS,    //�O�l�̎��_
			Look,   //����
			Tackle, //�^�b�N��
			Debug,  //�f�o�b�O
		};

		/// <summary>
		/// �X�e�[�g�}�V���̑J�ڗp�p�����[�^
		/// </summary>
		struct TransitionMember {

		};

		using StateMachine = EnemyMainStateMachine<GameObject, State, TransitionMember>;

	private:
		std::unique_ptr<GameTimer> m_tackleTimer;      //�^�b�N���o�ߎ��Ԍv���p�N���X
		std::unique_ptr<StateMachine> m_stateMachine;  //�X�e�[�g�}�V���N���X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		ChangeCameraManager(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate2() override;

	private:
		/// <summary>
		/// �m�[�h�̐���
		/// </summary>
		void CreateNode();

		/// <summary>
		/// �G�b�W�̐���
		/// </summary>
		void CreateEdge();

	public:
		/// <summary>
		/// �X�e�[�g�̕ύX
		/// </summary>
		/// <param name="state">�ύX����X�e�[�g</param>
		void ChangeState(const State state);

		/// <summary>
		/// ���݂̃X�e�[�g���擾
		/// </summary>
		State GetNowState() const noexcept;
	};

}

//endbasecross