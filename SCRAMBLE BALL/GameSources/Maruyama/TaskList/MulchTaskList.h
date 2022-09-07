/*!
@file TaskList.h
@brief TaskList
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"

#include "TaskList.h"

#include "DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����̃^�X�N�𓯎��ɃA�b�v�f�[�g����N���X
	//--------------------------------------------------------------------------------------
	template<class EnumType>
	class MulchTaskList
	{
	public:
		/// <summary>
		/// �^�X�N�N���X
		/// </summary>
		class Task
		{
			bool m_isEnd = false;				//�I�����Ă��邩�ǂ���
			bool m_isKey = false;				//�L�[�^�X�N���ǂ���
			ex_weak_ptr<I_TaskNode> m_taskNode;	//�^�X�N�m�[�h

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="taskNode">�^�X�N�m�[�h</param>
			/// <param name="isKey">�L�[�^�X�N���ǂ���</param>
			Task(const std::shared_ptr<I_TaskNode>& taskNode, const bool isKey = false)
				:m_taskNode(taskNode), m_isKey(isKey)
			{}

			/// <summary>
			/// �J�n���C�x���g
			/// </summary>
			void OnStart() { m_taskNode->OnStart(); }

			/// <summary>
			/// �X�V�C�x���g
			/// </summary>
			/// <returns>�X�V�������I�����Ă��邩�ǂ���</returns>
			bool OnUpdate() {
				if (m_isEnd) {
					return m_isEnd;
				}

				m_isEnd = m_taskNode->OnUpdate();

				return m_isEnd;
			}

			/// <summary>
			/// �I�����C�x���g
			/// </summary>
			void OnExit() { m_taskNode->OnExit(); }

			/// <summary>
			/// �^�X�N���I�����Ă��邩�ǂ���
			/// </summary>
			/// <returns>�^�X�N���I�����Ă�����true</returns>
			bool IsEnd() const noexcept { return m_isEnd; }

			/// <summary>
			/// �L�[�^�X�N���ǂ���
			/// </summary>
			/// <returns>�L�[�^�X�N�Ȃ�true</returns>
			bool IsKey() const noexcept { return m_isKey; }
		};

	private:
		//��`���ꂽ�^�X�N
		std::map<EnumType, std::shared_ptr<I_TaskNode>> m_defineTaskMap;

		//���݋l�܂�Ă���^�X�N
		vector<vector<std::shared_ptr<Task>>> m_currentTaskLists;

		//���݂̃C���f�b�N�X
		int m_currentIndex = 0; 

	public:

		/// <summary>
		/// �A�b�v�f�[�g����(�O������Ăяo���B)
		/// </summary>
		void UpdateTask() {
			if (IsEnd()) {
				return;
			}

			auto curretTasks = m_currentTaskLists[m_currentIndex];

			for (auto& task : curretTasks) {
				task->OnUpdate();
			}

			if (IsNextTask(curretTasks)) {
				NextTask(curretTasks);
			}
		}

	private:

		/// <summary>
		/// �^�X�N����`����Ă��邩�ǂ������f
		/// </summary>
		/// <param name="type">�^�X�N�^�C�v</param>
		/// <returns>��`����Ă���Ȃ�true</returns>
		bool IsDefineTask(const EnumType& type) {
			if (!m_defineTaskMap.count(type)) {
				DebugObject::AddString(L"�^�X�N���o�^����Ă��܂���");
				return false;
			}

			return true;
		}

		/// <summary>
		/// ���̃^�X�N�Ɉڍs���邩�ǂ���
		/// </summary>
		/// <param name="baseCurrentTasks">���ݐς܂�Ă���^�X�N</param>
		/// <returns>���̃^�X�N�Ɉڍs����Ȃ�true</returns>
		bool IsNextTask(const vector<std::shared_ptr<Task>>& baseCurrentTasks) {
			vector<std::shared_ptr<Task>> currentTasks = CalcuKeyTask(baseCurrentTasks);

			//Key�^�X�N���Ȃ��Ȃ�
			if (currentTasks.size() == 0) {      
				currentTasks = baseCurrentTasks; //�S�Ẵ^�X�N��Ώۂɂ���B 
			}

			for (const auto& task : currentTasks) {
				if (!task->IsEnd()) { //��ł��I�����ĂȂ������玟�̃^�X�N�ɂ��Ȃ��B
					return false;
				}
			}

			return true;
		}

		/// <summary>
		/// �L�[�^�X�N���擾����
		/// </summary>
		/// <param name="currentTasks">���ݐς܂�Ă���^�X�N</param>
		/// <returns>�L�[�^�X�N</returns>
		vector<std::shared_ptr<Task>> CalcuKeyTask(const vector<std::shared_ptr<Task>>& currentTasks) {
			vector<std::shared_ptr<Task>> resultTasks;
			for (const auto& task : currentTasks) {
				if (task->IsKey()) {
					resultTasks.push_back(task);
				}
			}

			return resultTasks;
		}

		/// <summary>
		/// ���̃^�X�N�ɂ���
		/// </summary>
		/// <param name="currentTasks">���ݐς܂�Ă���^�X�N</param>
		void NextTask(const vector<std::shared_ptr<Task>>& currentTasks) {
			for (const auto& task : currentTasks) {
				task->OnExit();
			}

			m_currentIndex++;

			if (IsEnd()) { //�I�����Ă�����
				m_currentIndex = 0;
				m_currentTaskLists.clear();
				return;
			}

			for (const auto& task : m_currentTaskLists[m_currentIndex]) {
				task->OnStart();
			}
		}

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�X�N�̒�`
		/// </summary>
		/// <param name="type">�^�X�N�^�C�v</param>
		/// <param name="taskNode">��`����^�X�N�m�[�h</param>
		void DefineTask(const EnumType& type, const std::shared_ptr<I_TaskNode>& taskNode) {
			if (m_defineTaskMap.count(type)) {  //�L�[�����݂���Ȃ�
				DebugObject::AddString(L"���ɒǉ�����Ă��܂��B");
				return;
			}

			m_defineTaskMap[type] = taskNode;
		}

		/// <summary>
		/// ��`���ꂽ�^�X�N���擾
		/// </summary>
		/// <param name="type">�^�X�N�^�C�v</param>
		template<class T>
		std::shared_ptr<T> GetDefineTask(const EnumType& type) {
			if (IsDefineTask(type)) {
				auto t = dynamic_pointer_cast<T>(m_defineTaskMap[type]);
				return t;
			}

			return nullptr;
		}

		/// <summary>
		/// �^�X�N�̒ǉ�
		/// </summary>
		/// <param name="types">�ǉ�����^�X�N�z��</param>
		void AddTask(const vector<EnumType>& types) {
			vector<std::shared_ptr<Task>> addTasks;
			bool isFirstTask = m_currentTaskLists.size() == 0; //�ŏ��̃^�X�N���ǂ���
			for (const auto& type : types) {
				if (IsDefineTask(type)) { //�^�X�N����`����Ă�����
					auto task = std::make_shared<Task>(m_defineTaskMap[type]);
					addTasks.push_back(task);  //�^�X�N�̒ǉ�
					if (isFirstTask) { //�ŏ��̃^�X�N�Ȃ�
						task->OnStart();
					}
				}
			}

			m_currentTaskLists.push_back(addTasks);
		}

		/// <summary>
		/// �^�X�N�̋������Z�b�g
		/// </summary>
		void ForceReset() {
			m_currentIndex = 0;
			m_currentTaskLists.clear();
		}

		/// <summary>
		/// �I�����ǂ�����Ԃ�
		/// </summary>
		/// <returns>�ς܂�Ă���^�X�N�����݂��Ȃ��Ȃ�true</returns>
		bool IsEnd() {
			return m_currentIndex >= m_currentTaskLists.size() ? true : false;
		}

	};

}

//endbasecross