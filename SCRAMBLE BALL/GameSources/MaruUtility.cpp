
/*!
@file Utility.cpp
@brief Utilityクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"
#include "MaruUtility.h"
#include "Mathf.h"

#include "ArrowObject.h"

namespace basecross {

	namespace maru {

		//ステージの取得
		std::shared_ptr<Stage> Utility::GetStage() {
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			return scene->GetActiveStage();
		}

		//正面にいるかどうか
		bool Utility::IsTargetInFront(const std::shared_ptr<GameObject>& target, const std::shared_ptr<GameObject>& self)
		{
			auto targetTrans = target->GetComponent<Transform>();
			auto targetPosition = targetTrans->GetPosition();

			auto selfTrans = self->GetComponent<Transform>();
			auto selfPosition = selfTrans->GetPosition();
			auto selfForward = selfTrans->GetForward();

			return IsTargetInFront(targetPosition, selfPosition, selfForward);
		}

		bool Utility::IsTargetInFront(const Vec3& targetPosition, const Vec3& selfPosition, const Vec3& selfForward) {
			auto toTarget = targetPosition - selfPosition;
			auto newDot = dot(selfForward, toTarget); 

			//0より大きければ正面にいる。
			return (newDot >= 0.0f);
		}

		bool Utility::IsInTargetCircleRange(const std::shared_ptr<GameObject>& selfTarget,
			const std::shared_ptr<GameObject>& target,
			const float& range)
		{
			auto selfPosition = selfTarget->GetComponent<Transform>()->GetPosition();
			auto targetPosition = target->GetComponent<Transform>()->GetPosition();
			auto toTargetVec = targetPosition - selfPosition;

			return toTargetVec.length() < range;
		}

		//一つ目の引数の方向に向く
		void Utility::SetLookDirect(const Vec3& direct, const std::shared_ptr<GameObject>& objPtr) {
			auto delta = App::GetApp()->GetElapsedTime();

			auto trans = objPtr->GetComponent<Transform>();
			auto forward = trans->GetForward();
			forward.y = 0.0f;
			forward.normalize();

			auto newDot = dot(forward, direct);   //dotで角度を取得 
			auto rad = acosf(newDot);             //acosfで角度の差異が求められる。
			auto newCross = cross(forward, direct);//角度の差異が-なのか+なのか判断するためにcrossを求める。

			float rotDirect = 1.0f;
			if (newCross.y > 0.0f) {  //クロスのYがマイナスなら
				rotDirect *= -1.0f;  //回転する方向を逆にする。
			}
			auto mat = trans->GetWorldMatrix();
			mat = Mathf::Mat4RotationAxisY(mat, rad * rotDirect);

			trans->SetQuaternion(mat.quatInMatrix());

			//return cr.y;
		}

		Vec3 Utility::CalcuToTargetVec(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target) {
			auto selfTrans = self->GetComponent<Transform>();
			auto selfPos = selfTrans->GetWorldPosition();

			auto targetTrans = target->GetComponent<Transform>();
			auto targetPos = targetTrans->GetWorldPosition();

			auto toVec = targetPos - selfPos;

			return toVec;
		}

		Vec3 Utility::CalculateNearDirect(const Vec3& stdDirect, const std::vector<Vec3>& directs) {
			Vec3 resultDirect;

			//一番近い向きを取る。
			float minDot = FLT_MAX;
			for (auto direct : directs) {
				float newDot = dot(direct.GetNormalized(), stdDirect.GetNormalized());
				float newRad = acosf(newDot);
				if (newRad < minDot) {
					resultDirect = direct;
					minDot = newRad;
				}
			}

			return resultDirect;
		}

		Vec3 Utility::CalculateNearDirect(const Vec3& stdDirect,
			const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets) 
		{
			std::vector<Vec3> directs;
			for (const auto& target : targets) {
				directs.push_back(CalcuToTargetVec(selfObject, target));
			}

			return CalculateNearDirect(stdDirect, directs);
		}

		std::vector<std::shared_ptr<GameObject>> Utility::FindNearDirectTargetsDegree(const Vec3& stdDirect,
			const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets,
			const float& degree)
		{
			return FindNearDirectTargetsRadian(stdDirect, selfObject, targets, XMConvertToRadians(degree));
		}

		std::vector<std::shared_ptr<GameObject>> Utility::FindNearDirectTargetsRadian(const Vec3& stdDirect,
			const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets,
			const float& radian)
		{
			std::vector<std::shared_ptr<GameObject>> resultTargets;

			for (const auto& target : targets) {
				if (selfObject == target) { //同じオブジェクトなら処理をしない。
					continue;
				}

				auto toTargetVec = CalcuToTargetVec(selfObject, target);
				if (maru::Utility::IsInAngleRadian(stdDirect, toTargetVec, radian)) {
					resultTargets.push_back(target);
				}
			}

			return resultTargets;
		}

		bool Utility::IsInAngleDegree(const Vec3& first, const Vec3& second, const float& degree) {
			return IsInAngleRadian(first, second, XMConvertToRadians(degree));
		}

		bool Utility::IsInAngleRadian(const Vec3& first, const Vec3& second, const float& radian) {
			float newDot = dot(first.GetNormalized(), second.GetNormalized());
			float angle = acosf(newDot);

			return angle < radian;
		}

		Vec3 Utility::ConvertForwardOffset(const std::shared_ptr<GameObject>& selfObject, const Vec3& offset) {
			auto forward = selfObject->GetComponent<Transform>()->GetForward();
			return ConvertForwardOffset(forward, offset);
		}

		Vec3 Utility::ConvertForwardOffset(const Vec3& forward, const Vec3& offset) {
			auto resultOffset = Vec3(0.0f);
			auto right = maru::Mathf::Mat4RotationAxisY(forward, XM_PIDIV4);
			Vec3 up = cross(right.GetNormalized(), forward.GetNormalized());
			//Vec3 up = maru::Mathf::Mat4RotationAxisX(forward, XM_PIDIV4);
			
			resultOffset += right.GetNormalized() * offset.x;
			resultOffset += up.GetNormalized() * offset.y;
			resultOffset += forward.GetNormalized() * offset.z;

			return resultOffset;
		}

		//回転調整を掛けたQuatを返す。
		Quat Utility::ConvertRotationOffset(const std::shared_ptr<GameObject>& selfObject, const Vec3& offset) {
			auto transform = selfObject->GetComponent<Transform>();
			auto quat = transform->GetQuaternion(); //現在のQuatを取得

			quat *= (Quat)XMQuaternionRotationAxis(transform->GetRight(), offset.x);
			quat *= (Quat)XMQuaternionRotationAxis(transform->GetUp(), offset.y);
			quat *= (Quat)XMQuaternionRotationAxis(transform->GetForward(), offset.z);

			return quat;
		}

		Quat Utility::ConvertForwardToQuat(const Vec3& forward, const Vec3& nowPosition) {
			Vec3 mz = forward.GetNormalized();
			Vec3 mx = cross(Vec3::Up(), mz).GetNormalized();
			Vec3 my = cross(mz, mx).GetNormalized();

			Vec3 mp = nowPosition + forward;

			auto mat = Mat4x4(
				{ mx.x, mx.y, mx.z, 0.0f },
				{ my.x, my.y, my.z, 0.0f },
				{ mz.x, mz.y, mz.z, 0.0f },
				{ mp.x, mp.y, mp.z, 1.0f }
			);

			return mat.quatInMatrix();
		}

		std::shared_ptr<GameObject> Utility::CreateArrow(const Vec3& startPosition, const Vec3& endPosition, const std::shared_ptr<Stage>& stage) {
			auto toEndVec = endPosition - startPosition;
			auto halfToEndVec = toEndVec * 0.5f;
			auto createPosition = startPosition + halfToEndVec;
			auto createQuat = ConvertForwardToQuat(toEndVec.GetNormalized(), startPosition);

			auto object = stage->Instantiate<ArrowObject>(createPosition, createQuat);
			auto transform = object->GetComponent<Transform>();

			//スケールのセット
			auto scale = Vec3(toEndVec.length(), 0.5f, 1.0f);
			transform->SetScale(scale);

			//方向のセット
			//transform->SetForward(toEndVec.GetNormalized());
			transform->Rotate(Vec3(0.0f, -XM_PIDIV2, 0.0f));

			return object;
		}

		bool Utility::IsRayObstacle(const std::shared_ptr<GameObject>& startObj, const std::shared_ptr<GameObject>& endObj) {
			return IsRayObstacle(startObj, endObj, vector<std::shared_ptr<GameObject>>());
		}

		bool Utility::IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition) {
			return IsRayObstacle(startPosition, endPosition, vector<std::shared_ptr<GameObject>>());
		}

		bool Utility::IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
			const std::shared_ptr<GameObject>& endObj,
			const vector<shared_ptr<GameObject>>& excluteObjs)
		{
			return IsRayObstacle(startObj, endObj, GetStage()->GetGameObjectVec(), excluteObjs);
		}

		bool Utility::IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition,
			const vector<shared_ptr<GameObject>>& excluteObjs)
		{
			return IsRayObstacle(startPosition, endPosition, GetStage()->GetGameObjectVec(), excluteObjs);
		}

		bool Utility::IsRayObstacle(const std::shared_ptr<GameObject>& startObj, const std::shared_ptr<GameObject>& endObj,
			const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
		{
			auto startPosition = startObj->GetComponent<Transform>()->GetWorldPosition();
			auto endPosition = endObj->GetComponent<Transform>()->GetWorldPosition();

			//excluteObjsにスタートオブジェクトと、エンドオブジェクトを省く。
			auto newExcluteObjs = excluteObjs;
			newExcluteObjs.push_back(startObj);
			newExcluteObjs.push_back(endObj);

			return IsRayObstacle(startPosition, endPosition, obstacleObjs,newExcluteObjs);
		}

		bool Utility::IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition,
			const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
		{
			auto direction = endPosition - startPosition;

			for (const auto& object : obstacleObjs)
			{
				auto collision = object->GetComponent<CollisionObb>(false);
				if (!collision) {
					continue;
				}
				if (collision->GetUpdateActive() == false) {
					continue;
				}

				if (collision->GetAfterCollision() == AfterCollision::None) {
					continue;
				}

				//対象外のオブジェクトだったら
				if (IsExclute(object, excluteObjs)) {
					continue;
				}

				if (HitTest::SEGMENT_OBB(startPosition, endPosition, collision->GetObb())) {
					return true;
				}

				//IsRayHitに不具合があるまでこちらでcontinue
				continue;

				//ヒットしたら、障害物があることになる。
				RayHitData data;
				if (collision->IsRayHit(startPosition, direction, data)) {
					auto length = direction.length();
					if (direction.length() > data.length) {  //lengthが手前だったら
						return true;
					}
				}
			}

			return false;
		}

		bool Utility::IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
			const std::shared_ptr<GameObject>& endObj,
			const std::shared_ptr<GameObject>& obstacleObj)
		{
			auto startPosition = startObj->GetComponent<Transform>()->GetWorldPosition();
			auto endPosition = endObj->GetComponent<Transform>()->GetPosition();
			//auto direction = endPosition - startPosition;

			return IsRayObstacle(startPosition, endPosition, obstacleObj);
		}

		bool Utility::IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition,
			const std::shared_ptr<GameObject>& obstacleObj)
		{
			auto collision = obstacleObj->GetComponent<Collision>();
			auto direction = endPosition - startPosition;

			//ヒットしたら、障害物があることになる。
			RayHitData data;
			if (collision->IsRayHit(startPosition, direction, data)) {
				if (direction.length() > data.length) {
					return true;
				}
			}

			return false;
		}

		bool Utility::IsExclute(const shared_ptr<GameObject>& targetObj,const vector<shared_ptr<GameObject>>& excluteObjs) {
			for (auto& exclute : excluteObjs) {
				if (targetObj == exclute) {
					return true;
				}

				if (targetObj->GetParent() == exclute) {
					return true;
				}

				if (targetObj->GetComponent<Transform>()->GetParent() == exclute) {
					return true;
				}
			}

			return false;
		}

		float Utility::CalcuSubRange(const std::shared_ptr<GameObject>& selfObject, const Vec3& targetPosition) {
			auto selfPosition = selfObject->GetComponent<Transform>()->GetPosition();
			auto toVec = targetPosition - selfPosition;
			return toVec.length();
		}

		Vec3 Utility::CalcuCameraVec(const Vec3& input, const std::shared_ptr<Camera>& camera, const std::shared_ptr<GameObject>& selfObject)
		{
			Vec3 angle = Vec3(0.0f);

			if (input.x != 0 || input.z != 0)
			{
				//進行方向の向きを計算
				auto front = selfObject->GetComponent<Transform>()->GetPosition() - camera->GetEye();
				front.y = 0;

				float frontAngle = atan2f(front.z, front.x);
				float cntlAngle = atan2f(-input.x, input.z);
				float totalAngle = frontAngle + cntlAngle;

				angle = Vec3(cosf(totalAngle), 0, sinf(totalAngle));

				angle.y = 0;
			}

			return angle;
		}

		Vec3 Utility::ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position)
		{
			const auto camera = view->GetTargetCamera();
			const auto viewport = view->GetTargetViewport();
			float w = viewport.Width * 0.5f;  // 画面の幅の半分
			float h = viewport.Height * 0.5f; // 画面の高さの半分

			auto mView = camera->GetViewMatrix(); // ビュー行列
			auto mProj = camera->GetProjMatrix(); // プロジェクション(射影)行列
			Mat4x4 mScreen( // スクリーン行列（BaseCross用）
				{ w, 0, 0, 0 },
				{ 0, h, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			);

			Mat4x4 mVPS = mView * mProj * mScreen;

			return Vec3(XMVector3TransformCoord(position, mVPS));
		}
	}
}

//enbbasecross