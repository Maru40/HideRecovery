
/*!
@file Utility.cpp
@brief MyUtility�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruHitHelper.h"

namespace basecross {
	namespace maru {

		Vec3 HitData::GetSelfPosition() {
			return selfObject->GetComponent<Transform>()->GetPosition();
		}

		Vec3 HitData::GetSelfToHitVec() {
			return point - GetSelfPosition();
		}

		Vec3 HitData::GetHitToSelfVec() {
			return GetSelfPosition() - point;
		}



		HitData HitHelper::ClosestPtPointOBB(const Vec3& selfPosition, const std::shared_ptr<CollisionObb>& other) {
			HitData data;
			HitTest::ClosestPtPointOBB(selfPosition, other->GetObb(), data.point);
			auto toSelf = selfPosition - data.point;

			//�m�[�}���̎擾
			auto transform = other->GetGameObject()->GetComponent<Transform>();
			auto forward = transform->GetForward();
			auto right = transform->GetRight();

			const vector<Vec3> directs = {
				forward, -forward, right, -right
			};

			//��ԋ߂����������B
			float minDot = 99999999.0f;
			for (auto& direct : directs) {
				float newDot = dot(direct, toSelf.normalize());
				if (newDot < minDot) {
					data.normal = -direct;
					minDot = newDot;
				}
			}

			return data;
		}

		HitData HitHelper::ClosestPtPointOBB(const std::shared_ptr<GameObject>& self, const std::shared_ptr<CollisionObb>& other) {
			HitData data;

			auto PtrTransform = self->GetComponent<Transform>();
			auto PtrDestTransform = other->GetGameObject()->GetComponent<Transform>();
			bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
			bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
			//�O��̃^�[������̎���
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			//�ړ��ȊO�ω��Ȃ�
			auto selfObb = self->GetComponent<CollisionObb>();
			OBB SrcObb = selfObb->GetObb();
			OBB SrcBeforeObb = selfObb->GetBeforeObb();
			//����
			OBB DestObb = other->GetObb();
			OBB DestBeforeObb = other->GetBeforeObb();
			//�ȈՓI�Ȕ���
			if (!HitTest::OBB_OBB(SrcObb, DestObb)) {
				return data;
			}
			bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
			float HitTime = 0;
			if (HitTest::CollisionTestObbObb(SrcBeforeObb, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
				CollisionPair pair;
				pair.m_Src = self->GetComponent<Collision>();
				pair.m_Dest = other;
				OBB SrcChkObb = SrcBeforeObb;
				SrcChkObb.m_Center += SrcVelocity * HitTime;
				pair.m_SrcCalcHitCenter = SrcChkObb.m_Center;
				OBB DestChkObb = DestBeforeObb;
				DestChkObb.m_Center += DestVelocity * HitTime;
				pair.m_DestCalcHitCenter = DestChkObb.m_Center;
				bsm::Vec3 RetVec;
				//Src��OBB��Dest�̍ŋߐړ_�𓾂�
				HitTest::ClosestPtPointOBB(SrcChkObb.m_Center, DestChkObb, RetVec);
				//�ړ_�ւ̃x�N�g��
				//�Փ˂����u�ԂŖ@�����v�Z
				pair.m_SrcHitNormal = SrcChkObb.m_Center - RetVec;
				pair.m_CalcHitPoint = RetVec;
				pair.m_SrcHitNormal.normalize();
				//GetCollisionManager()->InsertNewPair(pair);

				data.normal = pair.m_SrcHitNormal;
				data.point = pair.m_CalcHitPoint;
			}

			return data;
		}
	}
}
