/*!
@file UtilityObstacle.cpp
@brief UtilityObstacleのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "UtilityObstacle.h"
#include "Utility.h"
#include "MaruHitHelper.h"

namespace basecross {
	const std::vector<std::wstring> maru::UtilityObstacle::DEFAULT_OBSTACLE_TAGS = { L"T_Obstacle" };
	const std::vector<std::wstring> maru::UtilityObstacle::DEFAULT_OBSTACLE_AND_FLOOR_TAGS = { L"T_Obstacle", L"Floor" };
	const std::vector<std::wstring> maru::UtilityObstacle::DEFAULT_OBSTACLE_AND_FLOOR_AND_ENEMY_TAGS = { L"T_Obstacle", L"Floor", L"T_Enemy"};
	const std::vector<std::wstring> maru::UtilityObstacle::DEFAULT_OBSTACLE_AND_PLAYER_AND_HIDEPLACE = { L"T_Obstacle", L"T_HidePlace", L"T_Player" };

	bool maru::UtilityObstacle::IsObstacle(const std::shared_ptr<GameObject>& object, const std::vector<wstring>& tags) {
		for (const auto& tag : tags)
		{
			if (object->FindTag(tag)) {  //タグが見つかったら
				return true;
			}
		}

		return false;
	}

	bool maru::UtilityObstacle::IsRayHit(
		const Vec3& startPosition,
		const Vec3& endPosition,
		const std::shared_ptr<GameObject>& object
	){
		auto collision = object->GetComponent<CollisionObb>(false);
		if (!collision) {
			return false;
		}
		if (collision->GetUpdateActive() == false) {
			return false;
		}

		if (collision->GetAfterCollision() == AfterCollision::None) {
			return false;
		}

		if (HitTest::SEGMENT_OBB(startPosition, endPosition, collision->GetObb())) {
			return true;
		}

		return false;
	}

	bool maru::UtilityObstacle::IsRayHit(
		const Vec3& startPosition,
		const Vec3& endPosition,
		const std::shared_ptr<GameObject>& object,
		maru::HitData& hitData
	) {
		if (IsRayHit(startPosition, endPosition, object)) {
			auto collision = object->GetComponent<CollisionObb>(false);
			hitData = HitHelper::ClosestPtPointOBB(startPosition, collision);

			return true;
		}

		return false;
	}

	bool maru::UtilityObstacle::IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition,
		const vector<std::shared_ptr<GameObject>>& objects, const std::vector<wstring>& tags)
	{
		auto obstacleObjects = FindObstacles(objects, tags);
		auto direction = endPosition - startPosition;

		for (const auto& object : obstacleObjects)
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

			if (HitTest::SEGMENT_OBB(startPosition, endPosition, collision->GetObb())) {
				return true;
			}
		}

		return false;
	}

	bool maru::UtilityObstacle::IsRayObstacle(
		const Vec3& startPosition, 
		const Vec3& endPosition,
		const vector<std::weak_ptr<GameObject>>& objects)
	{
		auto direction = endPosition - startPosition;
		float directionRange = direction.length();

		for (const auto& weakObject : objects)
		{
			auto object = weakObject.lock();

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

			auto transform = object->GetComponent<Transform>();
			auto toEndRange = (endPosition - transform->GetPosition()).length();

			if (directionRange < toEndRange) {	//距離が遠いなら判定をしない。
				continue;
			}

			if (HitTest::SEGMENT_OBB(startPosition, endPosition, collision->GetObb())) {
				return true;
			}
		}

		return false;
	}

	std::shared_ptr<GameObject> maru::UtilityObstacle::FindRayHitNearObstacle(
		const Vec3& startPosition, const Vec3& endPosition,
		const vector<std::shared_ptr<GameObject>>& objects,
		const std::vector<wstring>& tags)
	{
		auto direction = endPosition - startPosition;
		auto obstacleObjects = FindObstacles(objects, tags);

		struct Pair {
			std::shared_ptr<GameObject> hitObject;
			float range;
		};

		//当たったオブジェクトの取得
		std::vector<Pair> hitPairs;
		for (const auto& object : obstacleObjects) {
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

			if (HitTest::SEGMENT_OBB(startPosition, endPosition, collision->GetObb())) {
				auto objectPosition = object->GetComponent<Transform>()->GetPosition();
				auto toObject = objectPosition - startPosition;

				Pair pair;
				pair.hitObject = object;
				pair.range = toObject.length();

				hitPairs.push_back(pair);
			}
		}

		if (hitPairs.empty()) {  //どこにもヒットしてないならnullptr
			return nullptr;
		}

		//ソート
		auto sortNearObjectFunction = [](const Pair& left, const Pair& right) { return left.range < right.range; };
		std::sort(hitPairs.begin(), hitPairs.end(), sortNearObjectFunction);

		return hitPairs[0].hitObject; 
	}

	std::shared_ptr<GameObject> maru::UtilityObstacle::FindRayHitNearObstacle(
		const std::shared_ptr<GameObject>& selfObject,
		const std::shared_ptr<GameObject>& targetObject,
		const vector<std::shared_ptr<GameObject>>& objects,
		const std::vector<wstring>& tags
	)
	{
		auto selfPosition = selfObject->GetComponent<Transform>()->GetPosition();
		auto targetPosition = targetObject->GetComponent<Transform>()->GetPosition();

		return FindRayHitNearObstacle(selfPosition, targetPosition, objects, tags);
	}

	bool maru::UtilityObstacle::FindRayHitNearData(
		const Vec3& startPosition,
		const Vec3& endPosition,
		const vector<std::shared_ptr<GameObject>>& objects,
		HitData& hitData,
		const std::vector<wstring>& tags
	) {
		auto direction = endPosition - startPosition;
		auto obstacleObjects = FindObstacles(objects, tags);

		struct Data {
			float range;
			HitData hitData;
		};

		//当たったオブジェクトの取得
		std::vector<Data> datas;
		for (const auto& object : obstacleObjects) {
			Data data;
			if (IsRayHit(startPosition, endPosition, object, data.hitData)) {
				auto objectPosition = object->GetComponent<Transform>()->GetPosition();
				auto toObject = objectPosition - startPosition;

				data.hitData.hitObject = object;
				data.range = toObject.length();

				datas.push_back(data);
			}
		}

		if (datas.empty()) {  //どこにもヒットしてないならfalse
			return false;
		}

		//ソート
		auto sortNearDataFunction = [](const Data& left, const Data& right) { return left.range < right.range; };
		std::sort(datas.begin(), datas.end(), sortNearDataFunction);

		hitData = datas[0].hitData;
		return true;
	}
	
}

//endbasecross