#pragma once

namespace basecross
{
	class SeekTarget;
	class SelfAstarNodeController;
	class TargetManager;
	class AIVirtualController;

	namespace Enemy
	{
		class EnemyBase;
		class AIPlayerStator;
	}

	class AIActiveChanger : public Component
	{
		std::weak_ptr<SeekTarget> m_seekTarget;
		std::weak_ptr<SelfAstarNodeController> m_selfAStarNodeController;
		std::weak_ptr<TargetManager> m_targetManager;
		std::weak_ptr<AIVirtualController> m_aiVertualController;
		std::weak_ptr<Enemy::EnemyBase> m_enemyBase;
		std::weak_ptr<Enemy::AIPlayerStator> m_aiPlayerStator;

	public:
		AIActiveChanger(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void AIActiveChange(bool isActive);
	};
}