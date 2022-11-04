/*!
@file ObserveIsInEyeTarget.h
@brief ObserveIsInEyeTargetなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;

	//--------------------------------------------------------------------------------------
	/// 監視対象が視界範囲にいるかどうかを判断するクラス
	//--------------------------------------------------------------------------------------
	class ObserveIsInEyeTarget
	{
	public:
		using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
		using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

	private:
		ObserveTargets m_observeTargets;			//監視対象配列

		std::weak_ptr<EyeSearchRange> m_eyeRange;	//視界管理コンポーネント

	public:
		ObserveIsInEyeTarget(const std::shared_ptr<EyeSearchRange>& eyeRange);

		ObserveIsInEyeTarget(
			const std::shared_ptr<EyeSearchRange>& eyeRange,
			const ObserveSharedTargets& observeTargets
		);

		ObserveIsInEyeTarget(
			const std::shared_ptr<EyeSearchRange>& eyeRange,
			const ObserveTargets& observeTargets
		);

		virtual ~ObserveIsInEyeTarget() = default;

		/// <summary>
		/// 視界内のターゲットを取得する。
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<GameObject> SearchIsInEyeTarget() const;

		/// <summary>
		/// 視界内のターゲットを全て取得する。
		/// </summary>
		/// <returns>視界内のターゲット全て</returns>
		std::vector<std::shared_ptr<GameObject>> SearchIsInEyeTargets() const;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 監視対象の追加
		/// </summary>
		/// <param name="target">監視対象</param>
		void AddObserveTarget(const std::shared_ptr<GameObject>& target) {
			m_observeTargets.push_back(target);
		}

		void SetObserveTargets(const ObserveTargets& targets) { m_observeTargets = targets; }

		void SetObserveTargets(const ObserveSharedTargets& targets);

		_NODISCARD ObserveTargets GetObserveTargets() const noexcept { return m_observeTargets; }

		void ClearObserveTargets() { m_observeTargets.clear(); }

	};

}