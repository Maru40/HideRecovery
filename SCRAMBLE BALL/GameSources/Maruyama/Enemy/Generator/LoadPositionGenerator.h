
/*!
@file LoadPositionGenerator.h
@brief LoadPositionGeneratorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "GeneratorBase.h"
#include "Maruyama/Utility/Random.h"
#include "Maruyama/Load/StageMapCSV.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 生成用データをロードするためのパラメータ
	//--------------------------------------------------------------------------------------
	struct LoadPositionGenerator_Parametor {
		wstring objectName;  //オブジェクトの名前
		wstring folderName;  //フォルダネーム
		wstring fileName;    //ファイルネーム

		LoadPositionGenerator_Parametor();
		LoadPositionGenerator_Parametor(const wstring& objectName, const wstring& folderName, const wstring& fileName);
	};

	//--------------------------------------------------------------------------------------
	/// 生成位置をロードして生成するジェネレータ
	//--------------------------------------------------------------------------------------
	template<class T>
	class LoadPositionGenerator : public GeneratorBase
	{
	public:
		using Parametor = LoadPositionGenerator_Parametor;

	private:
		Parametor m_param;  //パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="objectName">生成するオブジェクト名</param>
		/// <param name="folderName">読み込むフォルダー名</param>
		/// <param name="fileName">読み込むファイル名</param>
		LoadPositionGenerator(const std::shared_ptr<GameObject>& objPtr, const wstring& objectName, const wstring& folderName, const wstring& fileName) :
			LoadPositionGenerator(objPtr, Parametor(objectName, folderName, fileName))
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">生成用パラメータ</param>
		LoadPositionGenerator(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
			GeneratorBase(objPtr), m_param(parametor)
		{}

		void OnCreate() override {
			//ファイルから生成したい場所を取得
			auto datas = StageMapCSV::GetPositionScaleRotations(m_param.objectName, m_param.folderName, m_param.fileName);
			for (auto& data : datas) {
				auto object = GetStage()->Instantiate<T>(data.position, Quat::Identity());
				auto objTrans = object->GetComponent<Transform>();
				objTrans->Rotate(data.rotation);

				AddObject(object);
			}
		}

	};

}
