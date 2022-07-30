/*!
@file WallEvasion.h
@brief WallEvasion
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"
#include "TactileCtrl.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	障害物回避用のデータ
	//--------------------------------------------------------------------------------------
	struct ObstacleRangePair
	{
		ex_weak_ptr<GameObject> object;	//オブジェクトデータ
		float range;					//回避距離

		/// <summary>
		/// コンストラクタ
		/// </summary>
		ObstacleRangePair()
			:ObstacleRangePair(nullptr, 0.0f)
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">オブジェクトデータ</param>
		/// <param name="range">回避距離</param>
		ObstacleRangePair(const std::shared_ptr<GameObject>& objPtr, const float& range)
			:object(objPtr), range(range)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	障害物回避
	//--------------------------------------------------------------------------------------
	class WallEvasion : public Component
	{
		std::vector<ex_weak_ptr<GameObject>> m_obstacleObjs;	//障害物の対象配列
		float m_maxSpeed = 30.0f;								//最大回避速度

		std::vector<ex_weak_ptr<GameObject>> m_tactileObjects;	//触覚をオブジェクト配列

		/// <summary>
		/// 壁との距離を測定して、小さい順に並べて配列を返す
		/// </summary>
		/// <returns>壁との距離を測定した配列</returns>
		vector<ObstacleRangePair> CalucWallRangeSort();

		/// <summary>
		/// 加える力を計算する。
		/// </summary>
		/// <param name="tactile">触覚オブジェクト</param>
		/// <returns>計算した力</returns>
		Vec3 CalucForce(const std::shared_ptr<GameObject>& tactile);

		/// <summary>
		/// 壁回避処理
		/// </summary>
		/// <param name="tactile">触覚オブジェクト</param>
		/// <param name="other">接触したオブジェクト</param>
		void EvasionUpdate(const std::shared_ptr<TactileCtrl>& tactile, const std::shared_ptr<GameObject>& other);

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		WallEvasion(const std::shared_ptr<GameObject>& objPtr)
			:Component(objPtr)
		{}
		
		void OnDestroy() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 障害物判定のオブジェクトを増やす。
		/// </summary>
		/// <param name="object">障害物のオブジェクト</param>
		void AddObstacleObjs(const std::shared_ptr<GameObject>& object) {
			m_obstacleObjs.push_back(object);
		}

		/// <summary>
		/// 触覚のセット
		/// </summary>
		/// <param name="tactile">触覚のオブジェクト</param>
		void SetTactile(const std::shared_ptr<TactileCtrl>& tactile);

		/// <summary>
		/// 最大速度の設定
		/// </summary>
		/// <param name="speed">最大速度</param>
		void SetMaxSpeed(const float& speed) noexcept { m_maxSpeed = speed; }

		/// <summary>
		/// 最大速度の取得
		/// </summary>
		/// <returns>最大速度</returns>
		float GetMaxSpeed() const noexcept { return m_maxSpeed; }

	};

}

//endbasecross