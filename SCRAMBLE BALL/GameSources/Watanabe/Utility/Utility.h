/*!
@file Utility.h
@brief いろんな関数
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Utility {
		/**
		 * @brief 配列のサイズを返す関数
		 *
		 * @param TYPE 対象の配列のポインタ
		 * @return size_t 配列のサイズ
		 */
		template <typename TYPE, size_t SIZE>
		size_t GetArrayLength(const TYPE(&)[SIZE]) {
			return SIZE;
		}

		/**
		 * @brief テクスチャのサイズを返す関数
		 *
		 * @param key テクスチャキー
		 * @return const Vec2 テクスチャサイズ
		 */
		const Vec2 GetTextureSize(const wstring& key);

		/**
		 * @brief テクスチャの座標をUV座標に変換する関数（単体）
		 *
		 * @param coordinate テクスチャ座標の位置
		 * @param textureSize テクスチャサイズ
		 * @return const Vec2 UV座標
		 */
		const Vec2 ConvertToUVCoordinate(const Vec2& coordinate, const Vec2& textureSize);
		/**
		 * @brief テクスチャの座標をUV座標に変換する関数（単体）
		 *
		 * @param coordinate テクスチャ座標の位置
		 * @param textureName テクスチャキー
		 * @return const Vec2 UV座標
		 */
		const Vec2 ConvertToUVCoordinate(const Vec2& coordinate, const wstring& textureName);
		/**
		 * @brief テクスチャの座標をUV座標に変換する関数（複数）
		 *
		 * @param coordinates テクスチャ座標の位置のvector配列
		 * @param textureSize テクスチャサイズ
		 * @param uv UV座標の位置のvector配列（出力）
		 */
		void ConvertToUVCoordinates(const vector<Vec2>& coordinates, const Vec2& textureSize, vector<Vec2>& uv);
		/**
		 * @brief テクスチャの座標をUV座標に変換する関数（複数）
		 *
		 * @param coordinates テクスチャ座標の位置のvector配列
		 * @param textureName テクスチャキー
		 * @param uv UV座標の位置のvector配列（出力）
		 */
		void ConvertToUVCoordinates(const vector<Vec2>& coordinates, const wstring& textureName, vector<Vec2>& uv);

		/**
		 * @brief Vec3のDegをRadに変換
		 *
		 * @param deg 角度（Deg）
		 * @return Vec3 角度（Rad）
		 */
		Vec3 ConvertDegVecToRadVec(const Vec3& deg);
		/**
		 * @brief Vec3のRadをDegに変換
		 *
		 * @param rad 角度（Rad）
		 * @return Vec3 角度（Deg）
		 */
		Vec3 ConvertRadVecToDegVec(const Vec3& rad);

		/**
		 * @brief Vec2のDegをRadに変換
		 *
		 * @param deg 角度（Deg）
		 * @return Vec2 角度（Rad）
		 */
		Vec2 ConvertDegVecToRadVec(const Vec2& deg);
		/**
		 * @brief Vec2のRadをDegに変換
		 *
		 * @param rad 角度（Rad）
		 * @return Vec2 角度（Deg）
		 */
		Vec2 ConvertRadVecToDegVec(const Vec2& rad);

		/**
		 * @brief 2つの角度（Deg）が同じかどうかを調べる
		 *
		 * @param deg1 角度1
		 * @param deg2 角度2
		 * @return bool true:等しい/false:異なる
		 */
		bool MatchAngle(float deg1, float deg2);

		/**
		 * @brief 角度（Deg）をmin～maxの間に収める（minとmaxの差は360である必要がある）
		 *
		 * @param min 最小角度
		 * @param max 最大角度
		 * @param angle 収めたい角度
		 * @return float min～maxの間のangle
		 */
		float ClampAngle(float min, float max, float angle);

		/**
		 * @brief wstringをboolに変換
		 *
		 * @param str 文字列（wstring）
		 * @return bool bool型
		 */
		bool WStrToBool(wstring str);

		/**
		 * @brief Col4の0～255を0～1に収める
		 *
		 * @param color 0～255の範囲の色データ
		 * @return Col4 0～1の範囲の色データ
		 */
		Col4 ConvertColorZeroToOne(const Col4& color);

		Vec2 ConvertVec2to01(const Vec2& vec);

		/**
		 * @brief ベクトルの長さを変更する
		 *
		 * @param vector 変更したいベクトル
		 * @param length 指定する長さ
		 * @return 長さがlengthのベクトル
		 */
		Vec3 ChangeVectorLength(const Vec3& vector, float length);

		/**
		 * @brief ワールド座標をスクリーン座標に変更する
		 *
		 * @param view ビュー
		 * @param position ワールド座標
		 * @return Vec3 スクリーン座標
		 */
		Vec3 ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position);

		/**
		 * @brief 2つのベクトルのなす角を求める
		 *
		 * @param vector1 ベクトル1
		 * @param vector2 ベクトル2
		 * @return float 角度（deg）
		 */
		float GetTwoVectorAngle(const Vec3& vector1, const Vec3& vector2);

		Vec3 ClampVector3(const Vec3& value, const Vec3& _min, const Vec3& _max);

		/**
		 * @brief クランプ
		 *
		 * @param value 値
		 * @param low 最小値
		 * @param high 最大値
		 * @return クランプされた値
		 */
		template<typename T>
		T Clamp(T value, T low, T high) {
			return min(max(value, low), high);
		}

		/**
		 * @brief リマップ
		 */
		float Remap(float val, float inMin, float inMax, float outMin, float outMax);

		/**
		 * @brief クォータニオンをオイラー角に変換
		 */
		Vec3 QuaternionToEulerAngle(const Quat& quat);

		/**
		 * @brief 親からの相対位置をワールド座標に変換する
		 */
		Vec3 ParentRelativePositionToWorldPosition(const shared_ptr<Transform>& transPtr, const Vec3& relativePosition);
	}
}
//end basecross
