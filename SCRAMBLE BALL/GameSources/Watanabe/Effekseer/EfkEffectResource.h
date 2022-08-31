#pragma once
#include "stdafx.h"
#include "EfkEffect.h"

namespace basecross {
	/**
	 * @brief .efkファイルのデータクラス
	 */
	class EfkEffectResource :public BaseResource {
		// ファイルパス
		wstring m_filePath;
		// エフェクトデータのポインタ
		Effekseer::EffectRef m_effect;
	public:
		/**
		 * @brief コンストラクタ
		 *
		 * @param path ファイルパス
		 */
		EfkEffectResource(const wstring& path);

		/**
		 * @brief エフェクトデータのファイルパスを取得
		 *
		 * @return ファイルパス
		 */
		wstring GetEffectFilePath()const {
			return m_filePath;
		}

		/**
		 * @brief エフェクトデータを取得
		 *
		 * @return エフェクトデータのポインタ
		 */
		Effekseer::EffectRef GetEffectData()const {
			return m_effect;
		}

		/**
		 * @brief エフェクトデータを登録
		 *
		 * @param key		識別用キー
		 * @param filePath	ファイルパス
		 * @return データクラスのポインタ
		 */
		static shared_ptr<EfkEffectResource> RegisterEffectResource(
			const wstring& key, const wstring& filePath);
	};
}
