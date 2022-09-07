/*!
@file StageMapCSV.h
@brief StageMapCSV
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "LoadData.h"

#include "Maruyama/StageObject/StageObjectBase.h"

#include "Maruyama/StageObject/OriginalMeshStageObject.h"
#include "Maruyama/StageObject/OriginalMeshToonObject.h"
#include "Maruyama/Utility/Object/CollisionObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	位置、大きさ、回転
	//--------------------------------------------------------------------------------------
	struct PositionScaleRotation {
		Vec3 position;	//位置
		Vec3 scale;		//大きさ
		Vec3 rotation;	//回転

		/// <summary>
		/// コンストラクタ
		/// </summary>
		PositionScaleRotation();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="scale">大きさ</param>
		/// <param name="rotation">回転</param>
		PositionScaleRotation(const Vec3& position, const Vec3& scale, const Vec3& rotation);
	};

	//--------------------------------------------------------------------------------------
	///	ステージマップCSVファイルの読み込み
	//--------------------------------------------------------------------------------------
	class StageMapCSV : public GameObject
	{
	public:
		static map<wstring, int> sm_admissionCount; //入場した回数

	private:

		//--------------------------------------------------------------------------------------
		///	CSVファイルの列インデックス
		//--------------------------------------------------------------------------------------
		enum CsvIndex
		{
			name,
			posX, posY, posZ,
			scaleX, scaleY, scaleZ,
			rotX, rotY, rotZ,
			texture,
		};

		wstring m_folderName;				//フォルダー名
		wstring m_fileName;					//ファイル名
		map<wstring, CsvFile> m_csvFiles;	//CSVファイル名

		map<wstring ,std::vector<ex_weak_ptr<StageObjectBase>>> m_stageObjs;	//生成したステージオブジェクト配列

		/// <summary>
		/// オブジェクトを生成する
		/// </summary>
		/// <param name="name">オブジェクトの名前</param>
		/// <param name="scale">大きさ</param>
		/// <param name="rotation">回転</param>
		/// <param name="position">位置</param>
		/// <param name="texture">画像</param>
		/// <returns>生成したオブジェクト</returns>
		template<class T>
		std::shared_ptr<T> Instantiate(const wstring& name, const Vec3& scale, const Vec3& rotation, const Vec3& position, const wstring& texture) {
			auto object = GetStage()->AddGameObject<T>(name, texture);
			auto objTrans = object->GetComponent<Transform>();
			auto newScale = scale;
			auto newPosition = position;

			objTrans->SetScale(scale);
			objTrans->SetRotation(rotation);
			objTrans->SetPosition(position);

			return object;
		}

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		StageMapCSV(const std::shared_ptr<Stage>& stage,
			const std::wstring& folderName,
			const std::wstring& fileName
		) :
			GameObject(stage),
			m_folderName(folderName),
			m_fileName(fileName)
		{}

		void OnCreate() override;
		void OnUpdate() override;
 
		/// <summary>
		/// CSVファイルのロード
		/// </summary>
		void LoadCSV();

		/// <summary>
		/// CSVファイルのロード
		/// </summary>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>読み込んだCSVファイル</returns>
		static map<wstring, CsvFile> LoadCSVFileMap(const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// マップのリセット
		/// </summary>
		void ResetMap();

		/// <summary>
		/// マップの切り替え
		/// </summary>
		/// <param name="fileName">切り替えたいマップの情報の入ったcsvファイル名</param>
		/// <returns>マップを新規作成する必要がある場合はtureを返す。</returns>
		bool ChangeMap(const wstring& fileName);

		/// <summary>
		/// ステージのアクティブの切り替え
		/// </summary>
		/// <param name="isActive">アクティブかどうか</param>
		void StageObjectActiveChange(const bool isActive);

		/// <summary>
		/// ステージオブジェクトの追加
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <param name="object">追加するオブジェクト</param>
		void AddStageObject(const wstring& fileName,const std::shared_ptr<StageObjectBase>& object) {
			ex_weak_ptr<StageObjectBase> obj = object;
			m_stageObjs[fileName].push_back(obj);
		}

		/// <summary>
		/// 指定した列のデータをwsring型で取得する
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <param name="typeIndex">列インデックス</param>
		/// <returns>指定した列データのwstring型</returns>
		static wstring GetWstringData(const wstring& objName, const wstring& folderName, const wstring& fileName, const int& typeIndex);

		/// <summary>
		/// 指定したオブジェクト名の位置を配列で受け取る
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>指定したオブジェクト名の位置配列</returns>
		static vector<Vec3> GetPositions(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// 指定したオブジェクト名の回転を配列で受け取る
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>指定したオブジェクト名の回転配列</returns>
		static vector<Vec3> GetRotations(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// 指定したオブジェクト名の大きさを配列で受け取る
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>指定したオブジェクト名の大きさ配列</returns>
		static vector<Vec3> GetScales(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// 指定したオブジェクト名で一番早く見つかった「位置、大きさ、回転」を受け取る
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>指定したオブジェクト名で一番早く見つかった「位置、大きさ、回転」</returns>
		static PositionScaleRotation GetPositionScaleRotation(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// 指定したオブジェクト名「位置、大きさ、回転」を配列で受け取る
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="folderName">フォルダー名</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>指定したオブジェクト名「位置、大きさ、回転」の配列</returns>
		static vector<PositionScaleRotation> GetPositionScaleRotations(const wstring& objName, const wstring& folderName, const wstring& fileName);

		/// <summary>
		/// 指定したオブジェクト名のテクスチャ名を配列で取得
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <returns>テクスチャ名の配列</returns>
		vector<wstring> GetTextures(const wstring& objName);

		/// <summary>
		/// マップ上にオブジェクトを生成
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <param name="offset">生成するオフセット位置</param>
		/// <param name="isAddObjectList">このクラスが持つかどうか</param>
		/// <returns>生成したオブジェクトの配列</returns>
		template<class T>
		vector<std::shared_ptr<T>> CreateObject(const wstring& objName, const Vec3& offset = Vec3(0.0f), const bool isAddObjectList = true)
		{
			vector<wstring> lineVec;
			m_csvFiles[m_fileName].GetSelect(lineVec, 0, objName);
			vector<std::shared_ptr<T>> resultObjects;

			for (auto& line : lineVec)
			{
				vector<wstring> tokens;
				Util::WStrToTokenVector(tokens, line, L',');

				Vec3 scale(     //スケールの取得
					static_cast<float>(_wtof(tokens[CsvIndex::scaleX].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::scaleY].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::scaleZ].c_str()))
				);

				Vec3 rotation(  //ローテーション取得
					XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotX].c_str()))),
					XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotY].c_str()))),
					XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotZ].c_str())))
				);

				Vec3 position(  //ポジション取得
					static_cast<float>(_wtof(tokens[CsvIndex::posX].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::posY].c_str())),
					static_cast<float>(_wtof(tokens[CsvIndex::posZ].c_str()))
				);

				wstring texture = tokens[CsvIndex::texture].c_str();  //テクスチャの取得
				
				auto stageObject = Instantiate<T>(objName, scale, rotation, position + offset, texture);
				resultObjects.push_back(stageObject);

				if (isAddObjectList) {
					ex_weak_ptr<StageObjectBase> stageObj = stageObject;
					m_stageObjs[m_fileName].push_back(stageObj);  //オブジェクトを自分のリストに追加	
				}

				if (auto originalMesh = dynamic_pointer_cast<I_OriginalMeshObject>(stageObject)) {
					bool isCollision = texture == L"in";
					originalMesh->SettingCollisionObject(isCollision);
				}
			}

			return resultObjects;
		}

		/// <summary>
		/// マップ上にオブジェクトの複数生成
		/// </summary>
		/// <param name="names">オブジェクト名の配列</param>
		/// <param name="offset">生成するオフセット位置</param>
		/// <param name="isAddObjectList">このクラスが持つかどうか</param>
		/// <returns>生成したオブジェクトの配列</returns>
		template<class T>
		std::vector<std::shared_ptr<T>> CreateObjects(const vector<wstring>& names, const Vec3& offset = Vec3(0.0f), const bool isAddObjectList = true) {
			std::vector<std::shared_ptr<T>> resultObjects;
			for (const auto& name : names) {
				auto objects = CreateObject<T>(name, offset, isAddObjectList);
				for (const auto& object : objects) {
					resultObjects.push_back(object);
				}
			}
			return resultObjects;
		}

		/// <summary>
		/// 追加した数の取得
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <returns>追加した数</returns>
		static int GetAdmissionCount(const wstring& fileName) {
			return sm_admissionCount.at(fileName);
		}

	};

}
