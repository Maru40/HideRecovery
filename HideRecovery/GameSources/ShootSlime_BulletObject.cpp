/*!
@file ShootSlime_BulletObject.cpp
@brief ShootSlime_BulletObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ShootSlime_BulletObject.h"

#include "I_Damaged.h"
#include "BulletBase.h"
#include "ShootSlime_Bullet.h"

#include "MetaballRenderBase.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "SeekTarget.h"
#include "MetaballChildrenSeekManager.h"

#include "VelocityManager.h"

#include "MyRandom.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			BulletObject::BulletObject(const std::shared_ptr<Stage>& stage)
				:GameObject(stage)
			{}

		 	void BulletObject::OnCreate() {
				//コリジョン設定
				auto collision = AddComponent<CollisionSphere>();
				collision->SetAfterCollision(AfterCollision::None);

				AddComponent<ShootSlime::Bullet>();
				//追従パラメータを設定
				auto seekParametor = SeekTarget::Parametor(100.0f, 0.0f, Vec3(0.0f, 2.5f, 0.0f), SeekTarget::SeekType::Lerp);
				AddComponent<SeekTarget>(nullptr, seekParametor);

				AddComponent<VelocityManager>();

				SettingDraw();
			}

			void BulletObject::SettingDraw() {
				//追従用のパラメータ設定
				std::vector<Vec3> offsets = {
					Vec3(+1.0f , -2.5f, +1.0f),
					Vec3(+1.0f , -2.0f, -1.0f),
					Vec3(-1.0f , -1.7f, -1.0f),
					Vec3(-1.0f , -2.3f, +1.0f),
				};

				//位置調整
				for (auto& offset : offsets) {
					float adjust = 1.5f;
					offset.x *= adjust;
					offset.z *= adjust;
				}

				float speed = 100.0f;
				float range = 0.0f;
				auto seekType = SeekTarget::SeekType::Lerp;
				auto seekParams = Metaball::Utility::ChildrenSeekSetting::CreateSeekTargetParametors(speed, range, offsets, seekType);
				auto parametor = Metaball::ChildrenSeekManager::Parametor(seekParams);

				//配列の数だけ生成
				int NumCreate = static_cast<int>(std::size(offsets));
				auto render = AddComponent<Metaball::RenderBase>(NumCreate);
				const float Radius = 0.001f;
				render->SetChildrenRadius(Radius);
				render->SetDiffuse(Col4(0.1f, 0.1f, 0.8f, 1.0f));
				//追従コンポーネント
				AddComponent<Metaball::ChildrenSeekManager>(parametor);
			}

		}
	}
}