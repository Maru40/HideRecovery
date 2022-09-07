/*!
@file MaruAnimationHelper.h
@brief MaruAnimationHelperなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/StateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
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
		///	モーションインターフェース
		//--------------------------------------------------------------------------------------
		class I_Motion
		{
		public:
			/// <summary>
			/// 開始処理
			/// </summary>
			virtual void OnStart() = 0;

			/// <summary>
			/// 更新処理
			/// </summary>
			/// <param name="speed">更新スピード(Default == 1.0f)</param>
			/// <returns>更新が終了しているならtrue</returns>
			virtual bool OnUpdate(const float& speed = 1.0f) = 0;

			/// <summary>
			/// 終了処理
			/// </summary>
			virtual void OnExit() = 0;

			/// <summary>
			/// アニメーションの再生にかかる時間を取得
			/// </summary>
			/// <returns>アニメーションの再生にかかる時間</returns>
			virtual float GetTime() const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	モーション基底クラス
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class MotionBase : public I_Motion 
		{
		private:
			float m_time = 0.0f;            //モーションの全体時間
			ex_weak_ptr<OwnerType> m_owner; //所有者

		public:
			/// <summary>
			/// コンストラクタｎ
			/// </summary>
			/// <param name="owner">所有者のポインタ</param>
			MotionBase(const std::shared_ptr<OwnerType>& owner)
				:m_owner(owner), m_time(0.0f)
			{}

		protected:
			/// <summary>
			/// アニメーションにかかる時間をセッティングする。
			/// </summary>
			/// <param name="params">アニメーション再生用のデータ</param>
			void SettingTime(const vector<basecross::Metaball::Task::SeekOffsetMove_Parametor>& params) {
				m_time = 0.0f;
				for (const auto& param : params) {
					m_time += param.time;
				}
			}

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 所有者の取得
			/// </summary>
			/// <returns>所有者</returns>
		 	std::shared_ptr<OwnerType> GetOwner() const {
				return m_owner.GetShard();
			}

			float GetTime() const override final{
				return m_time;
			}
		};

		//--------------------------------------------------------------------------------------
		///	メタボールモーションの初期データ
		//--------------------------------------------------------------------------------------
		struct MetaballMotion_InitializeData {
			float radius;               //半径
			std::vector<Vec3> offsets;  //オフセット配列
		};

		//--------------------------------------------------------------------------------------
		///	メタボールを利用したモーションの基底クラス
		//--------------------------------------------------------------------------------------
		template<class OwnerType>
		class MetaballMotionBase : public MotionBase<OwnerType>
		{
		private:
			//初期状態のモデルデータを保存
			MetaballMotion_InitializeData m_initializeData;

		protected:
			//モーション管理クラス
			std::unique_ptr<basecross::Metaball::MotionManager> m_motionManager = nullptr;

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">所有者のポインタ</param>
			MetaballMotionBase(const std::shared_ptr<OwnerType>& owner)
				:MotionBase(owner), m_motionManager(new Metaball::MotionManager(owner->GetGameObject()))
			{
				auto render = GetOwner()->GetGameObject()->GetComponent<Metaball::RenderBase>(false);
				if (render) { //nullCheck
					//オフセットの取得
					m_initializeData.radius = render->GetChildren()[0]->GetRadius();
					m_initializeData.offsets = render->CalculateChildrenSeekOffsets();
				}
			}

			void OnStart() override final {
				//モーション管理クラスにモーションデータを渡す。
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
			/// モーション再生用のパラメータ群を返す。
			/// </summary>
			/// <returns>モーション再生用のパラメータ群</returns>
			virtual vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() = 0;

			/// <summary>
			/// 初期半径の取得
			/// </summary>
			/// <returns>初期半径</returns>
			float GetInitializeRadius() const { return m_initializeData.radius; }

			/// <summary>
			/// 初期オフセット配列を取得
			/// </summary>
			/// <returns>初期オフセット配列</returns>
			std::vector<Vec3> GetInitializeOffsets() const { return m_initializeData.offsets; }
		};

		//--------------------------------------------------------------------------------------
		///	アニメーションイベント構造体
		//--------------------------------------------------------------------------------------
		struct AnimationTimeEvent
		{
			float time;                   //呼び出す時間
			std::function<void()> event;  //呼び出すイベント。
			bool isEnd;                   //呼び出された後かどうかを判断する。

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">呼び出す時間</param>
			/// <param name="event">呼び出すイベント</param>
			AnimationTimeEvent(const float& time, const std::function<void()> event);
		};

		//--------------------------------------------------------------------------------------
		///	アニメーションクリップクラス
		//--------------------------------------------------------------------------------------
		class AnimationClip : public I_StateNode
		{
			std::shared_ptr<I_Motion> m_motion;                //モーションデータ
			bool m_isLoop;                                     //ループするかどうか
			float m_timeSpeed;                                 //アニメーションの再生スピード
			vector<AnimationTimeEvent> m_timeEvents;           //アニメーションのタイムイベント
			vector<std::shared_ptr<I_StateNode>> m_stateNodes; //アニメーションクリップに後付けで処理を付与したい時用

			std::unique_ptr<GameTimer> m_timer = nullptr;      //タイマー管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="motion">モーションインターフェース</param>
			AnimationClip(const std::shared_ptr<I_Motion>& motion);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="motion">モーションインターフェース</param>
			/// <param name="isLoop">ループ判断</param>
			AnimationClip(const std::shared_ptr<I_Motion>& motion, const bool isLoop);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="motion">モーションインターフェース</param>
			/// <param name="isLoop">ループ判断</param>
			/// <param name="timeSpeed">モーション再生スピード</param>
			AnimationClip(const std::shared_ptr<I_Motion>& motion, const bool isLoop, const float& timeSpeed);

			virtual void OnStart() override;
			virtual bool OnUpdate() override;
			virtual void OnExit() override;

		private:
			/// <summary>
			/// モーションのアップデート
			/// </summary>
			void MotionUpdate();

			/// <summary>
			/// タイムイベント管理
			/// </summary>
			void TimeEventManager();

			/// <summary>
			/// 時間が経過しきったら呼び出す処理
			/// </summary>
			void TimerEndFunction(); 

			/// <summary>
			/// 時間終了時に呼び出されていなかったタイムイベントを呼ぶ処理
			/// </summary>
			void ResetTimeEvents();

		public:
			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// タイムイベントの追加
			/// </summary>
			/// <param name="time">呼び出す時間</param>
			/// <param name="event">呼び出すイベント</param>
			void AddTimeEvent(const float& time, const std::function<void()>& event);

			/// <summary>
			/// 後付けステートノードの追加
			/// </summary>
			/// <param name="stateNode">追加したいステートノード</param>
			void AddStateNode(const std::shared_ptr<I_StateNode>& stateNode);

			/// <summary>
			/// ループ判断
			/// </summary>
			/// <returns>ループするならtrue</returns>
			bool IsLoop() const noexcept;

			/// <summary>
			/// 終了判断
			/// </summary>
			/// <returns>終了しているならtrue</returns>
			bool IsEnd() const;

		};

		//--------------------------------------------------------------------------------------
		///	アニメーションステートノードのインターフェース
		//--------------------------------------------------------------------------------------
		class I_AnimationStateNode : public I_StateNode
		{
		public:
			/// <summary>
			/// アニメーションクリップの取得
			/// </summary>
			/// <returns>アニメーションクリップ</returns>
			virtual std::shared_ptr<AnimationClip> GetAnimationClip() const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	アニメーションステートノードの基底クラス
		//--------------------------------------------------------------------------------------
		class AnimationStateNodeBase : public I_AnimationStateNode
		{
			//アニメーションクリップ
			std::shared_ptr<AnimationClip> m_animationClip = nullptr;

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="animationClip">アニメーションクリップ</param>
			AnimationStateNodeBase(const std::shared_ptr<AnimationClip>& animationClip);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

			std::shared_ptr<AnimationClip> GetAnimationClip() const noexcept override;
		};

	}
	
}