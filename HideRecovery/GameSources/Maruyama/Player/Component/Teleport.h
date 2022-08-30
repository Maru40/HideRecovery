
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
	class GameTimer;
	class VirtualCamera;
	class ToTargetMove;
	class TeleportUI;
	class I_TeamMember;

	//--------------------------------------------------------------------------------------
	/// テレポート機能のパラメータ
	//--------------------------------------------------------------------------------------

	struct Teleport_Parametor 
	{
		float maxRangeLate;	//テレポートできる最大距離
		Vec3 position;		//テレポートする位置
		bool isTeleporting;	//テレポート中かどうか

		Teleport_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// テレポート機能
	//--------------------------------------------------------------------------------------
	class Teleport : public Component
	{
	public:
		using Parametor = Teleport_Parametor;

	private:
		Parametor m_param;						//パラメータ

		std::weak_ptr<FieldMap> m_fieldMap;		//マップ
		std::unique_ptr<GameTimer> m_timer;		//タイマー

		std::weak_ptr<VirtualCamera> m_camera;	//カメラ
		std::weak_ptr<ToTargetMove> m_toTargetMove;
		std::weak_ptr<TeleportUI> m_teleportUI;
		std::weak_ptr<I_TeamMember> m_teamMember;

		Vec3 m_cameraPosition;

	public:
		Teleport(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
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

		void StartTeleport(const Vec3& teleportPosition);

		/// <summary>
		/// マップを開く。
		/// </summary>
		void OpenMap();

		/// <summary>
		/// マップを閉じる
		/// </summary>
		void CloseMap();

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

		/// <summary>
		/// フィールドマップの取得
		/// </summary>
		/// <returns>フィールドマップ</returns>
		std::shared_ptr<FieldMap> GetFieldMap() const;

		/// <summary>
		/// テレポートできるかどうか
		/// </summary>
		/// <returns>テレポートできるならtrue</returns>
		bool CanTeleport() const;

		/// <summary>
		/// テレポート中かどうか
		/// </summary>
		/// <returns>テレポート中かどうか</returns>
		bool IsTeleporting() const;

		/// <summary>
		/// テレポートカメラの設定
		/// </summary>
		/// <param name="camera">カメラの設定</param>
		void SetTeleportCamera(const std::shared_ptr<VirtualCamera> camera);

		/// <summary>
		/// テレポートカメラの取得
		/// </summary>
		/// <returns>テレポートカメラ</returns>
		std::shared_ptr<VirtualCamera> GetTeleportCamera() const;

		void SetToTargetMove(const std::shared_ptr<ToTargetMove>& toTargetMove) { m_toTargetMove = toTargetMove; }

		std::shared_ptr<ToTargetMove> GetToTargetMove() const { return m_toTargetMove.lock(); }

		void SetCameraPosition(const Vec3& cameraPosition) { m_cameraPosition = cameraPosition; }

		std::shared_ptr<TeleportUI> GetTeleportUI() const;
	};

}