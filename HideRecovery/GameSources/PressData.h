/*!
@file PressData.h
@brief PressDataなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SlowTimerManager.h"

#include "ReactiveProperty.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class Pusher;
	struct SlowTimeParametor;
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// 潰しタイプ
	//--------------------------------------------------------------------------------------
	enum class PressType
	{
		Perfect,  //パーフェクト判定
		Good,     //グッド判定
		Miss,     //ミス判定
	};

	//--------------------------------------------------------------------------------------
	/// 潰しデータ
	//--------------------------------------------------------------------------------------
	struct PressData
	{
		/// <summary>
		/// 潰し失敗データ
		/// </summary>
		struct MissData
		{
			Vec3 pusherVec = Vec3(0.0f);      //潰す方向
			Vec3 refrectionVec = Vec3(0.0f);  //反射方向
		};

		ReactiveProperty<GameObject> pusher;  //潰してきた相手
		ex_weak_ptr<GameObject> pressed;      //潰される自分自身
		ex_weak_ptr<GameObject> obstacle;     //潰す壁などの障害物
		SlowTimeParametor slowParam;          //スロータイマーパラメータ
		MissData missData;                    //MissData
		PressType pressType;                  //プレスタイプ
		CollisionPair collisionPair;          //コリジョンデータ
		Vec3 pressedVec;                      //潰す方向
		Vec3 lostPusherPosition;              //潰す側のオブジェクトがロストした場所

		/// <summary>
		///	コンストラクタ
		/// </summary>
		PressData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="pusher">潰してきた相手</param>
		/// <param name="pressed">潰される自分自身</param>
		/// <param name="obstacle">障害物</param>
		PressData(const std::shared_ptr<GameObject>& pusher, 
			const std::shared_ptr<GameObject>& pressed, 
			const std::shared_ptr<GameObject>& obstacle);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="pusher">潰してきた相手</param>
		/// <param name="pressed">潰される自分自身</param>
		/// <param name="obstacle">障害物</param>
		/// <param name="slowParam">スロータイムパラメータ</param>
		/// <param name="missData">ミスデータ</param>
		/// <param name="pressType">潰しタイプ</param>
		/// <param name="collisionPair">衝突データ</param>
		PressData(const std::shared_ptr<GameObject>& pusher,
			const std::shared_ptr<GameObject>& pressed,
			const std::shared_ptr<GameObject>& obstacle,
			const SlowTimeParametor& slowParam,
			const MissData& missData,
			const PressType pressType,
			const CollisionPair& collisionPair);

		/// <summary>
		/// 潰してきた相手を取得
		/// </summary>
		/// <returns>潰してきた相手</returns>
		std::shared_ptr<Pusher> GetPusher() const;

		/// <summary>
		/// 潰す力の取得
		/// </summary>
		/// <returns>潰す力</returns>
		float CalculatePressPower() const;

		/// <summary>
		/// 潰される側から潰す側へのベクトル
		/// </summary>
		/// <returns>潰される側から潰す側へのベクトル</returns>
		Vec3 GetPressedToPuserVec() const;

		/// <summary>
		/// 潰す側から潰される側へのベクトル
		/// </summary>
		/// <returns>潰す側から潰される側へのベクトル</returns>
		Vec3 GetPusherToPressedVec() const;

		/// <summary>
		/// ヒットポイントから潰される側へのベクトル
		/// </summary>
		/// <returns>ヒットポイントから潰される側へのベクトル</returns>
		Vec3 GetHitPointToPressedVec() const;

		/// <summary>
		/// Pusherのポジションを取得
		/// </summary>
		/// <returns>Pusherのポジション</returns>
		Vec3 GetPusherPosition() const;

		/// <summary>
		/// PusherObjectのコンポーネントを取得する。
		/// </summary>
		/// <returns>PusherObjectのコンポ―ネント</returns>
		template<class T>
		std::shared_ptr<T> GetPusherObjectComponent() const {
			return pusher ? pusher->GetComponent<T>(false) : nullptr;
		}
	};

}

//endbasecross