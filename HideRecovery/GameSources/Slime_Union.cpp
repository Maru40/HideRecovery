
/*!
@file Slime_Find.cpp
@brief Slime_Findのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "EnemyStateNodeBase.h"
#include "Slime_Union.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "TargetManager.h"


#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "MaruAction.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Slime_Union::Slime_Union(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
				EnemyStateNodeBase(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{}

			void Slime_Union::OnStart() {
				m_timer->ResetTimer(m_paramPtr->time); 

				//開始位置を記録
				m_startPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();

				//片方にRenderを合わせる。
				TranserChildren();
			}

			bool Slime_Union::OnUpdate() {
				m_timer->UpdateTimer();

				//くっつく処理
				UpdateUnion();

				return m_timer->IsTimeUp();
			}

			void Slime_Union::OnExit() {
				//経過時間が終了していたら、
				if (m_timer->IsTimeUp()) {
					SccessUnion(); //Union成功
				}
				else {	
					MissUnion();   //Union失敗
				}
			}

			void Slime_Union::TranserChildren() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto targetManager = ownerObject->GetComponent<TargetManager>(false);
				if (!targetManager) {
					return;
				}

				maru::Action<void()> removeFunc;  //削除を後で呼ぶためのAnition
				auto selfRender = ownerObject->GetComponent<SlimeRender>(); //自分のRender
				auto targetRender = targetManager->GetTarget()->GetComponent<SlimeRender>();  //相手のRender
				for (auto& child : selfRender->GetChildren()) {
					targetRender->AddChild(child.GetShard()); //Renderに自分のCubeを渡す
					removeFunc.AddFunction([&, child]() { selfRender->RemoveChild(child); }); //自分自身からChildを削除イベントの発行
				}

				removeFunc.Invoke(); //Childの削除
			}

			void Slime_Union::UpdateUnion() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto ownerTrans = ownerObject->GetComponent<Transform>();
				auto targetManager = ownerObject->GetComponent<TargetManager>(false);
				if (!targetManager) {
					return;
				}

				const float StartTime = 0.0f;
				const float EndTime = 1.0f;
				auto halfToTargetVec = targetManager->CalcuToTargetVec() * 0.5f;
				auto endPosition = ownerTrans->GetPosition() + halfToTargetVec;

				auto position = Lerp::CalculateLerp(m_startPosition, endPosition, StartTime, EndTime, m_timer->GetTimeRate(), Lerp::rate::Linear);
				ownerTrans->SetPosition(position);
			}

			void Slime_Union::SccessUnion() {

			}

			void Slime_Union::MissUnion() {

			}

		}
	}
}