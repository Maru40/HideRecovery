﻿#pragma once
#include "stdafx.h"

namespace basecross
{
	class Teleport;
	class VelocityManager;
	class ItemAcquisitionManager;
	class Item;
	class UseWeapon;
	class GoalAnimationController;
	class HidePlaceOpener;
	class HidePlace;
	class PlayerAnimator;
	class RotationController;

	namespace Operator
	{
		class ObjectMover;
	}

	/// <summary>
	/// プレイヤーに対しての操作をまとめたクラス
	/// </summary>
	class PlayerControlManager : public Component
	{
		/// <summary>
		/// 前の基準にするカメラ
		/// </summary>
		std::weak_ptr<Camera> m_forwardCamera;

		/// <summary>
		/// テレポート機能コンポーネント
		/// </summary>
		std::weak_ptr<Teleport> m_teleport;
		/// <summary>
		///	アイテムの取得管理コンポーネント
		/// </summary>
		std::weak_ptr<ItemAcquisitionManager> m_acquisitionManager;

		std::weak_ptr<VelocityManager> m_velocityManager;
		/// <summary>
		/// オブジェクトを動かす機能を持ったコンポーネント
		/// </summary>
		std::weak_ptr<Operator::ObjectMover> m_objectMover;
		/// <summary>
		/// 武器を使う機能を持ったコンポーネント
		/// </summary>
		std::weak_ptr<UseWeapon> m_useWeapon;
		/// <summary>
		/// ゴールアニメーション用コンポーネント
		/// </summary>
		std::weak_ptr<GoalAnimationController> m_goalAnimationController;
		/// <summary>
		/// 隠し場所に対して操作するコンポーネント
		/// </summary>
		std::weak_ptr<HidePlaceOpener> m_hidePlaceOpener;
		/// <summary>
		/// プレイヤーのアニメーション管理コンポーネント
		/// </summary>
		std::weak_ptr<PlayerAnimator> m_playerAnimator;
		/// <summary>
		/// オブジェクトの回転制御用コンポーネント
		/// </summary>
		std::weak_ptr<RotationController> m_rotationController;

		/// <summary>
		/// 前回のカメラ前方ベクトル
		/// </summary>
		Vec3 m_beforeCameraForward = Vec3();

	public:
		PlayerControlManager(const std::shared_ptr<GameObject>& owner) : Component(owner) {}

		void OnLateStart() override;

		void OnUpdate() override {}

		void OnDraw() override {}

		/// <summary>
		/// カメラのforwardの更新を試す
		/// </summary>
		/// <param name="forward">変更があればそのベクトルが入る</param>
		/// <returns>変更があればtrue</returns>
		bool IsUpdateCameraForward(Vec3* forward);

		void ExecuteUpdateCameraForward(const Vec3& forward);

		/// <summary>
		/// 移動ができるかを試す
		/// </summary>
		/// <param name="inputVector">入力ベクトル</param>
		/// <param name="outputMoveVector">nullptrでないなら移動量ベクトルをいれる</param>
		/// <param name="outputForward">nullptrでないなら前方ベクトルを入れる</param>
		/// <returns>成功したらtrue</returns>
		bool TryMove(const Vec2& inputVector, Vec3* outputMoveVector, Vec3* outputForward);
		/// <summary>
		/// 誰かが移動したら呼ばれるイベント
		/// </summary>
		/// <param name="moveVector">移動量ベクトル</param>
		/// <param name="forward">前方ベクトル</param>
		void ExecuteMove(const Vec3& moveVector, const Vec3& forward);

		/// <summary>
		/// アイテムの取得を試す
		/// </summary>
		/// <param name="item">取得するアイテム</param>
		/// <returns>成功したらtrue</returns>
		bool TryAquisition(const std::shared_ptr<Item>& item);

		/// <summary>
		/// マップの開閉を試す
		/// </summary>
		/// <returns>成功したらtrue</returns>
		bool TryOpenMap();

		bool TryTeleport(Vec3* teleportPosition, Vec3* cameraPosition);
		void ExecuteTeleport(const Vec3& temeportPosition, const Vec3& cameraPosition);

		/// <summary>
		/// エイム状態の変更を試す
		/// </summary>
		/// <param name="isAim">エイム状態ならtrue</param>
		/// <returns>変更できればtrue</returns>
		bool TryUpdateAim(bool isAim);
		/// <summary>
		/// エイムの更新をする
		/// </summary>
		/// <param name="isAim">エイム状態ならtrue</param>
		void ExecuteUpdateAim(bool isAim);

		std::shared_ptr<HidePlace> GetCanOpenHidePlace() const;

		/// <summary>
		/// HidePlaceが開けるか試す
		/// </summary>
		/// <param name="hidePlace">開くHidePlace</param>
		/// <returns>開けたならtrue</returns>
		bool TryAccessHidePlace(const std::shared_ptr<HidePlace>& hidePlace);

		/// <summary>
		/// 前方カメラコンポーネントの設定
		/// </summary>
		/// <param name="camera">前方カメラコンポーネント</param>
		void SetCamera(const std::shared_ptr<Camera>& camera) { m_forwardCamera = camera; }
		/// <summary>
		/// 前方カメラコンポーネントの取得
		/// </summary>
		/// <returns>前方カメラコンポーネント</returns>
		std::shared_ptr<Camera> GetCamera() const { return m_forwardCamera.lock(); }

	};
}