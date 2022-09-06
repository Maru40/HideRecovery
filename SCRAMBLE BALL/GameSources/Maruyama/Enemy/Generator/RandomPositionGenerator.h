
/*!
@file RandomPositionGenerator.h
@brief RandomPositionGeneratorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "GeneratorBase.h"
#include "MyRandom.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ランダム位置生成パラメータ
	//--------------------------------------------------------------------------------------
	struct RandomPositionGenerator_Parametor
	{
		int numCreate;        //生成数
		Vec3 centerPosition;  //生成位置の中心
		Vec3 createRangeVec;  //生成する範囲

		/// <summary>
		/// コンストラクタ
		/// </summary>
		RandomPositionGenerator_Parametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="numCreate">生成数</param>
		RandomPositionGenerator_Parametor(const int& numCreate);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="numCreate">生成数</param>
		/// <param name="centerPosition">生成位置の中心</param>
		/// <param name="createRangeVec">生成範囲のベクトル</param>
		RandomPositionGenerator_Parametor(const int& numCreate,
			const Vec3& centerPosition, const Vec3& createRangeVec);
	};

	//--------------------------------------------------------------------------------------
	/// ランダム位置生成ジェネレータ
	//--------------------------------------------------------------------------------------
	template<class Type>
	class RandomPositionGenerator : public GeneratorBase
	{
	public:
		using Parametor = RandomPositionGenerator_Parametor;

	private:
		Parametor m_param;  //パラメータ

		/// <summary>
		/// 生成する場所をランダムに取得
		/// </summary>
		/// <returns>生成する場所</returns>
		Vec3 CalculatePosition() {
			auto randomPosition = maru::MyRandom::RandomPosition(m_param.createRangeVec);
			return m_param.centerPosition + randomPosition;
		}

		/// <summary>
		/// オブジェクトの生成
		/// </summary>
		/// <param name="position">生成位置</param>
		/// <returns>生成したオブジェクト</returns>
		std::shared_ptr<GameObject> CreateObject(const Vec3& position) {
			auto object = GetStage()->AddGameObject<Type>();
			auto objTrans = object->GetComponent<Transform>();
			objTrans->SetPosition(position);
			return object;
		}

		/// <summary>
		/// パラメータの生成数分、オブジェクトを生成する
		/// </summary>
		void CreateObjects() {
			for (int i = 0; i < m_param.numCreate; i++) {
				auto object = CreateObject(CalculatePosition());
				m_objects.push_back(object);
			}
		}

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		RandomPositionGenerator(const std::shared_ptr<GameObject>& objPtr)
			:RandomPositionGenerator(objPtr, Parametor())
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		RandomPositionGenerator(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
			:GeneratorBase(objPtr), m_param(parametor)
		{}

		void OnCreate() override {
			CreateObjects();
		}

	};

}