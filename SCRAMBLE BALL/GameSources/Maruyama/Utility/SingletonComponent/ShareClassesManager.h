/*!
@file ShareClassesManager.h
@brief ShareClassManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 現在使用不可
	//--------------------------------------------------------------------------------------
	class I_ShareClass
	{
	public:
			
	};

	//--------------------------------------------------------------------------------------
	/// 現在使用不可
	//--------------------------------------------------------------------------------------
	template<class T>
	class ShareCalss : public I_ShareClass
	{
		std::vector<std::weak_ptr<T>> m_shareClasses;	//シェアクラスの配列
	public:
		void AddShareClass(const std::shared_ptr<T>& shareClass) noexcept { m_shareClasses.push_back(shareClass); }

		std::vector<std::weak_ptr<T>> GetShareClasses() const noexcept { return m_shareClasses; }
	};

	//--------------------------------------------------------------------------------------
	/// クラスのシェアを管理するクラス
	//--------------------------------------------------------------------------------------
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
		bool AddShareClass(const std::shared_ptr<ObjectInterface>& objPtr) {
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
			return HasShareType(index) ? m_shareClassesMap.at(index) : ShareClassVector();
		}

		/// <summary>
		/// シェアクラスをテンプレートの型にキャストして取得する。
		/// </summary>
		template<class T>
		std::vector<std::weak_ptr<T>> GetCastShareClasses() const {
			std::vector<std::weak_ptr<T>> result;
			auto index = type_index(typeid(T));

			if (!HasShareType(index)) {	//存在しないなら、空の配列を返す。
				return result;
			}

			//キャストして配列に入れる。
			for (auto& shareClass : m_shareClassesMap.at(index)) {
				auto tPtr = std::dynamic_pointer_cast<T>(shareClass.lock());
				if (tPtr) {	//キャストに成功したら配列にpushback
					result.push_back(tPtr);
				}
			}

			return result;
		}

	};

}