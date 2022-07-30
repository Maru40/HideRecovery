/*!
@file TargetCameraManager.h
@brief TargetCameraManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Task_ToTargetMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class MulchTaskList;

	//--------------------------------------------------------------------------------------
	///	ターゲットカメラ管理クラス
	//--------------------------------------------------------------------------------------
	class TargetCameraManager : public Component
	{
		/// <summary>
		/// タスクタイプ
		/// </summary>
		enum class TaskEnum
		{
			AtMove,   //視点移動
			EyeMove,  //位置移動
		};

		std::unique_ptr<MulchTaskList<TaskEnum>> m_mulchTaskList = nullptr; //マルチタスククラス

		std::shared_ptr<Task_ToTargetMove::Parametor> m_eyeParamPtr;        //タスクの移動パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		TargetCameraManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// タスクの定義
		/// </summary>
		void DefineTask();

		/// <summary>
		/// タスクの選択
		/// </summary>
		void SelectTask();

	public:
		/// <summary>
		/// タスクの開始
		/// </summary>
		void StartTask();

	};

}

//endbasecross