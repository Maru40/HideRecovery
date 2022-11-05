
/*!
@file PatrolCoordinator.cpp
@brief PatrolCoordinator�̃N���X����
�S���F�ێR�T��
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
					//�������N�G�X�g������Ȃ�A����������B
					ObserveTransitionButtle();

					return IsEnd();
				}

				void HidePlacePatrol::OnExit() {

				}

				void HidePlacePatrol::ObserveTransitionButtle() {
					constexpr float ButtleRange = 10.0f;	//�o�g���̋���(�����I�Ƀ����o)
					auto tupleSpace = GetTupleSpace();

					for (auto member : GetMembers()) {
						//�����o�[��null�Ȃ珈�������Ȃ��B
						if (member.expired()) {	
							continue;
						}

						//���łɃo�g���X�e�[�g�Ȃ�J�ڂ��Ȃ��B
						auto stator = member.lock()->GetGameObject()->GetComponent<AIPlayerStator>(false);
						if (stator && stator->IsCurrentState(AIPlayerStator::StateType::Buttle)) {
							continue;
						}

						//�����o�[���o�^�����҂̂ݎ擾����B
						auto isRequester = [&](const std::shared_ptr<Tuple::ButtleTarget>& tuple) {
							return tuple->GetRequester() == member.lock()->GetGameObject();
						};
						auto buttles = tupleSpace->Takes<Tuple::ButtleTarget>(isRequester);

						if (buttles.empty()) {	//��Ȃ珈�����΂��B
							continue;
						}

						//�]���l�����ɏ����\�[�g
						auto sortFunc = [](const std::shared_ptr<Tuple::ButtleTarget>& left, const std::shared_ptr<Tuple::ButtleTarget>& right) {
							return left->GetValue() < right->GetValue();
						};
						std::sort(buttles.begin(), buttles.end(), sortFunc);	//�\�[�g

						auto buttle = buttles[0];	//��ԕ]���̍����l���Q��

						//�����o�[��

						if (buttle->GetValue() < ButtleRange) {	//�o�g���������Ȃ��Ȃ�
							auto targetManager = member.lock()->GetGameObject()->GetComponent<TargetManager>(false);
							if (targetManager && stator) {
								auto memberTupleSpace = member.lock()->GetTupleSpace();

								//memberTupleSpace->Write<Tuple::ButtleTransition>();

								targetManager->SetTarget(buttle->GetTarget());
								stator->ChangeState(AIPlayerStator::StateType::Buttle);

								//�o�g�������o�[�ɃA�T�C��������B
								

							}
						}
					}
				}

				void HidePlacePatrol::FindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple) {
					//�{����I_FactionMember�������o�[�ɓo�^���ꂽ���ɒʒm���󂯎���悤�ɂ������B
					//Notify�̍폜�����肵�čs����悤�ɂȂ�܂ŕۗ��B

					auto tupleSpace = GetTupleSpace();

					for (auto& member : GetMembers()) {
						constexpr float RayHitValue = 2.0f;	//��Q���̕��A�]���l�������邽�߂̐ݒ�B
						constexpr float TransitionTargetRange = 3.0f;	//��苗���ȏ�Ȃ�^�[�Q�b�g�ʒm�����Ȃ�����

						//�^�[�Q�b�g�Ƃ̋����𑪒�B
						auto target = tuple->GetTarget();
						auto memberObject = member.lock()->GetGameObject();
						auto toTargetVec = maru::Utility::CalcuToTargetVec(memberObject, target);

						float hopeValue = toTargetVec.length();	//���Ғl

						tupleSpace->Write<Tuple::ButtleTarget>(memberObject, target, hopeValue);	//�^�[�Q�b�g��_�����Ƃ����N�G�X�g
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
						//nullptr�܂��́A���ł�Open�Ȃ�ΏۂɂȂ�Ȃ��B
						if (hidePlace == nullptr || hidePlace->IsOpen()) {
							continue;
						}

						//���̃����o�[���^�[�Q�b�g�ɂ��Ă��Ȃ�������
						if (!IsSomeMemberTarget(hidePlace->GetGameObject())) {
							result.push_back(hidePlace->GetGameObject());	//���̃^�[�Q�b�g��Ԃ��B
						}
					}

					return result;
				}

				std::shared_ptr<GameObject> HidePlacePatrol::SearchTarget(const std::shared_ptr<I_FactionMember>& member) {
					//�T�����̂��Ȃ��Ȃ�A�������I���B
					auto hidePlaces = ShareClassesManager::GetInstance()->GetCastShareClasses<HidePlace>();
					if (hidePlaces.size() == 0) {
						return nullptr;
					}

					//���̃����o�[���^�[�Q�b�g�ɂ��Ă��Ȃ��҂݂̂ɍi��B
					auto otherTargets = SearchOtherTargets(member, hidePlaces);

					//�^�[�Q�b�g��₪0�Ȃ�Anullptr��Ԃ�(�^�[�Q�b�g�����݂��Ȃ��B)
					if (otherTargets.empty()) {
						return nullptr;
					}

					//�^�[�Q�b�g���̒������ԋ߂��^�[�Q�b�g����O�ɗ���悤�Ƀ\�[�g����B
					auto sortFunction = [&](const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right) {
						constexpr float RayCost = 2.0f;
						auto objects = member->GetSelfObject()->GetStage()->GetGameObjectVec();
						auto memberPosition = member->GetSelfObject()->GetComponent<Transform>()->GetPosition();

						//�����̋���
						auto leftTrans = left->GetComponent<Transform>();
						auto toLeftRange = (leftTrans->GetPosition() - memberPosition).length();
						if (maru::UtilityObstacle::IsRayObstacle(memberPosition, leftTrans->GetPosition(), objects)) {
							toLeftRange *= RayCost;		//��Q��������Ȃ炻�̕��R�X�g���オ��B
						}
						
						//�E���̋���
						auto rightTrans = right->GetComponent<Transform>();
						auto toRightRange = (rightTrans->GetPosition() - memberPosition).length();
						if (maru::UtilityObstacle::IsRayObstacle(memberPosition, rightTrans->GetPosition(), objects)) {
							toRightRange *= RayCost;	//��Q��������Ȃ炻�̕��R�X�g���オ��B
						}

						return toLeftRange < toRightRange;
					};
					std::sort(otherTargets.begin(), otherTargets.end(), sortFunction);	//�\�[�g����

					return otherTargets[0];
				}

				bool HidePlacePatrol::IsEnd() const {
					auto hidePlaces = ShareClassesManager::GetInstance()->GetShareClasses<HidePlace>();
					return hidePlaces.size() == 0;	//�B���ꏊ��0�Ȃ珈�����I��
				}

			}

		}
	}
}