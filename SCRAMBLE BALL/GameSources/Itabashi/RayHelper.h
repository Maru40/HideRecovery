#pragma once

namespace basecross
{
namespace Physics
{
	/// <summary>
	/// レイのヘルパークラス
	/// </summary>
	class RayHelper
	{
	public:
		RayHelper() = delete;
		RayHelper(const RayHelper&) = delete;
		RayHelper& operator=(const RayHelper&) = delete;

		/// <summary>
		/// レイを飛ばして当たった相手を取得する
		/// </summary>
		/// <param name="startPosition">レイの開始地点</param>
		/// <param name="direction">レイの方向</param>
		/// <param name="hitData">レイが当たった際のデータ</param>
		/// <returns>当たった相手のコリジョン(当たってないならnullptr)</returns>
		static std::shared_ptr<Collision> RayCast(const Vec3& startPosition, const Vec3& direction, RayHitData& hitData = RayHitData());

		/// <summary>
		/// レイを飛ばして当たった相手を取得する
		/// </summary>
		/// <param name="startPosition">レイの開始地点</param>
		/// <param name="direction">レイの方向</param>
		/// <param name="maxDistance">レイの最大距離</param>
		/// <param name="hitData">レイが当たった際のデータ</param>
		/// <returns>当たった相手のコリジョン(当たってないならnullptr)</returns>
		static std::shared_ptr<Collision> RayCast(const Vec3& startPosition, const Vec3& direction, float maxDistance, RayHitData& hitData = RayHitData());

		/// <summary>
		/// レイを飛ばして当たった相手を取得する
		/// </summary>
		/// <param name="startPosition">レイの開始地点</param>
		/// <param name="direction">レイの方向</param>
		/// <param name="hitTags">当たる判定のタグ配列</param>
		/// <param name="tagsSize">タグ配列の長さ</param>
		/// <param name="hitData">レイが当たった際のデータ</param>
		/// <returns>当たった相手のコリジョン(当たってないならnullptr)</returns>
		static std::shared_ptr<Collision> RayCast(const Vec3& startPosition, const Vec3& direction, const std::wstring* hitTags, size_t tagsSize, RayHitData& hitData = RayHitData());

		/// <summary>
		/// レイを飛ばして当たった相手を取得する
		/// </summary>
		/// <param name="startPosition">レイの開始地点</param>
		/// <param name="direction">レイの方向</param>
		/// <param name="maxDistance">レイの最大距離</param>
		/// <param name="hitTags">当たる判定のタグ配列</param>
		/// <param name="tagsSize">タグ配列の長さ</param>
		/// <param name="hitData">レイが当たった際のデータ</param>
		/// <returns>当たった相手のコリジョン(当たってないならnullptr)</returns>
		static std::shared_ptr<Collision> RayCast(const Vec3& startPosition, const Vec3& direction, float maxDistance, const std::wstring* hitTags, size_t tagsSize, RayHitData& hitData = RayHitData());
	};
}
}