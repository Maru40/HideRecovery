
/*!
@file GeneratorBase.h
@brief GeneratorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ジェネレータの基底クラス
	//--------------------------------------------------------------------------------------
	class GeneratorBase : public Component
	{
	protected:
		std::vector<ex_weak_ptr<GameObject>> m_objects; //生成したオブジェクトの配列

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		GeneratorBase(const std::shared_ptr<GameObject>& objPtr);

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 生成したオブジェクト数の取得
		/// </summary>
		/// <returns></returns>
		virtual int GetNumObject() const;

		/// <summary>
		/// 生成したオブジェクトの配列を取得
		/// </summary>
		/// <returns></returns>
		std::vector<ex_weak_ptr<GameObject>> GetObjects() const;

		/// <summary>
		/// オブジェクトの追加
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject>& object) {
			m_objects.push_back(object);
		}
	};

}

//endbasecross