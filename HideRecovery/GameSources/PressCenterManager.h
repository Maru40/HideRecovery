/*!
@file PressCenterManager.h
@brief PressCenterManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "PressData.h"
#include "HitStopManager.h"

#include "ReactiveBool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;
	struct PressData;
	struct PressData::MissData;
	class Pusher;

	namespace maru {
		struct HitData;
		enum class DeltaType;
		class ReactiveBool;
	}

	//--------------------------------------------------------------------------------------
	/// 潰す中心管理
	//--------------------------------------------------------------------------------------
	class PressCenterManager : public Component
	{
	public:

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------
		struct Parametor
		{
			float searchRange;			      //壁との感知範囲
			float pressedRad;			      //Excerent判定角度
			float pressedGoodRad;		      //GoodRadの判定角度
			SlowTimeParametor slowParam;      //スロータイマーパラメータ
			HitStopManager::Data hitStopData; //ヒットストップデータ
			maru::ReactiveBool isPressed;     //プレイヤーなどに押されている状態
			PressData successPressData;       //潰し成功時データ

			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="searchRange">壁との感知範囲</param>
			/// <param name="pressedDegree">Excerent判定角度(デグリー角)</param>
			/// <param name="pressedGoodDegree">Good判定角度(デグリー角)</param>
			/// <param name="slowParam">スロータイマーパラメータ</param>
			/// <param name="hitStopData">ヒットストップデータ</param>
			/// <param name="isPressed">潰しが成功しているかどうか</param>
			Parametor(const float& searchRange, 
				const float& pressedDegree, const float& pressedGoodDegree,
				const SlowTimeParametor& slowParam,
				const HitStopManager::Data& hitStopData,
				const bool isPressed = false);
		};

	private:
		Parametor m_param;             //パラメータ

		ex_weak_ptr<Pusher> m_pusher;  //潰す側のポインタ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		PressCenterManager(const std::shared_ptr<GameObject>& objPtr);

	private:
		/// <summary>
		/// プレスデータの生成
		/// </summary>
		/// <param name="pair">衝突データ</param>
		/// <param name="missData">失敗データ</param>
		/// <param name="pressType">潰すタイプ</param>
		/// <returns>生成したプレスデータ</returns>
		PressData CreatePressData(const CollisionPair& pair, const PressData::MissData& missData, const PressType pressType);

		/// <summary>
		/// ミスデータの生成
		/// </summary>
		/// <param name="pair">衝突データ</param>
		/// <returns>生成したミスデータ</returns>
		PressData::MissData CreateMissData(const CollisionPair& pair);

		/// <summary>
		/// 近くの垂直な障害物を取得する。
		/// </summary>
		/// <returns>近くに垂直判定が取れる障害物を配列で返す。</returns>
		vector<maru::HitData> FindNearObstacleHitDatas();

		/// <summary>
		/// 障害物がヒット候補に入るかどうか
		/// </summary>
		/// <param name="data">ヒットデータ</param>
		/// <returns>障害物の候補ならtrue</returns>
		bool IsHitCandidate(const maru::HitData& data);

		/// <summary>
		/// 潰せる障害物かどうか
		/// </summary>
		/// <param name="object">対象のオブジェクト</param>
		/// <returns>障害物ならtrue</returns>
		bool IsHitObject(const std::shared_ptr<GameObject>& object);

		/// <summary>
		/// 潰せる挿入角かどうか
		/// </summary>
		/// <param name="type">潰すタイプ</param>
		/// <returns>潰せる挿入角ならtrue</returns>
		bool IsPusherAngle(const PressType type);

		/// <summary>
		/// カメラシェイクスタート
		/// </summary>
		/// <param name="time">シェイク時間</param>
		/// <param name="deltaType">経過時間タイプ</param>
		void StartCameraShake(const float& time, const maru::DeltaType& deltaType);

		/// <summary>
		/// 押しつぶし成功処理
		/// </summary>
		/// <param name="pair">衝突データ</param>
		/// <param name="pressType">潰すタイプ</param>
		void SuccessPressProcess(const CollisionPair& pair, const PressType pressType);

		/// <summary>
		/// 押しつぶし失敗処理
		/// </summary>
		/// <param name="pair">衝突データ</param>
		/// <param name="pressType">潰しタイプ</param>
		void MissPressProcess(const CollisionPair& pair, const PressType pressType);

		/// <summary>
		/// スライムに当たった時
		/// </summary>
		/// <param name="pair">衝突データ</param>
		void SlimeHit(const CollisionPair& pair);

		/// <summary>
		/// 障害物に当たった時に判定を取って、成功したら潰す。
		/// </summary>
		/// <param name="pair">衝突データ</param>
		void PressObstacleHit(const CollisionPair& pair);

		/// <summary>
		/// ヒット判定
		/// </summary>
		/// <param name="pair">衝突データ</param>
		void OnCollisionEnter(const CollisionPair& pair) override;

		/// <summary>
		/// ヒット判定
		/// </summary>
		/// <param name="pair">衝突データ</param>
		void OnCollisionExcute(const CollisionPair& pair) override;

	public:

		/// <summary>
		/// 潰しタイプを計算する
		/// </summary>
		/// <param name="pair">衝突データ</param>
		/// <returns>潰しタイプ</returns>
		PressType CalculatePressType(const CollisionPair& pair) const;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 垂直に当たるオブジェクトの候補一覧を取得する
		/// </summary>
		/// <returns>垂直に当たるオブジェクトの候補一覧</returns>
		vector<maru::HitData> FindHitCandidateHitDatas();

		/// <summary>
		/// 潰されている状態の設定
		/// </summary>
		/// <param name="isPressed">潰されている状態かどうか</param>
		void SetIsPressed(const bool isPressed) noexcept;

		/// <summary>
		/// 潰されている状態かどうかを取得
		/// </summary>
		/// <returns>潰されている状態かどうか</returns>
		bool IsPressed() const;

		/// <summary>
		/// 潰されている状態が切り替わったときに呼び出すイベントの追加
		/// </summary>
		/// <param name="b">どっちの状態に切り替わったときに呼び出すか</param>
		/// <param name="function">切り替わったときに呼び出すイベント</param>
		void AddIsPressedReactiveFunction(const bool b, const std::function<void()>& function);

		/// <summary>
		/// 潰す側の設定
		/// </summary>
		/// <param name="pusher">潰す側</param>
		void SetPusher(const std::shared_ptr<Pusher>& pusher) noexcept {
			m_pusher = pusher;
		}

		/// <summary>
		/// 潰す側の取得
		/// </summary>
		/// <returns>潰す側</returns>
		std::shared_ptr<Pusher> GetPusher() const {
			return m_pusher.GetShard();
		}

		/// <summary>
		/// 潰し成功データの設定
		/// </summary>
		/// <param name="data">潰し成功データ</param>
		void SetSuccessPressData(const PressData& data) noexcept;

		/// <summary>
		/// 潰し成功データの取得
		/// </summary>
		/// <returns>潰し成功データ</returns>
		PressData GetSuccessPressData() const noexcept;
	};

}

//endbasecross