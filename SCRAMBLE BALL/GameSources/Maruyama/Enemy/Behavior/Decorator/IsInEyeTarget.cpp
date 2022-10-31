/*!
@file I_Decorator.cpp
@brief I_Decorator�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				IsInEyeTarget::IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					IsInEyeTarget(owner, std::vector<std::weak_ptr<GameObject>>())
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveSharedTargets& observeTargets
				):
					IsInEyeTarget(owner, maru::Utility::ConvertArraySharedToWeak(observeTargets))
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveTargets& observeTargets
				):
					DecoratorBase(owner),
					m_observeTargets(observeTargets)
				{
					m_eyeRange = owner->GetGameObject()->GetComponent<EyeSearchRange>(false);
				}

				bool IsInEyeTarget::CanTransition() const {
					auto eyeRange = m_eyeRange.lock();
					if (!eyeRange) {
						Debug::GetInstance()->Log(L"IsInEyeTarget::CanTransition() : �K�v�R���|�[�l���g������܂���B");
						return false;	//���E���Ȃ����画�f�ł��Ȃ����߁A�J�ڂł��Ȃ��B
					}

					for (auto& weakTarget : m_observeTargets) {
						if (weakTarget.expired()) {
							continue;	//�Ώۂ��Ȃ��Ȃ�ΏۊO
						}
						
						//���E�͈͓��Ȃ�A�J�ڂł���B
						auto target = weakTarget.lock();
						auto targetPosition = target->GetComponent<Transform>()->GetPosition();
						if (eyeRange->IsInEyeRange(targetPosition)) {
							//�{���͕]���l���m�[�h�{�̂ɓn���B
							return true;
						}
					}

					return false;
				}

				bool IsInEyeTarget::CanUpdate() {
					//�^�[�Q�b�g���������āA��莞�Ԃ�������AUpdate���I��

					return true;
				}

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				void IsInEyeTarget::SetObserveTargets(const ObserveSharedTargets& targets) {
					SetObserveTargets(maru::Utility::ConvertArraySharedToWeak(targets));
				}

			}
		}
	}
}