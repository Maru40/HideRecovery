/*!
@file UtilityObstacle.h
@brief UtilityObstacleなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Utility.h"

namespace basecross {
	namespace maru
	{
		struct HitData;

		class UtilityObstacle
		{
		public:
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_TAGS;                      //デフォルトの障害物タグ
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_AND_FLOOR_TAGS;            //デフォルトの障害物タグと床タグ
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_AND_FLOOR_AND_ENEMY_TAGS;  //デフォルトの障害物タグと床タグとエネミータグ

			/// <summary>
			/// 障害物かどうか判断する
			/// </summary>
			/// <param name="object">判断したいオブジェクト</param>
			/// <param name="tags">障害物タグ</param>
			/// <returns>障害物ならtrue</returns>
			static bool IsObstacle(const std::shared_ptr<GameObject>& object, const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS);

			/// <summary>
			/// Rayを飛ばしてヒットしているか判断
			/// </summary>
			/// <param name="startPosition">Rayの開始位置</param>
			/// <param name="endPosition">Rayの終了位置</param>
			/// <param name="object">対象のオブジェクト</param>
			/// <returns>ヒットしたらtrue</returns>
			static bool IsRayHit(
				const Vec3& startPosition,
				const Vec3& endPosition,
				const std::shared_ptr<GameObject>& object
			);

			/// <summary>
			/// Rayを飛ばしてヒットしているか判断
			/// </summary>
			/// <param name="startPosition">Rayの開始位置</param>
			/// <param name="endPosition">Rayの終了位置</param>
			/// <param name="object">対象のオブジェクト</param>
			/// <param name="hitData">ヒットデータ</param>
			/// <returns>ヒットしたらtrue</returns>
			static bool IsRayHit(
				const Vec3& startPosition,
				const Vec3& endPosition,
				const std::shared_ptr<GameObject>& object,
				HitData& hitData
			);

			/// <summary>
			/// Rayを飛ばして障害物にヒットしたか判断
			/// </summary>
			/// <param name="startPosition">Ray開始位置</param>
			/// <param name="endPosition">Rayの終了位置</param>
			/// <param name="objects">判断したい障害物配列</param>
			/// <param name="tags">障害物とするタグ</param>
			/// <returns>ヒットしたらtrue</returns>
			static bool IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition,
				const vector<std::shared_ptr<GameObject>>& objects, const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			/// <summary>
			/// 一番近い障害物を見つける
			/// </summary>
			/// <param name="startPosition">Rayの開始位置</param>
			/// <param name="endPosition">Rayの終了位置</param>
			/// <param name="objects">判断したい障害物配列</param>
			/// <param name="tags">障害物とするタグ</param>
			/// <returns>ヒットしたらtrue</returns>
			static std::shared_ptr<GameObject> FindRayHitNearObstacle(
				const Vec3& startPosition, 
				const Vec3& endPosition,
				const vector<std::shared_ptr<GameObject>>& objects,
				const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			/// <summary>
			/// 一番近い障害物を見つける
			/// </summary>
			/// <param name="startPosition">Rayの開始位置にしたいオブジェクト</param>
			/// <param name="endPosition">Rayの終了位置にしたいオブジェクト</param>
			/// <param name="objects">判断したい障害物配列</param>
			/// <param name="tags">障害物とするタグ</param>
			/// <returns>ヒットしたらtrue</returns>
			static std::shared_ptr<GameObject> FindRayHitNearObstacle(
				const std::shared_ptr<GameObject>& selfObject, 
				const std::shared_ptr<GameObject>& targetObject,
				const vector<std::shared_ptr<GameObject>>& objects,
				const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			//現在使用禁止
			static bool FindRayHitNearData(
				const Vec3& startPosition,
				const Vec3& endPosition,
				const vector<std::shared_ptr<GameObject>>& objects,
				HitData& hitData,
				const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			/// <summary>
			/// 指定したタグの障害物を取得する。
			/// </summary>
			/// <param name="tVec">判断したい障害物配列</param>
			/// <param name="tags">障害物とするタグ</param>
			/// <returns>ヒットしたオブジェクトの配列</returns>
			template<class T>
			static vector<T> FindObstacles(const vector<T>& tVec, const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS)
			{
				vector<T> reVec;

				for (const auto& t : tVec)
				{
					for (const auto& tag : tags)
					{
						if (t->FindTag(tag)) {  //タグが見つかったら
							reVec.push_back(t);
						}
					}
				}

				return reVec;
			}
			
		};
	}
}

//endbasecross