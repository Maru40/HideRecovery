/*!
@file TaskList.h
@brief TaskList
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"

#include "TaskList.h"

#include "DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	複数のタスクを同時にアップデートするクラス
	//--------------------------------------------------------------------------------------
	template<class EnumType>
	class MulchTaskList
	{
	public:
		/// <summary>
		/// タスククラス
		/// </summary>
		class Task
		{
			bool m_isEnd = false;				//終了しているかどうか
			bool m_isKey = false;				//キータスクかどうか
			ex_weak_ptr<I_TaskNode> m_taskNode;	//タスクノード

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="taskNode">タスクノード</param>
			/// <param name="isKey">キータスクかどうか</param>
			Task(const std::shared_ptr<I_TaskNode>& taskNode, const bool isKey = false)
				:m_taskNode(taskNode), m_isKey(isKey)
			{}

			/// <summary>
			/// 開始時イベント
			/// </summary>
			void OnStart() { m_taskNode->OnStart(); }

			/// <summary>
			/// 更新イベント
			/// </summary>
			/// <returns>更新処理が終了しているかどうか</returns>
			bool OnUpdate() {
				if (m_isEnd) {
					return m_isEnd;
				}

				m_isEnd = m_taskNode->OnUpdate();

				return m_isEnd;
			}

			/// <summary>
			/// 終了時イベント
			/// </summary>
			void OnExit() { m_taskNode->OnExit(); }

			/// <summary>
			/// タスクが終了しているかどうか
			/// </summary>
			/// <returns>タスクが終了していたらtrue</returns>
			bool IsEnd() const noexcept { return m_isEnd; }

			/// <summary>
			/// キータスクかどうか
			/// </summary>
			/// <returns>キータスクならtrue</returns>
			bool IsKey() const noexcept { return m_isKey; }
		};

	private:
		//定義されたタスク
		std::map<EnumType, std::shared_ptr<I_TaskNode>> m_defineTaskMap;

		//現在詰まれているタスク
		vector<vector<std::shared_ptr<Task>>> m_currentTaskLists;

		//現在のインデックス
		int m_currentIndex = 0; 

	public:

		/// <summary>
		/// アップデート処理(外部から呼び出す。)
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
		/// タスクが定義されているかどうか判断
		/// </summary>
		/// <param name="type">タスクタイプ</param>
		/// <returns>定義されているならtrue</returns>
		bool IsDefineTask(const EnumType& type) {
			if (!m_defineTaskMap.count(type)) {
				DebugObject::AddString(L"タスクが登録されていません");
				return false;
			}

			return true;
		}

		/// <summary>
		/// 次のタスクに移行するかどうか
		/// </summary>
		/// <param name="baseCurrentTasks">現在積まれているタスク</param>
		/// <returns>次のタスクに移行するならtrue</returns>
		bool IsNextTask(const vector<std::shared_ptr<Task>>& baseCurrentTasks) {
			vector<std::shared_ptr<Task>> currentTasks = CalcuKeyTask(baseCurrentTasks);

			//Keyタスクがないなら
			if (currentTasks.size() == 0) {      
				currentTasks = baseCurrentTasks; //全てのタスクを対象にする。 
			}

			for (const auto& task : currentTasks) {
				if (!task->IsEnd()) { //一つでも終了してなかったら次のタスクにしない。
					return false;
				}
			}

			return true;
		}

		/// <summary>
		/// キータスクを取得する
		/// </summary>
		/// <param name="currentTasks">現在積まれているタスク</param>
		/// <returns>キータスク</returns>
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
		/// 次のタスクにする
		/// </summary>
		/// <param name="currentTasks">現在積まれているタスク</param>
		void NextTask(const vector<std::shared_ptr<Task>>& currentTasks) {
			for (const auto& task : currentTasks) {
				task->OnExit();
			}

			m_currentIndex++;

			if (IsEnd()) { //終了していたら
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
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// タスクの定義
		/// </summary>
		/// <param name="type">タスクタイプ</param>
		/// <param name="taskNode">定義するタスクノード</param>
		void DefineTask(const EnumType& type, const std::shared_ptr<I_TaskNode>& taskNode) {
			if (m_defineTaskMap.count(type)) {  //キーが存在するなら
				DebugObject::AddString(L"既に追加されています。");
				return;
			}

			m_defineTaskMap[type] = taskNode;
		}

		/// <summary>
		/// 定義されたタスクを取得
		/// </summary>
		/// <param name="type">タスクタイプ</param>
		template<class T>
		std::shared_ptr<T> GetDefineTask(const EnumType& type) {
			if (IsDefineTask(type)) {
				auto t = dynamic_pointer_cast<T>(m_defineTaskMap[type]);
				return t;
			}

			return nullptr;
		}

		/// <summary>
		/// タスクの追加
		/// </summary>
		/// <param name="types">追加するタスク配列</param>
		void AddTask(const vector<EnumType>& types) {
			vector<std::shared_ptr<Task>> addTasks;
			bool isFirstTask = m_currentTaskLists.size() == 0; //最初のタスクかどうか
			for (const auto& type : types) {
				if (IsDefineTask(type)) { //タスクが定義されていたら
					auto task = std::make_shared<Task>(m_defineTaskMap[type]);
					addTasks.push_back(task);  //タスクの追加
					if (isFirstTask) { //最初のタスクなら
						task->OnStart();
					}
				}
			}

			m_currentTaskLists.push_back(addTasks);
		}

		/// <summary>
		/// タスクの強制リセット
		/// </summary>
		void ForceReset() {
			m_currentIndex = 0;
			m_currentTaskLists.clear();
		}

		/// <summary>
		/// 終了かどうかを返す
		/// </summary>
		/// <returns>積まれているタスクが存在しないならtrue</returns>
		bool IsEnd() {
			return m_currentIndex >= m_currentTaskLists.size() ? true : false;
		}

	};

}

//endbasecross