
/*!
@file Utility.cpp
@brief MyUtilityクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruHitHelper.h"

#include "DebugObject.h"

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

			//ノーマルの取得
			auto transform = other->GetGameObject()->GetComponent<Transform>();
			auto forward = transform->GetForward();
			auto right = transform->GetRight();

			const vector<Vec3> directs = {
				forward, -forward, right, -right
			};

			//一番近い向きを取る。
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
			//前回のターンからの時間
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			//移動以外変化なし
			auto selfObb = self->GetComponent<CollisionObb>();
			OBB SrcObb = selfObb->GetObb();
			OBB SrcBeforeObb = selfObb->GetBeforeObb();
			//相手
			OBB DestObb = other->GetObb();
			OBB DestBeforeObb = other->GetBeforeObb();
			//簡易的な判定
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
				//SrcのOBBとDestの最近接点を得る
				HitTest::ClosestPtPointOBB(SrcChkObb.m_Center, DestChkObb, RetVec);
				//接点へのベクトル
				//衝突した瞬間で法線を計算
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
