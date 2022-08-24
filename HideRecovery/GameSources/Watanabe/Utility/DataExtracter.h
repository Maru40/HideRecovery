﻿#pragma once
#include "stdafx.h"
#include "TransformData.h"
#include "Utility.h"

namespace basecross {
	/**
	 * @brief 文字列の配列から各種データを抽出するクラス
	 */
	struct DataExtracter {
		/**
		 * @brief 1行のCSVデータを配列に格納
		 *
		 * @param line 1行のCSVデータ
		 * @return 配列
		 */
		static vector<wstring> DelimitData(const wstring& line, wchar_t delimiter = L',') {
			vector<wstring> tokens;
			Util::WStrToTokenVector(tokens, line, delimiter);
			return tokens;
		}

		/**
		 * @brief Vector3を抽出
		 *
		 * @param tokens 文字列
		 * @param firstIndex 最初のインデックス（デフォルト：0）
		 * @return Vector3データ
		 */
		static Vec3 Vector3DataExtraction(const vector<wstring>& tokens, size_t firstIndex = 0)
		{
			Vec3 vec;
			vec = Vec3(
				(float)_wtof(tokens[firstIndex + 0].c_str()),
				(float)_wtof(tokens[firstIndex + 1].c_str()),
				(float)_wtof(tokens[firstIndex + 2].c_str())
			);
			return vec;
		};

		/**
		 * @brief Vector2を抽出
		 *
		 * @param tokens 文字列
		 * @param firstIndex 最初のインデックス（デフォルト：0）
		 * @return Vector2データ
		 */
		static Vec2 Vector2DataExtraction(const vector<wstring>& tokens, size_t firstIndex = 0)
		{
			Vec2 vec;
			vec = Vec2(
				(float)_wtof(tokens[firstIndex + 0].c_str()),
				(float)_wtof(tokens[firstIndex + 1].c_str())
			);
			return vec;
		};

		/**
		 * @brief トランスフォームデータを抽出
		 *
		 * @param tokens 文字列
		 * @param transData データの出力先
		 * @param firstIndex 最初のインデックス（デフォルト：1）
		 * @return トランスフォームデータ
		 */
		static size_t TransformDataExtraction(const vector<wstring>& tokens, TransformData& transData, size_t firstIndex = 1) {
			transData.Position = Vector3DataExtraction(tokens, firstIndex);
			transData.Scale = Vector3DataExtraction(tokens, firstIndex + 3);
			transData.Rotation =
				Utility::ConvertDegVecToRadVec(Vector3DataExtraction(tokens, firstIndex + 6));
			return firstIndex + 9;
		}

		/**
		 * @brief レクトトランスフォームデータを抽出
		 *
		 * @param tokens 文字列
		 * @param rectTransData データの出力先
		 * @param firstIndex 最初のインデックス（デフォルト：2）
		 * @return 次のインデックス
		 */
		static size_t RectTransformDataExtraction(const vector<wstring>& tokens, RectTransformData& rectTransData, size_t firstIndex = 2) {
			rectTransData.Position = Vector2DataExtraction(tokens, firstIndex);
			rectTransData.Scale = Vector2DataExtraction(tokens, firstIndex + 2);
			rectTransData.Rotation = XMConvertToRadians((float)_wtof(tokens[firstIndex + 4].c_str()));
			return firstIndex + 5;
		}

		/**
		 * @brief カラーデータを抽出
		 *
		 * @param tokens 文字列
		 * @return カラーデータ
		 */
		static Col4 ColorDataExtraction(const vector<wstring>& tokens, size_t firstIndex = 0)
		{
			Col4 color;
			color = Col4(
				(float)_wtof(tokens[firstIndex + 0].c_str()),
				(float)_wtof(tokens[firstIndex + 1].c_str()),
				(float)_wtof(tokens[firstIndex + 2].c_str()),
				(float)_wtof(tokens[firstIndex + 3].c_str())
			);
			return color;
		};
	};
}
