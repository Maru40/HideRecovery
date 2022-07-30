/*!
@file StageMapCSV.cpp
@brief StageMapCSV
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "LoadData.h"
#include "StageMapCSV.h"

namespace basecross {

	map<wstring, int> StageMapCSV::sm_admissionCount = map<wstring, int>();

	//--------------------------------------------------------------------------------------
	///	位置、大きさ、回転
	//--------------------------------------------------------------------------------------

	PositionScaleRotation::PositionScaleRotation()
		:PositionScaleRotation(Vec3(0.0f), Vec3(0.0f), Vec3(0.0f))
	{}

	PositionScaleRotation::PositionScaleRotation(const Vec3& position, const Vec3& scale, const Vec3& rotation)
		:position(position), scale(scale), rotation(rotation)
	{}

	//--------------------------------------------------------------------------------------
	///	ステージマップCSVファイルの読み込み本体
	//--------------------------------------------------------------------------------------

	void StageMapCSV::OnCreate()
	{

		LoadCSV();
		
		//ステージに入場した回数をカウント
		if (StageMapCSV::sm_admissionCount.find(m_fileName) == StageMapCSV::sm_admissionCount.end()) {
			StageMapCSV::sm_admissionCount[m_fileName] = 0;
		}
		else {
			StageMapCSV::sm_admissionCount[m_fileName]++;
		}
	}

	void StageMapCSV::OnUpdate() {

	}

	void StageMapCSV::LoadCSV()
	{
		auto load = LoadData(m_folderName);
		load.LoadCSV(m_csvFiles[m_fileName],m_fileName);
		m_csvFiles[m_fileName].ReadCsv();
	}

	map<wstring, CsvFile> StageMapCSV::LoadCSVFileMap(const wstring& folderName, const wstring& fileName) {
		map<wstring, CsvFile> resultCSVFileMap;

		auto load = LoadData(folderName);
		load.LoadCSV(resultCSVFileMap[fileName], fileName);
		resultCSVFileMap[fileName].ReadCsv();

		return resultCSVFileMap;
	}

	void StageMapCSV::ResetMap()
	{
		
	}

	bool StageMapCSV::ChangeMap(const wstring& fileName) {
		StageObjectActiveChange(false); //現在のステージを当たり判定と見た目をfalse

		m_fileName = fileName;

		//ロード用のCSVが存在しなかったら
		if (m_csvFiles.find(fileName) == m_csvFiles.end()) {
			LoadCSV();  //新しくロード系にセット
			return true;
		}
		else {
			StageObjectActiveChange(true);  //見た目とコリジョンを回復。
			return false;
		}
	}

	void StageMapCSV::StageObjectActiveChange(const bool isActive) {
		for (auto& obj : m_stageObjs[m_fileName]) {
			if (obj) {
				obj->SetUpdateActive(isActive);
				obj->SetDrawActive(isActive);
			}
		}
	}

	wstring StageMapCSV::GetWstringData(const wstring& objName, const wstring& folderName, const wstring& fileName, const int& typeIndex) {
		wstring result = {};

		vector<wstring> lineVec;
		auto csvFileMap = LoadCSVFileMap(folderName, fileName);
		csvFileMap[fileName].GetSelect(lineVec, 0, objName);

		for (auto& line : lineVec) {
			vector<wstring> tokens;
			Util::WStrToTokenVector(tokens, line, L',');

			result = tokens[typeIndex];
		}

		return result;
	}

	vector<Vec3> StageMapCSV::GetPositions(const wstring& objName, const wstring& folderName, const wstring& fileName) {
		vector<Vec3> rePositions;

		vector<wstring> lineVec;
		auto csvFileMap = LoadCSVFileMap(folderName, fileName);
		csvFileMap[fileName].GetSelect(lineVec, 0, objName);

		for (auto& line : lineVec)
		{
			vector<wstring> tokens;
			Util::WStrToTokenVector(tokens, line, L',');

			Vec3 position(  //ポジション取得
				static_cast<float>(_wtof(tokens[CsvIndex::posX].c_str())),
				static_cast<float>(_wtof(tokens[CsvIndex::posY].c_str())),
				static_cast<float>(_wtof(tokens[CsvIndex::posZ].c_str()))
			);

			rePositions.push_back(position);
		}

		return rePositions;

	}

	vector<Vec3> StageMapCSV::GetRotations(const wstring& objName, const wstring& folderName, const wstring& fileName) {
		vector<Vec3> resultRotations;

		vector<wstring> lineVec;
		auto csvFileMap = LoadCSVFileMap(folderName, fileName);
		csvFileMap[fileName].GetSelect(lineVec, 0, objName);

		for (auto& line : lineVec)
		{
			vector<wstring> tokens;
			Util::WStrToTokenVector(tokens, line, L',');

			Vec3 rotation(  //ポジション取得
				XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotX].c_str()))),
				XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotY].c_str()))),
				XMConvertToRadians(static_cast<float>(_wtof(tokens[CsvIndex::rotZ].c_str())))
			);

			resultRotations.push_back(rotation);
		}

		return resultRotations;
	}

	vector<Vec3> StageMapCSV::GetScales(const wstring& objName, const wstring& folderName, const wstring& fileName) {
		vector<Vec3> resultScales;

		vector<wstring> lineVec;
		auto csvFileMap = LoadCSVFileMap(folderName, fileName);
		csvFileMap[fileName].GetSelect(lineVec, 0, objName);

		for (auto& line : lineVec)
		{
			vector<wstring> tokens;
			Util::WStrToTokenVector(tokens, line, L',');

			Vec3 scale(  //ポジション取得
				static_cast<float>(_wtof(tokens[CsvIndex::scaleX].c_str())),
				static_cast<float>(_wtof(tokens[CsvIndex::scaleY].c_str())),
				static_cast<float>(_wtof(tokens[CsvIndex::scaleZ].c_str()))
			);

			resultScales.push_back(scale);
		}

		return resultScales;
	}

	PositionScaleRotation StageMapCSV::GetPositionScaleRotation(const wstring& objName, const wstring& folderName, const wstring& fileName) {
		PositionScaleRotation result;

		auto positions = GetPositions(objName, folderName, fileName);
		auto scales = GetScales(objName, folderName, fileName);
		auto rotations = GetRotations(objName, folderName, fileName);

		//どれかがemptyなら処理をしない。
		if (positions.empty() || scales.empty() || rotations.empty()) {
			return result;
		}

		result = PositionScaleRotation(positions[0], scales[0], rotations[0]);
		return result;
	}

	vector<PositionScaleRotation> StageMapCSV::GetPositionScaleRotations(const wstring& objName, const wstring& folderName, const wstring& fileName) {
		vector<PositionScaleRotation> result;

		auto positions = GetPositions(objName, folderName, fileName);
		auto scales = GetScales(objName, folderName, fileName);
		auto rotations = GetRotations(objName, folderName, fileName);

		int index = 0;
		for (const auto& position : positions) {
			result.push_back(PositionScaleRotation(position, scales[index], rotations[index]));
			index++;
		}

		return result;
	}

	vector<wstring> StageMapCSV::GetTextures(const wstring& objName) {

		vector<wstring> reTextrues;

		vector<wstring> lineVec;
		m_csvFiles[m_fileName].GetSelect(lineVec, 0, objName);

		for (auto& line : lineVec)
		{
			vector<wstring> tokens;
			Util::WStrToTokenVector(tokens, line, L',');

			wstring texture = tokens[CsvIndex::texture].c_str();  //テクスチャの取得

			reTextrues.push_back(texture);
		}

		return reTextrues;
	}
}