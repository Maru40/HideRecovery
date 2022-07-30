/*!
@file DebugObject.h
@brief DebugObject
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	デバッグオブジェクト
	//--------------------------------------------------------------------------------------
	class DebugObject : public GameObject
	{
	public:
		static wstringstream sm_wss;  //表示する文字列
		static bool sm_isResetDelta;  //毎フレームテクストをリセットするかどうかを判断

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		DebugObject(const std::shared_ptr<Stage>& stage):
			GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnUpdate2() override;

		/// <summary>
		/// キューブの生成
		/// </summary>
		/// <param name="stage">生成するステージ</param>
		/// <param name="position">生成位置</param>
		/// <returns>生成したキューブ</returns>
		static std::shared_ptr<GameObject> CreateCube(const std::shared_ptr<Stage>& stage, const Vec3& position) {
			auto object = stage->Instantiate<GameObject>(position, Quat(0.0f));
			object->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");
			return object;
		}

		/// <summary>
		/// 文字列の追加
		/// </summary>
		/// <param name="string">追加する文字列</param>
		/// <param name="isEndl">改行するかどうか</param>
		static void AddString(const wstring& string, const bool isEndl = true) {
			sm_wss << string;
			if (isEndl) {
				sm_wss << endl;
			}
		}

		/// <summary>
		/// Vec3の出力
		/// </summary>
		/// <param name="vec">出力するVec3</param>
		static void AddVector(const Vec3& vec) {
			sm_wss << L"x: " << vec.x << L", "
				<< L"y: " << vec.y << L", "
				<< L"z: " << vec.z << endl;
		}

		/// <summary>
		/// 数字の出力
		/// </summary>
		/// <param name="value">出力する数字</param>
		/// <param name="isEndl">改行するかどうか</param>
		template<class T>
		static void AddValue(const T& value , const bool isEndl = true) {
			sm_wss << to_wstring(value);
			if (isEndl) {
				sm_wss << endl;
			}
		}

	};

}