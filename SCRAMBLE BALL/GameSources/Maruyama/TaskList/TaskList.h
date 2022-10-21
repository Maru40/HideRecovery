/*!
@file TaskList.h
@brief TaskList
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/DebugClass/Object/DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	タスクノードのインターフェース
	//--------------------------------------------------------------------------------------
	class I_TaskNode
	{
	public:
		/// <summary>
		/// 開始処理
		/// </summary>
		virtual void OnStart() = 0;

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <returns></returns>
		virtual bool OnUpdate() = 0;

		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void OnExit() = 0;
	};

	//--------------------------------------------------------------------------------------
	///	タスクノードの基底クラス
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskNodeBase : public I_TaskNode 
	{
		std::weak_ptr<T> m_owner;	//このクラスを所有するクラス
		bool m_isActive;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">このタスクを所有するクラス</param>
		TaskNodeBase(const std::shared_ptr<T>& owner)
			:m_owner(owner), m_isActive(true)
		{}

	protected:
		/// <summary>
		/// このクラスを所有するクラスの取得
		/// </summary>
		/// <returns>このクラスを所有するクラス</returns>
		std::shared_ptr<T> GetOwner() {
			return m_owner.lock();
		}

	public:
		void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

		bool IsActive() const noexcept { return m_isActive; }
	};

	//--------------------------------------------------------------------------------------
	///	タスクノードの基底クラスの拡張
	/// (開始、更新、終了、それぞれのイベントを追加することができる)
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskNodeBase_Ex : TaskNodeBase<T>
	{
	public:

		struct Parametor {
			std::function<void()> start;   //開始イベント
			std::function<void()> update;  //更新イベント
			std::function<void()> exit;    //終了イベント
		};

	protected:
		Parametor m_funcParam;	//追加イベントパラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">このクラスを所有するクラス</param>
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
		/// ブラックボードの取得
		/// </summary>
		/// <returns>ブラックボード</returns>
		BlackBoard GetBlackBoard() const { return m_getBlackBoardFunction(); }

		/// <summary>
		/// ブラックボードの参照を取得
		/// </summary>
		/// <returns>ブラックボードの参照</returns>
		BlackBoard& GetRefBlackBoard() { return m_getBlackBoardFunction(); }
	};

	//--------------------------------------------------------------------------------------
	///	タスク管理のインターフェース
	//--------------------------------------------------------------------------------------
	class I_TaskList
	{
		/// <summary>
		/// タスクの更新
		/// </summary>
		virtual void UpdateTask() = 0;

		/// <summary>
		/// タスクの終了判断
		/// </summary>
		/// <returns>終了状態ならtrue</returns>
		virtual bool IsEnd() = 0;
	};

	//--------------------------------------------------------------------------------------
	///	タスク管理クラス
	//--------------------------------------------------------------------------------------
	template<class EnumType>
	class TaskList : public I_TaskList
	{
	public:

		/// <summary>
		/// タスク構造体
		/// </summary>
		struct Task {
			EnumType type;                 //タスクの種類
			std::function<void()> start;   //開始イベント
			std::function<bool()> update;  //更新イベント
			std::function<void()> exit;	   //終了イベント

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="type">タスクの種類</param>
			/// <param name="start">開始イベント</param>
			/// <param name="update">更新イベント</param>
			/// <param name="exit">終了イベント</param>
			Task(const EnumType type,
				const std::function<void()>& start,
				const std::function<bool()>& update,
				const std::function<void()>& exit
			) :
				type(type), start(start), update(update), exit(exit)
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="type">タスクの種類</param>
			/// <param name="taskNode">タスクノードのインタ―フェース</param>
			Task(const EnumType type, I_TaskNode& taskNode) :
				type(type),
				start([]() = { taskNode.OnStart(); }),
				update([]() = { taskNode.OnUpdate(); }),
				exit([]() = { taskNode.OnExit(); })
			{}
		};

	private:

		//定義されたタスク
		std::map<EnumType, std::shared_ptr<Task>> m_defineTaskMap;
		
		//現在積まれているタスク
		std::vector<ex_weak_ptr<Task>> m_currentTasks;

		//現在動いているタスク
		ex_weak_ptr<Task> m_currentTask;

		//現在動いているタスクのインデックス。
		int m_currentIndex = 0;

	public:
		virtual ~TaskList() = default;

		/// <summary>
		/// 毎フレーム呼び出す関数(呼び出しは外部依存)
		/// </summary>
		void UpdateTask() override {
			if (IsEnd()) { //終了していたらUpdateを呼ばない
				return;
			}

			//現在動作中のタスクがnullptrなら。
			if (m_currentTask.GetShard() == nullptr) { 
				m_currentTask = m_currentTasks[m_currentIndex]; //現在動作中のタスクの更新
				m_currentTask->start();  //開始関数の実装
			}

			//タスクのアップデート
			bool isEndOneTask = m_currentTask->update();

			//タスクの終了したら
			if (isEndOneTask) {
				EndOneTask();
			}
		}

	private:

		/// <summary>
		/// 一つのタスクが終了したときに呼び出す処理
		/// </summary>
		void EndOneTask() {
			if (m_currentTask) {
				m_currentTask->exit(); //現在のタスクのExit
			}

			m_currentIndex++;

			if (IsEnd()) {  //次のタスクがないなら。
				m_currentIndex = 0;
				m_currentTask = nullptr;
				m_currentTasks.clear();
				return;
			}

			m_currentTask = m_currentTasks[m_currentIndex]; //次のタスクを取得
			if (m_currentTask) {
				m_currentTask->start();  //次のタスクのEnter()
			}
		}

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// タスクの定義
		/// </summary>
		/// <param name="type">タスクのタイプ</param>
		/// <param name="task">タスクのクラス</param>
		void DefineTask(const EnumType type, const std::shared_ptr<I_TaskNode>& task) {
			DefineTask(type, 
				[&, task]() { task->OnStart(); },
				[&, task]() { return task->OnUpdate(); },
				[&, task]() { task->OnExit(); }
			);
		}

		/// <summary>
		/// タスクの定義
		/// </summary>
		/// <param name="type">タスクの種類</param>
		/// <param name="start">開始イベント</param>
		/// <param name="update">更新イベント</param>
		/// <param name="exit">終了イベント</param>
		void DefineTask(const EnumType type, 
			const std::function<void()>& start,
			const std::function<bool()>& update,
			const std::function<void()>& exit)
		{
			auto task = std::make_shared<Task>(type, start, update, exit);
			if (m_defineTaskMap.count(type)) {  //キーが存在するなら
				DebugObject::AddString(L"既に追加されています。");
				return;
			}
			m_defineTaskMap[type] = task;
		}

		/// <summary>
		/// タスクの登録
		/// </summary>
		/// <param name="type">タスクの種類</param>
		void AddTask(const EnumType type) {
			if (!m_defineTaskMap.count(type)) {
				DebugObject::AddString(L"タスクが登録されていません");
				return;
			}

			m_currentTasks.push_back(m_defineTaskMap[type]);
		}

		/// <summary>
		/// 強制終了
		/// </summary>
		/// <param name="isExitFunc">終了イベントを呼ぶかどうか</param>
		void ForceStop(const bool isExitFunc = true) {
			//isExitFuncがtrueで、カレントタスクがnullptrでなかったら
			if (isExitFunc && m_currentTask.GetShard() != nullptr) { 
				m_currentTask->exit();
			}
			m_currentTask = nullptr;
			m_currentTasks.clear();
			m_currentIndex = 0;
		}

		/// <summary>
		/// 強制的に次のタスクに変更する。
		/// </summary>
		void ForceNextTask() {
			EndOneTask();
		}

		/// <summary>
		/// 終了しているかどうか
		/// </summary>
		/// <returns>終了しているならtrue</returns>
		bool IsEnd() override {
			return m_currentTasks.size() <= m_currentIndex ? true : false;
		}

		/// <summary>
		/// タスクが動いているかどうか
		/// </summary>
		/// <returns>動いているならtrue</returns>
		bool IsMoveTask() {
			return m_currentTask != nullptr;  //カレントタスクが合ったらtrue
		}

		/// <summary>
		/// 現在進行中のタスクの種類を取得
		/// </summary>
		/// <returns>現在進行中のタスクの種類</returns>
		EnumType GetCurrentTaskType() {
			return m_currentTask->type;
		}

		/// <summary>
		/// 追加されているタスクのタイプリストを取得
		/// </summary>
		/// <returns>追加されているタスクのタイプリスト</returns>
		std::vector<EnumType> GetCurrentTaskTypeList() {
			std::vector<EnumType> types;
			for (auto task : m_currentTasks) {
				types.push_back(task->type);
			}

			return types;
		}

		/// <summary>
		/// 現在のインデックスの取得
		/// </summary>
		/// <returns>現在のインデックス</returns>
		int GetCurrentIndex() {
			return m_currentIndex;
		}
	};

}

//endbasecross