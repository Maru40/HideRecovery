
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrol.h"

#include "../PatrolCoordinator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "../FactionCoordinator.h"

#include "Maruyama/StageObject/HidePlace.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/UtilityObstacle.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"
#include "Maruyama/Utility/Component/TargetManager.h"

namespace basecross {

	namespace Enemy {

		namespace AICoordinator {

			namespace Patrol {


				HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<FactionCoordinator>& owner) :
					HidePlacePatrol(owner, std::vector<std::weak_ptr<EnemyBase>>())
				{}

				HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<FactionCoordinator>& owner, const std::vector<std::weak_ptr<EnemyBase>>& members) :
					PatrolCoordinator(owner, members),
					m_param(Parametor())
				{
					
				}

				void HidePlacePatrol::OnCreate() {
					PatrolCoordinator::OnCreate();

					auto tuple = GetTupleSpace();
					tuple->Notify<Tuple::FindTarget>(GetThis<HidePlacePatrol>(), [&](const std::shared_ptr<Tuple::FindTarget>& tuple) { FindTarget(tuple); });
				}

				void HidePlacePatrol::OnStart() {
					PatrolCoordinator::OnStart();
				}

				bool HidePlacePatrol::OnUpdate() {
					//検索リクエストがあるなら、処理をする。
					ObserveTransitionButtle();

					return IsEnd();
				}

				void HidePlacePatrol::OnExit() {

				}

				void HidePlacePatrol::ObserveTransitionButtle() {
					constexpr float ButtleRange = 10.0f;	//バトルの距離(将来的にメンバ)
					auto tupleSpace = GetTupleSpace();

					for (auto member : GetMembers()) {
						//メンバーがnullなら処理をしない。
						if (member.expired()) {	
							continue;
						}

						//すでにバトルステートなら遷移しない。
						auto stator = member.lock()->GetGameObject()->GetComponent<AIPlayerStator>(false);
						if (stator && stator->IsCurrentState(AIPlayerStator::StateType::Buttle)) {
							continue;
						}

						//メンバーが登録した者のみ取得する。
						auto isRequester = [&](const std::shared_ptr<Tuple::ButtleTarget>& tuple) {
							return tuple->GetRequester() == member.lock()->GetGameObject();
						};
						auto buttles = tupleSpace->Takes<Tuple::ButtleTarget>(isRequester);

						if (buttles.empty()) {	//空なら処理を飛ばす。
							continue;
						}

						//評価値を元に昇順ソート
						auto sortFunc = [](const std::shared_ptr<Tuple::ButtleTarget>& left, const std::shared_ptr<Tuple::ButtleTarget>& right) {
							return left->GetValue() < right->GetValue();
						};
						std::sort(buttles.begin(), buttles.end(), sortFunc);	//ソート

						auto buttle = buttles[0];	//一番評価の高い値を参照

						//メンバーの

						if (buttle->GetValue() < ButtleRange) {	//バトル距離いないなら
							auto targetManager = member.lock()->GetGameObject()->GetComponent<TargetManager>(false);
							if (targetManager && stator) {
								auto memberTupleSpace = member.lock()->GetTupleSpace();

								//memberTupleSpace->Write<Tuple::ButtleTransition>();

								targetManager->SetTarget(buttle->GetTarget());
								stator->ChangeState(AIPlayerStator::StateType::Buttle);

								//バトルメンバーにアサインさせる。
								

							}
						}
					}
				}

				void HidePlacePatrol::FindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple) {
					//本当はI_FactionMemberがメンバーに登録された時に通知を受け取れるようにしたい。
					//Notifyの削除が安定して行えるようになるまで保留。

					auto tupleSpace = GetTupleSpace();

					for (auto& member : GetMembers()) {
						constexpr float RayHitValue = 2.0f;	//障害物の分、評価値を下げるための設定。
						constexpr float TransitionTargetRange = 3.0f;	//一定距離以上ならターゲット通知をしない処理

						//ターゲットとの距離を測定。
						auto target = tuple->GetTarget();
						auto memberObject = member.lock()->GetGameObject();
						auto toTargetVec = maru::Utility::CalcuToTargetVec(memberObject, target);

						float hopeValue = toTargetVec.length();	//期待値

						tupleSpace->Write<Tuple::ButtleTarget>(memberObject, target, hopeValue);	//ターゲットを狙うことをリクエスト
					}

					auto takeTuple = tupleSpace->Take(tuple);
				}

				bool HidePlacePatrol::IsSomeMemberTarget(const std::shared_ptr<GameObject>& target) {
					for (auto& weakMember : GetMembers()) {
						auto member = weakMember.lock();
						if (member->GetTarget() == target) {
							return true;
						}
					}

					return false;
				}

				std::vector<std::shared_ptr<GameObject>> HidePlacePatrol::SearchOtherTargets(
					const std::shared_ptr<I_FactionMember>& member,
					const std::vector<std::weak_ptr<HidePlace>>& hidePlaces
				) {
					std::vector<std::shared_ptr<GameObject>> result;

					for (auto& weakHidePlace : hidePlaces) {
						auto hidePlace = weakHidePlace.lock();
						//nullptrまたは、すでにOpenなら対象にならない。
						if (hidePlace == nullptr || hidePlace->IsOpen()) {
							continue;
						}

						//他のメンバーがターゲットにしていなかったら
						if (!IsSomeMemberTarget(hidePlace->GetGameObject())) {
							result.push_back(hidePlace->GetGameObject());	//そのターゲットを返す。
						}
					}

					return result;
				}

				std::shared_ptr<GameObject> HidePlacePatrol::SearchTarget(const std::shared_ptr<I_FactionMember>& member) {
					//探しものがないなら、処理を終了。
					auto hidePlaces = ShareClassesManager::GetInstance()->GetCastShareClasses<HidePlace>();
					if (hidePlaces.size() == 0) {
						return nullptr;
					}

					//他のメンバーがターゲットにしていない者のみに絞る。
					auto otherTargets = SearchOtherTargets(member, hidePlaces);

					//ターゲット候補が0なら、nullptrを返す(ターゲットが存在しない。)
					if (otherTargets.empty()) {
						return nullptr;
					}

					//ターゲット候補の中から一番近いターゲットが手前に来るようにソートする。
					auto sortFunction = [&](const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right) {
						constexpr float RayCost = 2.0f;
						auto objects = member->GetSelfObject()->GetStage()->GetGameObjectVec();
						auto memberPosition = member->GetSelfObject()->GetComponent<Transform>()->GetPosition();

						//左側の距離
						auto leftTrans = left->GetComponent<Transform>();
						auto toLeftRange = (leftTrans->GetPosition() - memberPosition).length();
						if (maru::UtilityObstacle::IsRayObstacle(memberPosition, leftTrans->GetPosition(), objects)) {
							toLeftRange *= RayCost;		//障害物があるならその分コストが上がる。
						}
						
						//右側の距離
						auto rightTrans = right->GetComponent<Transform>();
						auto toRightRange = (rightTrans->GetPosition() - memberPosition).length();
						if (maru::UtilityObstacle::IsRayObstacle(memberPosition, rightTrans->GetPosition(), objects)) {
							toRightRange *= RayCost;	//障害物があるならその分コストが上がる。
						}

						return toLeftRange < toRightRange;
					};
					std::sort(otherTargets.begin(), otherTargets.end(), sortFunction);	//ソート実装

					return otherTargets[0];
				}

				bool HidePlacePatrol::IsEnd() const {
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					return hidePlaces.size() == 0;	//隠し場所が0なら処理が終了
				}

			}

		}
	}
}