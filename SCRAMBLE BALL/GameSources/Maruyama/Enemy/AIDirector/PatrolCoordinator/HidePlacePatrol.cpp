
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

#include "Watanabe/Component/PlayerStatus.h"

#include "Maruyama/Enemy/AIDirector/Calculater/HidePlacePatrolCalculater.h"

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
					tuple->Notify<Tuple::FindTarget>(
						GetThis<HidePlacePatrol>(), 
						[&](const std::shared_ptr<Tuple::FindTarget>& tuple) { FindTarget(tuple); }
					);
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

				//�o�g���J�ڂ̃^�v�����Ă΂ꂽ��
				void HidePlacePatrol::ObserveTransitionButtle() {
					auto tupleSpace = GetTupleSpace();

					for (auto member : GetMembers()) {
						//�����o�[��null�Ȃ珈�������Ȃ��B
						if (member.expired()) {
							continue;
						}

						//�����o�[���o�^�����҂̂ݎ擾����B
						auto isRequester = [&](const std::shared_ptr<Tuple::ButtleTarget>& tuple) {
							return tuple->GetRequester() == member.lock();
						};
						auto buttleTuples = tupleSpace->Takes<Tuple::ButtleTarget>(isRequester);

						if (buttleTuples.empty()) {	//��Ȃ珈�����΂��B
							continue;
						}

						//�]���l�����ɏ����\�[�g
						auto sortFunc = [](const std::shared_ptr<Tuple::ButtleTarget>& left, const std::shared_ptr<Tuple::ButtleTarget>& right) {
							return left->GetValue() < right->GetValue();
						};
						std::sort(buttleTuples.begin(), buttleTuples.end(), sortFunc);	//�\�[�g

						auto buttleTuple = buttleTuples[0];	//��ԕ]���̍����l���Q��

						//�����o�[�̃^�v���X�y�[�X�Ƀo�g����ԂɑJ�ڂ��邱�Ƃ�`����B
						auto memberTupleSpace = member.lock()->GetTupleSpace();
						memberTupleSpace->Write<Tuple::ButtleTransition>(
							GetThis<HidePlacePatrol>(),
							buttleTuple->GetTarget(),
							buttleTuple->GetValue()
						);
					}

					tupleSpace->Takes<Tuple::ButtleTarget>();
				}

				//�^�[�Q�b�g�����^�v�����Ă΂ꂽ��B
				void HidePlacePatrol::FindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple) {
					auto tupleSpace = GetTupleSpace();
					auto takeTuple = tupleSpace->Take(tuple);
					auto target = tuple->GetTarget();

					for (auto& member : GetMembers()) {
						//�^�[�Q�b�g�Ƃ̋����𑪒�B
						auto memberObject = member.lock()->GetGameObject();

						float hopeValue = Calculater::HidePlacePatorl::CalculateEvalutionValue(memberObject, target);	//���Ғl

						tupleSpace->Write<Tuple::ButtleTarget>(member.lock(), target, hopeValue);	//�^�[�Q�b�g��_�����Ƃ����N�G�X�g
					}
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

				std::shared_ptr<GameObject> HidePlacePatrol::SearchSomeTarget() const {
					for (auto& member : GetMembers()) {
						//�^�[�Q�b�g�������Ă��Ȃ��Ȃ�Continue
						auto target = member.lock()->GetTarget();
						if (!target) {
							continue;
						}

						//HidePlace�Ȃ�A����Ɠ����^�[�Q�b�g�ɂ��邽�߁A�Ԃ��B
						auto hidePlace = target->GetComponent<HidePlace>(false);
						if (hidePlace) {
							return target;
						}
					}

					return nullptr;	//���݂��Ȃ��Ȃ�nullptr
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
						return SearchSomeTarget();
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