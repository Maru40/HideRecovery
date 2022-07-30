
/*!
@file ClearDirection.h
@brief ClearDirection�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class SpriteObject;
	class ClearUIObject;

	namespace Task {
		struct Wait_Parametor;
		struct InsertUI_Parametor;
	}

	struct ScalePop_Parametor;

	//--------------------------------------------------------------------------------------
	/// �N���A���o�p�����[�^
	//--------------------------------------------------------------------------------------
	struct ClearDirection_Parametor {
		float slowTime;                                                        //Slow�ɂ��鎞��
		float slowTimeSpeed;                                                   //Slow�̃X�s�[�h
		std::shared_ptr<Task::Wait_Parametor> beforeInsetUIStartWaitParamPtr;  //�J�n����܂ł̑ҋ@����
		float fadeInBackSpriteTime;                                            //�t�F�[�h�C���ɂ����鎞��
		std::shared_ptr<Task::InsertUI_Parametor> insertUIParamPtr;            //UI��\������܂ł̃p�����[�^
		std::shared_ptr<Task::Wait_Parametor> endWaitParamPtr;                 //UI��\�����Ă���A���S�Ƀ^�X�N���I������܂ł̎��ԁB

		ClearDirection_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// �N���A���o�Ǘ�
	//--------------------------------------------------------------------------------------
	class ClearDirection : public Component
	{
	public:
		using Parametor = ClearDirection_Parametor;

	private:
		/// <summary>
		/// �^�X�N�^�C�v
		/// </summary>
		enum class TaskEnum {
			Wait_BeforeInsertUI, //UI��\������O�̑ҋ@
			FadeInBackSprite,    //�o�b�N�X�v���C�g���t�F�[�h�C��������
			InsertUI,            //ClearUI��\��
			Wait_End,            //UI�\����̑ҋ@
			ChangeStageStart,    //�X�e�[�g�J�ڂ��J�n����
		};

		Parametor m_param;                               //�p�����[�^
		std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //�^�X�N�Ǘ��N���X
		
		ex_weak_ptr<ClearUIObject> m_clearUI;            //�N���AUI

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		ClearDirection(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		ClearDirection(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// ���o�J�n
		/// </summary>
		void Start();

	private:
		/// <summary>
		/// �^�X�N�̒�`
		/// </summary>
		void DefineTask();

		/// <summary>
		/// �^�X�N�̑I��
		/// </summary>
		void SelectTask();

		/// <summary>
		/// �N���AUI�̐���
		/// </summary>
		void CreateClearUI();

	};

	//--------------------------------------------------------------------------------------
	/// �^�X�N�̎���
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// UI�̑}���p�����[�^
		//--------------------------------------------------------------------------------------
		struct InsertUI_Parametor {
			std::shared_ptr<ScalePop_Parametor> scalePopParamPtr; //�X�P�[���ύX�p�����[�^

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			InsertUI_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scalePopParamPtr">�X�P�[���ύX�p�����[�^</param>
			InsertUI_Parametor(const std::shared_ptr<ScalePop_Parametor>& scalePopParamPtr);
		};

		//--------------------------------------------------------------------------------------
		/// UI�̑}���^�X�N
		//--------------------------------------------------------------------------------------
		class InsertUI : public TaskNodeBase<SpriteObject>
		{
		public:
			using Parametor = InsertUI_Parametor;

		private:
			ex_weak_ptr<Parametor> m_paramPtr; //�p�����[�^

		public:
			/// <summary>
			/// 
			/// </summary>
			InsertUI(const std::shared_ptr<SpriteObject> owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};
	}

}