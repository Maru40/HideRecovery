
/*!
@file MapCursor.h
@brief MapCursorÇ»Ç«
íSìñÅFä€éRóTäÏ
*/

#pragma once
#include "stdafx.h"

#include "BuilderVertexPCT.h"
#include "SingletonComponent.h"

#include "ImpactMap.h"

namespace basecross {

	class SpriteObject;

	class MapCursor : public Component
	{
		std::weak_ptr<GameObject> m_target;
		std::weak_ptr<SpriteObject> m_sprite;

	public:
		MapCursor(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

		void OnDrawActive() override;
		void OnDrawFalse() override;

	private:
		void MoveCursor();

		Vec3 MoveClamp(const Vec3& position);

	public:
		void SetTarget(const std::shared_ptr<GameObject>& target) { m_target = target; }

		std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.lock(); }

		Vec3 GetCursorFiledPosition();
	};
	
}