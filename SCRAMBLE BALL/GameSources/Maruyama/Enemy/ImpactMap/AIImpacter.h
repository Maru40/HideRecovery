/*!
@file AIImpacter.h
@brief AIImpacter
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Interface/I_Impact.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace maru {
		class ImpactMap;
	}

	//--------------------------------------------------------------------------------------
	///	AIの影響者パラメータ
	//--------------------------------------------------------------------------------------
	struct AIImpacter_Parametor {
		float occupancyUpdateIntervalTime;	//占有値更新インターバル時間(マルチスレッドにするまでの応急処置)

		AIImpacter_Parametor();
	};

	//--------------------------------------------------------------------------------------
	///	AIの影響者コンポ―ネント
	//--------------------------------------------------------------------------------------
	class AIImpacter : public Component, public maru::I_Impacter
	{
	public:
		using Parametor = AIImpacter_Parametor;

	private:
		Parametor m_param;								//パラメータ

		std::unique_ptr<GameTimer> m_occupancyTimer;	//占有値更新タイマー

	public:
		AIImpacter(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		void UpdateEyeRangeImpactMap();

	public:
		//--------------------------------------------------------------------------------------
		///	インターフェースの実装
		//--------------------------------------------------------------------------------------

		std::shared_ptr<GameObject> GetImpacterObject() const noexcept override;

		std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const override;

		std::shared_ptr<maru::ImpactMap> GetImpactMap() const override;
	};


}