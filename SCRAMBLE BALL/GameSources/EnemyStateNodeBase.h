
/*!
@file EnemyStateNodeBase.h
@brief EnemyStateNodeBase
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"

namespace basecross {

	/// <summary>
	/// エネミー用ステートノードの基底クラス
	/// </summary>
	/// <typeparam name="EnemyType">エネミータイプ</typeparam>
	template<class EnemyType>
	class EnemyStateNodeBase : public StateNodeBase<EnemyType>
	{
		/// <summary>
		/// コンポーネントの変更関係の情報をまとめた構造体
		/// </summary>
		struct ChangeComponentParametor 
		{
			std::shared_ptr<Component> component; //コンポーネントのポインタ
			bool isStart;                         //開始時にどっちにするか？
			bool isExit;                          //終了時にどっちにするか？

			ChangeComponentParametor(
				const std::shared_ptr<Component>& component,
				const bool isStart,
				const bool isExit
			) :
				component(component),
				isStart(isStart),
				isExit(isExit)
			{}
		};

		//コンポーネント変更用のパラメータ配列
		std::vector<ChangeComponentParametor> m_changeParams;

	protected:

		/// <summary>
		/// 開始と終了時に切り替えるコンポーネントの追加
		/// </summary>
		/// <param name="component">切り替えるコンポーネントのポインタ</param>
		/// <param name="isStart">スタート時にどっちに切り替える</param>
		/// <param name="isExit">終了時にどっちに切り替えるか</param>
		void AddChangeComponent(
			const std::shared_ptr<Component>& component,
			const bool isStart, 
			const bool isExit)
		{
			if (component == nullptr) {  //nullptrなら追加しない
				return;
			}

			ChangeComponentParametor param(component, isStart, isExit);
			m_changeParams.push_back(param);
		}

		/// <summary>
		/// 開始と終了時に切り替えるコンポーネントの追加
		/// </summary>
		/// <param name="param">切り替える情報をまとめたパラメータ</param>
		void AddChangeComponent(const ChangeComponentParametor& param) {
			if (param.component == nullptr) {
				return;
			}
			m_changeParams.push_back(param);
		}

		/// <summary>
		/// 登録されたコンポーネントの切り替えを行う(スタート時)
		/// </summary>
		void StartChangeComponents() {
			for (auto& param : m_changeParams) {
				param.component->SetUpdateActive(param.isStart);
			}
		}

		/// <summary>
		/// 登録されたコンポーネントの切り替えを行う(終了時)
		/// </summary>
		void ExitChangeComponents() {
			for (auto& param : m_changeParams) {
				param.component->SetUpdateActive(param.isExit);
			}
		}

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">このクラスを所有するクラス</param>
		EnemyStateNodeBase(const std::shared_ptr<EnemyType>& owner)
			:StateNodeBase<EnemyType>(owner)
		{}

		virtual ~EnemyStateNodeBase() {}

	};

}

//endbasecross