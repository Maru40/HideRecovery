
/*!
@file Teleport.h
@brief Teleportなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// テレポート機能
	//--------------------------------------------------------------------------------------

	struct Teleport_Parametor 
	{
		Vec3 position;	//テレポートする位置
	};

	//--------------------------------------------------------------------------------------
	/// テレポート機能
	//--------------------------------------------------------------------------------------
	class Teleport : public Component
	{
	public:
		using Parametor = Teleport_Parametor;

	private:
		Parametor m_param;

	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:


	public:
		/// <summary>
		/// テレポート開始
		/// </summary>
		void StartTeleport();

		/// <summary>
		/// マップを開く。
		/// </summary>
		void OpenMap();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// テレポート位置の設定
		/// </summary>
		/// <param name="position">テレポート位置の設定</param>
		void SetTeleportPosition(const Vec3& position) noexcept { m_param.position = position; }

		/// <summary>
		/// テレポート位置の取得
		/// </summary>
		/// <returns>テレポート位置</returns>
		Vec3 GetTeleportPosition() const noexcept { return m_param.position; }
	};

}