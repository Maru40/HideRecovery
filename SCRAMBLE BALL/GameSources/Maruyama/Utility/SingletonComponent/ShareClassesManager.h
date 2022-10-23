/*!
@file ShareClassesManager.h
@brief ShareClassManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	class ShareClassesManager : public maru::SingletonComponent<ShareClassesManager>
	{
	public:
		using ShareClassVector = std::vector<std::weak_ptr<ObjectInterface>>;
		using ShareClassesMap = std::unordered_map<type_index, ShareClassVector>;

	private:
		ShareClassesMap m_shareClassesMap;	//シェアするクラスのマップ

	public:
		ShareClassesManager(std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// テンプレートクラスのタイプIDを取得
		/// </summary>
		template<class T>
		type_index GetTypeIndex() const noexcept {
			return type_index(typeid(T));
		}

		/// <summary>
		/// 指定したインデックスがすでに登録されているか確認する。
		/// </summary>
		/// <param name="index">クラスのタイプインデックス</param>
		/// <returns>登録されていたらtrue</returns>
		bool HasShareType(const type_index index) const noexcept {
			return m_shareClassesMap.count(index) != 0;
		}

		/// <summary>
		/// テンプレートで指定したクラスがすでに登録されているか確認する。
		/// </summary>
		template<class T>
		bool HasShareType() const noexcept {
			auto index = type_index(typeid(T));
			return HasShareType(index);
		}

		/// <summary>
		/// シェアするクラスを取得する。
		/// </summary>
		template<class T>
		bool AddShareClass(const std::shared_ptr<T>& objPtr) {
			auto index = type_index(typeid(T));
			m_shareClassesMap[index].push_back(objPtr);
			return true;
		}

		/// <summary>
		/// シェアクラスの取得
		/// </summary>
		template<class T>
		ShareClassVector GetShareClasses() const noexcept {
			auto index = type_index(typeid(T));
			return HasShareType(index) ? m_shareClassesMap.at(index) : ShareClassVector();	//タイプを持っていたらそのオブジェクトのタイプを返す。
		}
	};

}