/*!
@file TaskList.h
@brief TaskList
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Watanabe/DebugClass/Debug.h"
#include "Maruyama/DebugClass/Object/DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�^�X�N�m�[�h�̃C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_TaskNode
	{
	public:
		/// <summary>
		/// �������Ɉ�x�����Ăяo������
		/// </summary>
		virtual void OnCreate() {}

		/// <summary>
		/// �J�n����
		/// </summary>
		virtual void OnStart() = 0;

		/// <summary>
		/// �X�V����
		/// </summary>
		/// <returns></returns>
		virtual bool OnUpdate() = 0;

		/// <summary>
		/// �I������
		/// </summary>
		virtual void OnExit() = 0;
	};

	//--------------------------------------------------------------------------------------
	///	�^�X�N�m�[�h�̊��N���X
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskNodeBase : public I_TaskNode 
	{
		std::weak_ptr<T> m_owner;	//���̃N���X�����L����N���X
		bool m_isActive;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="owner">���̃^�X�N�����L����N���X</param>
		TaskNodeBase(const std::shared_ptr<T>& owner)
			:m_owner(owner), m_isActive(true)
		{}

	protected:
		/// <summary>
		/// ���̃N���X�����L����N���X�̎擾
		/// </summary>
		/// <returns>���̃N���X�����L����N���X</returns>
		std::shared_ptr<T> GetOwner() const {
			return m_owner.lock();
		}

	public:
		void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

		bool IsActive() const noexcept { return m_isActive; }
	};

	//--------------------------------------------------------------------------------------
	///	�^�X�N�m�[�h�̊��N���X�̊g��
	/// (�J�n�A�X�V�A�I���A���ꂼ��̃C�x���g��ǉ����邱�Ƃ��ł���)
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskNodeBase_Ex : TaskNodeBase<T>
	{
	public:

		struct Parametor {
			std::function<void()> start;   //�J�n�C�x���g
			std::function<void()> update;  //�X�V�C�x���g
			std::function<void()> exit;    //�I���C�x���g
		};

	protected:
		Parametor m_funcParam;	//�ǉ��C�x���g�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="owner">���̃N���X�����L����N���X</param>
		/// <param name="param"></param>
		TaskNodeBase_Ex(const std::shared_ptr<T>& owner, const Parametor& param)
			:TaskNodeBase(owner), m_funcParam(param)
		{ }

		void OnStart() override {
			if (m_funcParam.start) {
				m_funcParam.start();
			}
		}

		bool OnUpdate() override {
			if (m_funcParam.update) {
				m_funcParam.update();
			}

			return true;
		}

		void OnExit() override {
			if (m_funcParam.exit) {
				m_funcParam.exit();
			}
		}
	};

	template<class OwnerType, class BlackBoard>
	class TaskNodeBase_WithBlackBoard : public TaskNodeBase<OwnerType>
	{
		std::function<BlackBoard()> m_getBlackBoardFunction = nullptr;

	public:
		TaskNodeBase_WithBlackBoard(
			const std::shared_ptr<OwnerType>& owner,
			const std::function<BlackBoard()>& getBlackBoardFunction
		) :
			TaskNodeBase<OwnerType>(owner),
			m_getBlackBoardFunction(getBlackBoardFunction)
		{}

		/// <summary>
		/// �u���b�N�{�[�h�̎擾
		/// </summary>
		/// <returns>�u���b�N�{�[�h</returns>
		BlackBoard GetBlackBoard() const { return m_getBlackBoardFunction(); }

		/// <summary>
		/// �u���b�N�{�[�h�̎Q�Ƃ��擾
		/// </summary>
		/// <returns>�u���b�N�{�[�h�̎Q��</returns>
		BlackBoard& GetRefBlackBoard() { return m_getBlackBoardFunction(); }
	};

	//--------------------------------------------------------------------------------------
	///	�^�X�N�Ǘ��̃C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_TaskList
	{
		/// <summary>
		/// �^�X�N�̍X�V
		/// </summary>
		virtual void UpdateTask() = 0;

		/// <summary>
		/// �^�X�N�̏I�����f
		/// </summary>
		/// <returns>�I����ԂȂ�true</returns>
		virtual bool IsEnd() = 0;
	};

	//--------------------------------------------------------------------------------------
	///	�^�X�N�Ǘ��N���X
	//--------------------------------------------------------------------------------------
	template<class EnumType>
	class TaskList : public I_TaskList
	{
	public:

		/// <summary>
		/// �^�X�N�\����
		/// </summary>
		struct Task {
			EnumType type;                 //�^�X�N�̎��
			std::function<void()> start;   //�J�n�C�x���g
			std::function<bool()> update;  //�X�V�C�x���g
			std::function<void()> exit;	   //�I���C�x���g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="type">�^�X�N�̎��</param>
			/// <param name="start">�J�n�C�x���g</param>
			/// <param name="update">�X�V�C�x���g</param>
			/// <param name="exit">�I���C�x���g</param>
			Task(const EnumType type,
				const std::function<void()>& start,
				const std::function<bool()>& update,
				const std::function<void()>& exit
			) :
				type(type), start(start), update(update), exit(exit)
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="type">�^�X�N�̎��</param>
			/// <param name="taskNode">�^�X�N�m�[�h�̃C���^�\�t�F�[�X</param>
			Task(const EnumType type, I_TaskNode& taskNode) :
				type(type),
				start([]() = { taskNode.OnStart(); }),
				update([]() = { taskNode.OnUpdate(); }),
				exit([]() = { taskNode.OnExit(); })
			{}
		};

	private:

		//��`���ꂽ�^�X�N
		std::map<EnumType, std::shared_ptr<Task>> m_defineTaskMap;
		
		//���ݐς܂�Ă���^�X�N
		std::vector<ex_weak_ptr<Task>> m_currentTasks;

		//���ݓ����Ă���^�X�N
		ex_weak_ptr<Task> m_currentTask;

		//���ݓ����Ă���^�X�N�̃C���f�b�N�X�B
		int m_currentIndex = 0;

	public:
		virtual ~TaskList() = default;

		/// <summary>
		/// ���t���[���Ăяo���֐�(�Ăяo���͊O���ˑ�)
		/// </summary>
		void UpdateTask() override {
			if (IsEnd()) { //�I�����Ă�����Update���Ă΂Ȃ�
				return;
			}

			//���ݓ��쒆�̃^�X�N��nullptr�Ȃ�B
			if (m_currentTask.GetShard() == nullptr) { 
				m_currentTask = m_currentTasks[m_currentIndex]; //���ݓ��쒆�̃^�X�N�̍X�V
				m_currentTask->start();  //�J�n�֐��̎���
			}

			//�^�X�N�̃A�b�v�f�[�g
			bool isEndOneTask = m_currentTask->update();

			//�^�X�N�̏I��������
			if (isEndOneTask) {
				EndOneTask();
			}
		}

	private:

		/// <summary>
		/// ��̃^�X�N���I�������Ƃ��ɌĂяo������
		/// </summary>
		void EndOneTask() {
			if (m_currentTask) {
				m_currentTask->exit(); //���݂̃^�X�N��Exit
			}

			m_currentIndex++;

			if (IsEnd()) {  //���̃^�X�N���Ȃ��Ȃ�B
				m_currentIndex = 0;
				m_currentTask = nullptr;
				m_currentTasks.clear();
				return;
			}

			m_currentTask = m_currentTasks[m_currentIndex]; //���̃^�X�N���擾
			if (m_currentTask) {
				m_currentTask->start();  //���̃^�X�N��Enter()
			}
		}

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�X�N�̒�`
		/// </summary>
		/// <param name="type">�^�X�N�̃^�C�v</param>
		/// <param name="task">�^�X�N�̃N���X</param>
		void DefineTask(const EnumType type, const std::shared_ptr<I_TaskNode>& task) {
			task->OnCreate();

			DefineTask(type, 
				[&, task]() { task->OnStart(); },
				[&, task]() { return task->OnUpdate(); },
				[&, task]() { task->OnExit(); }
			);
		}

		/// <summary>
		/// �^�X�N�̒�`
		/// </summary>
		/// <param name="type">�^�X�N�̎��</param>
		/// <param name="start">�J�n�C�x���g</param>
		/// <param name="update">�X�V�C�x���g</param>
		/// <param name="exit">�I���C�x���g</param>
		void DefineTask(const EnumType type, 
			const std::function<void()>& start,
			const std::function<bool()>& update,
			const std::function<void()>& exit)
		{
			auto task = std::make_shared<Task>(type, start, update, exit);
			if (m_defineTaskMap.count(type)) {  //�L�[�����݂���Ȃ�
				DebugObject::AddString(L"���ɒǉ�����Ă��܂��B");
				return;
			}
			m_defineTaskMap[type] = task;
		}

		/// <summary>
		/// �^�X�N�̒�`
		/// </summary>
		/// <typeparam name="T">�^�X�N�N���X�^�C�v</typeparam>
		/// <param name="type">�^�X�N�^�C�v</param>
		/// <param name="...params">�p�����[�^�p�b�N</param>
		template<class T, class... Ts,
			std::enable_if_t<
				std::is_base_of_v<I_TaskNode, T> &&
				std::is_constructible_v<T, Ts...>,
			std::nullptr_t> = nullptr
		>
		void DefineTask(const EnumType type, Ts&&... params) {
			auto task = std::make_shared<T>(params...);
			DefineTask(type, task);
		}

		/// <summary>
		/// �^�X�N�̓o�^
		/// </summary>
		/// <param name="type">�^�X�N�̎��</param>
		void AddTask(const EnumType type) {
			if (!m_defineTaskMap.count(type)) {
				Debug::GetInstance()->Log(L"�^�X�N���o�^����Ă��܂���B");
				//DebugObject::AddString(L"�^�X�N���o�^����Ă��܂���");
				return;
			}

			m_currentTasks.push_back(m_defineTaskMap[type]);
		}

		/// <summary>
		/// �����I��
		/// </summary>
		/// <param name="isExitFunc">�I���C�x���g���ĂԂ��ǂ���</param>
		void ForceStop(const bool isExitFunc = true) {
			//isExitFunc��true�ŁA�J�����g�^�X�N��nullptr�łȂ�������
			if (isExitFunc && m_currentTask.GetShard() != nullptr) { 
				m_currentTask->exit();
			}
			m_currentTask = nullptr;
			m_currentTasks.clear();
			m_currentIndex = 0;
		}

		/// <summary>
		/// �����I�Ɏ��̃^�X�N�ɕύX����B
		/// </summary>
		void ForceNextTask() {
			EndOneTask();
		}

		/// <summary>
		/// �I�����Ă��邩�ǂ���
		/// </summary>
		/// <returns>�I�����Ă���Ȃ�true</returns>
		bool IsEnd() override {
			return m_currentTasks.size() <= m_currentIndex ? true : false;
		}

		/// <summary>
		/// �^�X�N�������Ă��邩�ǂ���
		/// </summary>
		/// <returns>�����Ă���Ȃ�true</returns>
		bool IsMoveTask() {
			return m_currentTask != nullptr;  //�J�����g�^�X�N����������true
		}

		/// <summary>
		/// ���ݐi�s���̃^�X�N�̎�ނ��擾
		/// </summary>
		/// <returns>���ݐi�s���̃^�X�N�̎��</returns>
		EnumType GetCurrentTaskType() {
			return m_currentTask->type;
		}

		/// <summary>
		/// �ǉ�����Ă���^�X�N�̃^�C�v���X�g���擾
		/// </summary>
		/// <returns>�ǉ�����Ă���^�X�N�̃^�C�v���X�g</returns>
		std::vector<EnumType> GetCurrentTaskTypeList() {
			std::vector<EnumType> types;
			for (auto task : m_currentTasks) {
				types.push_back(task->type);
			}

			return types;
		}

		/// <summary>
		/// ���݂̃C���f�b�N�X�̎擾
		/// </summary>
		/// <returns>���݂̃C���f�b�N�X</returns>
		int GetCurrentIndex() {
			return m_currentIndex;
		}
	};

}

//endbasecross