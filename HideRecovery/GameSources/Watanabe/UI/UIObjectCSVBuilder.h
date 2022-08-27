#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class UIObjectCreatorBaseCSV {
	protected:
		explicit UIObjectCreatorBaseCSV() {}
		virtual ~UIObjectCreatorBaseCSV() {}
	public:
		/// <summary>
		/// UIオブジェクトを作成する（純粋仮想関数）
		/// </summary>
		/// <param name="stage">ステージ</param>
		/// <param name="line">1行分のCSV文字列</param>
		/// <returns>UIオブジェクトのポインタ</returns>
		virtual shared_ptr<UIObjectBase> Create(const shared_ptr<Stage>& stage, const wstring& line) = 0;
	};

	template<typename T>
	class UIObjectCreatorCSV : public UIObjectCreatorBaseCSV {
	public:
		explicit UIObjectCreatorCSV() {}
		virtual ~UIObjectCreatorCSV() {}

		/// <summary>
		/// UIオブジェクトを作成する（仮想関数）
		/// </summary>
		/// <param name="StagePtr">ステージ</param>
		/// <param name="Line">1行分のCSV文字列</param>
		/// <returns>UIオブジェクトのポインタ</returns>
		shared_ptr<UIObjectBase> Create(const shared_ptr<Stage>& stage, const wstring& line)override {
			return stage->AddGameObject<T>(line);
		}
	};

	class UIObjectCSVBuilder {
		map<wstring, shared_ptr<UIObjectCreatorBaseCSV>> m_creatorMap;
		// 生成したオブジェクトマップ
		map<wstring, shared_ptr<UIObjectBase>> m_uiObjectMap;
	public:
		UIObjectCSVBuilder();
		~UIObjectCSVBuilder();

		/// <summary>
		/// 自身のインスタンスを生成
		/// </summary>
		/// <returns>インスタンス</returns>
		static shared_ptr<UIObjectCSVBuilder> Create();

		/// <summary>
		/// クリエーターを登録する
		/// </summary>
		/// <typeparam name="T">登録する型（ゲームオブジェクトの派生）</typeparam>
		/// <param name="name">型の識別名</param>
		template<typename T>
		void Register(const wstring& name) {
			m_creatorMap[name] = shared_ptr<UIObjectCreatorCSV<T>>(new UIObjectCreatorCSV<T>());
		}

		/// <summary>
		/// 型の識別名を使ってUIオブジェクトを構築する
		/// </summary>
		/// <param name="name">型の識別名</param>
		/// <param name="stage">所属するステージ</param>
		/// <param name="line">1行分のCSV文字列</param>
		/// <returns>作成したUIオブジェクト</returns>
		shared_ptr<UIObjectBase> CreateFromCSV(const wstring& name, const shared_ptr<Stage>& stage, const wstring& line);

		/// <summary>
		/// CSVからUIオブジェクトを構築する
		/// </summary>
		/// <param name="stage">所属するステージ</param>
		/// <param name="fileName">CSVファイル名</param>
		/// <param name="parent">親になるオブジェクト</param>
		void Build(const shared_ptr<Stage>& stage, const wstring& fileName, const shared_ptr<GameObject>& parent = nullptr);

		/// <summary>
		/// 生成したUIオブジェクトを取得する
		/// </summary>
		/// <param name="uiName">UIオブジェクトの名前</param>
		/// <returns>UIオブジェクト</returns>
		shared_ptr<UIObjectBase> GetUIObject(const wstring& uiName);

		/// <summary>
		/// 生成したUIオブジェクトを取得する（型変換あり）
		/// </summary>
		/// <typeparam name="T">取得する型</typeparam>
		/// <param name="uiName">UIオブジェクトの名前</param>
		/// <param name="exceptionActive">例外を出すか</param>
		/// <returns>UIオブジェクト</returns>
		template<typename T>
		shared_ptr<T> GetUIObject(const wstring& uiName, bool exceptionActive = true) {
			// uiNameのペアの存在確認
			if (m_uiObjectMap.count(uiName) > 0) {
				shared_ptr<T> ptr = dynamic_pointer_cast<T>(m_uiObjectMap[uiName]);
				// キャストがうまく行けばそのまま返す
				if (ptr) {
					return ptr;
				}
				else {
					if (exceptionActive) {
						throw BaseException(
							L"オブジェクトを" + Util::GetWSTypeName<T>() + L"型にキャストできません",
							L"uiName : " + uiName,
							L"UIObjectCSVBuilder::GetUIObject()"
						);
					}
				}
			}
			else if (exceptionActive) {
				throw BaseException(
					L"オブジェクトが見つかりませんでした",
					L"uiName : " + uiName,
					L"UIObjectCSVBuilder::GetUIObject()"
				);
			}
			return nullptr;
		}

		/// <summary>
		/// 生成したすべてのオブジェクトを取得
		/// </summary>
		/// <returns></returns>
		vector<shared_ptr<UIObjectBase>> GetUIObjects();
	};
}
