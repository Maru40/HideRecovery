
/*!
@file MapCursor.h
@brief MapCursorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/UI/BuilderVertexPCT.h"
#include "SingletonComponent.h"

#include "ImpactMap.h"

namespace basecross {

	class SpriteObject;
	class Teleport;
	class I_TeamMember;
	class TeleportCursorUI;

	class MapCursor : public Component
	{
		//float m_moveRangeLate;	//移動制限範囲

		std::weak_ptr<GameObject> m_target;
		std::weak_ptr<SpriteObject> m_cursor;		//
		std::weak_ptr<SpriteObject> m_underCircle;	//下の円
		std::weak_ptr<GameObject> m_teleportUIObject;

		std::weak_ptr<Teleport> m_teleport;
		std::weak_ptr<I_TeamMember> m_teamMember;

		Vec3 m_beforePosition;

	public:
		MapCursor(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

		void OnDrawActive() override;
		void OnDrawFalse() override;

	private:
		void SettingCursor();
		void SettingUnderCircle();
		void SettingTeleportUI();

		void MoveCursor();

		Vec3 MoveClamp(const Vec3& position);

		/// <summary>
		/// マップカーソルをターゲットの位置に設定する。
		/// </summary>
		void SetMapCursorPositionConnectTargetPosition();

	public:
		void SetTarget(const std::shared_ptr<GameObject>& target);

		std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.lock(); }

		Vec3 GetCursorFiledPosition();
	};
	
}