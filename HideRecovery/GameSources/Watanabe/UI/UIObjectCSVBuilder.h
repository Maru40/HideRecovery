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
	public:
		UIObjectCSVBuilder();
		~UIObjectCSVBuilder();

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
		void Build(const shared_ptr<Stage>& stage, const wstring& fileName);
	};
}
