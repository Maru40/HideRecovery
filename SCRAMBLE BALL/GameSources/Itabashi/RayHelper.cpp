#include "RayHelper.h"
#include "Scene.h"

namespace basecross
{
namespace Physics
{
	/// <summary>
	/// レイが衝突可能なオブジェクトか
	/// </summary>
	/// <param name="gameObject">調べるオブジェクト</param>
	/// <param name="hitTags">衝突確認するタグ配列</param>
	/// <param name="tagsSize">タグ配列サイズ</param>
	/// <returns>衝突可能ならtrue</returns>
	bool IsHittableTagToGameObject(const std::shared_ptr<GameObject>& gameObject, const std::wstring* hitTags, size_t tagsSize)
	{
		if (!hitTags || tagsSize == 0)
		{
			return true;
		}

		int count = 0;

		while (count < tagsSize)
		{
			if (gameObject->FindTag(hitTags[count]))
			{
				return true;
			}

			++count;
		}

		return false;
	}

	std::shared_ptr<Collision> RayHelper::RayCast(const Vec3& startPosition, const Vec3& direction, RayHitData& hitData)
	{
		return RayCast(startPosition, direction, FLT_MAX, nullptr, 0, hitData);
	}

	std::shared_ptr<Collision> RayHelper::RayCast(const Vec3& startPosition, const Vec3& direction, float maxDistance, RayHitData& hitData)
	{
		return RayCast(startPosition, direction, maxDistance, nullptr, 0, hitData);
	}

	std::shared_ptr<Collision> RayHelper::RayCast(const Vec3& startPosition, const Vec3& direction, const std::wstring* hitTags, size_t tagsSize, RayHitData& hitData)
	{
		return RayCast(startPosition, direction, FLT_MAX, hitTags, tagsSize, hitData);
	}

	std::shared_ptr<Collision> RayHelper::RayCast(const Vec3& startPosition, const Vec3& direction, float maxDistance, const std::wstring* hitTags, size_t tagsSize, RayHitData& hitData)
	{
		RayHitData resultHitData;
		resultHitData.length = maxDistance;

		std::shared_ptr<Collision> hitCollision;

		for (const auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
		{
			if (!IsHittableTagToGameObject(gameObject, hitTags, tagsSize))
			{
				continue;
			}

			auto collision = gameObject->GetComponent<Collision>(false);

			if (!collision)
			{
				continue;
			}


			RayHitData hitData;

			if (!collision->IsRayHit(startPosition, direction, hitData) || hitData.length > resultHitData.length)
			{
				continue;
			}


			resultHitData = hitData;
			hitCollision = collision;
		}

		if (!hitCollision)
		{
			return nullptr;
		}

		hitData = resultHitData;

		return hitCollision;
	}
}
}