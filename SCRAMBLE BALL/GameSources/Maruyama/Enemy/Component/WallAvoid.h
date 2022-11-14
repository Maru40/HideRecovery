
/*!
@file WallAvoid.h
@brief WallAvoidなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------------
	///	壁回避用の触覚
	//--------------------------------------------------------------------------------------
	class WallAvoidTacticle {
		float m_range;	//長さ
		float m_degree;	//角度

	public:

		WallAvoidTacticle();

		WallAvoidTacticle(const float degree);

		WallAvoidTacticle(const float range, const float degree);

		float GetRange() const noexcept { return m_range; }

		float GetDegree() const noexcept { return m_degree; }

		float GetRad() const { return XMConvertToRadians(m_degree); }
	};

	//--------------------------------------------------------------------------------------
	///	壁回避パラメータ
	//--------------------------------------------------------------------------------------
	struct WallAvoid_Parametor {
		float avoidPower;
		Vec3 offset;

		WallAvoid_Parametor();
	};

	//--------------------------------------------------------------------------------------
	///	壁回避
	//--------------------------------------------------------------------------------------
	class WallAvoid : public Component 
	{
	public:
		using Parametor = WallAvoid_Parametor;

	private:
		Parametor m_param;

		Vec3 m_avoidVec;	//壁回避のベクトル

		std::weak_ptr<Transform> m_transform;

		std::vector<std::shared_ptr<WallAvoidTacticle>> m_tacticles;	//触覚配列
		std::vector<std::weak_ptr<GameObject>> m_obstacles;				//障害物群

	public:
		WallAvoid(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnStart() override;
		void OnLateStart() override;

		void OnUpdate() override;

	private:
		/// <summary>
		/// 障害物よけ
		/// </summary>
		void AvoidUpdate();

		/// <summary>
		/// 障害物よけベクトルを計算して取得
		/// </summary>
		/// <returns></returns>
		Vec3 CalculateAvoidVec(const std::shared_ptr<WallAvoidTacticle>& tacticle);

		/// <summary>
		/// レイの方向を計算して取得する
		/// </summary>
		/// <param name="tacticle">レイ判定用の触覚</param>
		/// <returns></returns>
		Vec3 CalculateRayDirection(const std::shared_ptr<WallAvoidTacticle>& tacticle);

		/// <summary>
		/// デフォルトの障害物配列を生成
		/// </summary>
		void SettingDefaultObstacles();

		/// <summary>
		/// デフォルト触覚を生成
		/// </summary>
		void SettingDefaultTacticles();

	public:

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		Vec3 TakeAvoidVec() noexcept {
			auto result = m_avoidVec;
			m_avoidVec = Vec3(0.0f);
			return result;
		}
	};

}