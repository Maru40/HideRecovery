/*!
@file ShootSlime_BulletObject.cpp
@brief ShootSlime_BulletObject�̃N���X����
�S���F�ێR�T��
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
				//�R���W�����ݒ�
				auto collision = AddComponent<CollisionSphere>();
				collision->SetAfterCollision(AfterCollision::None);

				AddComponent<ShootSlime::Bullet>();
				//�Ǐ]�p�����[�^��ݒ�
				auto seekParametor = SeekTarget::Parametor(100.0f, 0.0f, Vec3(0.0f, 2.5f, 0.0f), SeekTarget::SeekType::Lerp);
				AddComponent<SeekTarget>(nullptr, seekParametor);

				AddComponent<VelocityManager>();

				SettingDraw();
			}

			void BulletObject::SettingDraw() {
				//�Ǐ]�p�̃p�����[�^�ݒ�
				std::vector<Vec3> offsets = {
					Vec3(+1.0f , -2.5f, +1.0f),
					Vec3(+1.0f , -2.0f, -1.0f),
					Vec3(-1.0f , -1.7f, -1.0f),
					Vec3(-1.0f , -2.3f, +1.0f),
				};

				//�ʒu����
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

				//�z��̐���������
				int NumCreate = static_cast<int>(std::size(offsets));
				auto render = AddComponent<Metaball::RenderBase>(NumCreate);
				const float Radius = 0.001f;
				render->SetChildrenRadius(Radius);
				render->SetDiffuse(Col4(0.1f, 0.1f, 0.8f, 1.0f));
				//�Ǐ]�R���|�[�l���g
				AddComponent<Metaball::ChildrenSeekManager>(parametor);
			}

		}
	}
}