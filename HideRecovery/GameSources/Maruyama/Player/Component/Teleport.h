
/*!
@file Teleport.h
@brief Teleportなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class FieldMap;

	//--------------------------------------------------------------------------------------
	/// テレポート機能のパラメータ
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
		Parametor m_param;		//パラメータ

		std::weak_ptr<FieldMap> m_fieldMap;	//マップ

	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void SettingFieldMap();
		void SettingAnimationEvent();

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

		std::shared_ptr<FieldMap> GetFieldMap() const;
	};

}