/*!
@file Pusher.h
@brief Pusherなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "ReactiveBool.h"

#include "PressData.h"
#include "MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;
	struct PressData;

	//--------------------------------------------------------------------------------------
	/// 潰すターゲットデータ
	//--------------------------------------------------------------------------------------
	struct PushTargetData {
		ex_weak_ptr<GameObject> target; //ターゲット
		CollisionPair collisionPair;    //衝突データ
		
		PushTargetData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="target">ターゲット</param>
		/// <param name="collisionPair">衝突データ</param>
		PushTargetData(const std::shared_ptr<GameObject>& target, const CollisionPair& collisionPair);
	};

	//--------------------------------------------------------------------------------------
	/// 潰す者
	//--------------------------------------------------------------------------------------
	class Pusher : public Component
	{
	public:
		/// <summary>
		/// パラメータ
		/// </summary>
		struct Parametor
		{
			float slowMoveSpeed = 0.0f;				   //スロー移動スピード
			Vec3 slowMoveDirect = Vec3(0.0f);		   //スロー移動方向
			float missPressSpeed = 0.0f;			   //失敗時の潰すスピード
			float missDeselerationTime = 0.5f;		   //失敗時の減速時間
			float missDeselerationPower = 20.0f;	   //失敗時の減速力
			float energyDebuffRate = 0.5f;			   //エナジーデバフ
			float energyDebuffTime = 1.0f;			   //デバフ時間
			float pushPower;						   //潰す力
			maru::ReactiveBool isPush = false;         //Pushしているかどうか。
			maru::ReactiveBool isPushing = false;      //プッシング中かどうか。
			maru::ReactiveBool isMissPressing = false; //MissPressing状態。
			std::shared_ptr<PressData> data;           //潰すデータ
			maru::Action<void(Pusher&)> destroyAction; //死亡時に呼び出すイベント

			Parametor();
		};

	private:
		Parametor m_param;                         //パラメータ
		std::vector<PushTargetData> m_targetDatas; //ターゲットデータ配列

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		Pusher(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// タックルステートのチェック
		/// </summary>
		void CheckTackleState();

		/// <summary>
		/// 潰し失敗時の更新処理
		/// </summary>
		void MissPressUpdate();

		/// <summary>
		/// 移動更新
		/// </summary>
		/// <param name="speed">移動スピード</param>
		/// <param name="direct">移動方向</param>
		void MoveUpdate(const float& speed, const Vec3& direct);

		/// <summary>
		/// 潰している時の更新処理
		/// </summary>
		void PressingUpdate();

		/// <summary>
		/// 潰すターゲットに潰していることを伝える。
		/// </summary>
		void PushTargetUpdate();

		/// <summary>
		/// 潰しが終わった時に呼び出す処理
		/// </summary>
		/// <param name="target">潰し終わったターゲット</param>
		void EndPush(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// 自分が潰せる状態か判断
		/// </summary>
		/// <returns>潰せる状態ならtrue</returns>
		bool IsPush() const;

		/// <summary>
		/// ターゲットを追加できるかどうか
		/// </summary>
		/// <returns>追加できるならtrue</returns>
		bool IsAddTarget(const std::shared_ptr<GameObject>& target) const;

		/// <summary>
		/// Pusherに当たった時の処理
		/// </summary>
		/// <param name="pair">当たった相手のデータ</param>
		void HitPusher(const CollisionPair& pair);

		/// <summary>
		/// 減速開始
		/// </summary>
		void StartDeseleration();

		/// <summary>
		/// Pushing中に障害物にヒットした時
		/// </summary>
		void HitPushingObstacle(const CollisionPair& pair);

		/// <summary>
		/// 当たったときの処理
		/// </summary>
		/// <param name="pair">当たった相手</param>
		void OnCollisionEnter(const CollisionPair& pair) override;

		/// <summary>
		/// 当たり続けている間呼ぶ処理
		/// </summary>
		/// <param name="pair">当たった相手</param>
		void OnCollisionExcute(const CollisionPair& pair) override;

		/// <summary>
		/// タックルスピードの取得
		/// </summary>
		/// <returns>タックルスピード</returns>
		float GetTackleSpeed() const;

	public:
		/// <summary>
		/// ターゲットのプッシュを始める。
		/// </summary>
		/// <param name="pair">当たった相手</param>
		void StartPush(const CollisionPair& pair);

		/// <summary>
		/// 押しつぶした時の反動
		/// </summary>
		/// <param name="backVec">押しつぶした時の反動ベクトル</param>
		void PushBack(const Vec3& backVec);

		/// <summary>
		/// 押しつぶしを開始(StartPushでPush候補に入れたもの全て入れる)
		/// </summary>
		/// <param name="data">潰すデータ</param>
		void SuccessPressing(const PressData& data);

		/// <summary>
		/// 押しつぶしの失敗スタート
		/// </summary>
		/// <param name="pair">当たった相手</param>
		void MissPressing(const CollisionPair& pair);

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットデータの追加
		/// </summary>
		/// <param name="data">ターゲットデータ</param>
		void AddPushTargetData(const PushTargetData& data) {
			if (IsAddTarget(data.target.GetShard())) {
				m_targetDatas.push_back(data);
			}
		}

		/// <summary>
		///	潰している状態の設定
		/// </summary>
		/// <param name="isPush">潰している状態</param>
		void SetIsPush(const bool isPush) {
			m_param.isPush = isPush;
		}

		/// <summary>
		/// 潰している状態が切り替わったときに呼び出したいイベントの追加
		/// </summary>
		/// <param name="b">どっちの状態に切り替わったときに呼び出したいか</param>
		/// <param name="function">呼び出したいイベント</param>
		void AddReactiveIsPusherFunction(const bool b, const std::function<void()>& function);

		/// <summary>
		/// 潰し失敗状態かどうかを取得
		/// </summary>
		/// <returns>潰し失敗状態ならtrue</returns>
		bool IsMissPressing() const {
			return m_param.isMissPressing;
		}

		/// <summary>
		/// 潰し成功データの設定
		/// </summary>
		/// <param name="data">潰し成功データ</param>
		void SetSuccessPressData(const PressData& data);

		/// <summary>
		/// 潰し成功データの取得
		/// </summary>
		/// <returns>潰し成功データ</returns>
		PressData GetSuccessPressData() const;

		/// <summary>
		/// 死亡イベントの追加
		/// </summary>
		/// <param name="action">死亡イベント</param>
		void AddDestroyAction(const std::function<void(Pusher&)>& action);

	};

}

//endbasecross