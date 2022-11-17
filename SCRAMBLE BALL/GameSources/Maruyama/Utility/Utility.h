
/*!
@file Utility.h
@brief Utility�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru{

		//--------------------------------------------------------------------------------------
		///	�֗��֐��܂Ƃ�
		//--------------------------------------------------------------------------------------
		class Utility
		{
		public:
			static constexpr float FOWARD_ADJUST = -XM_PIDIV2;	//Maya�̃f�[�^�𐳖ʂɌ�������p�x

			/// <summary>
			/// �X�e�[�W�̎擾
			/// </summary>
			/// <returns>�X�e�[�W</returns>
			static std::shared_ptr<Stage> GetStage();

			/// <summary>
			/// �Ώۂ����ʂɂ��邩�ǂ���
			/// </summary>
			/// <param name="target">�Ώۂ̃I�u�W�F�N�g</param>
			/// <param name="self">�������g�̃I�u�W�F�N�g</param>
			/// <returns>���ʂȂ�true</returns>
			static bool IsTargetInFront(const std::shared_ptr<GameObject>& target, const std::shared_ptr<GameObject>& self);

			/// <summary>
			/// �Ώۂ����ʂɂ��邩�ǂ���
			/// </summary>
			/// <param name="targetPosition">�Ώۂ̈ʒu</param>
			/// <param name="selfPosition">�������g�̈ʒu</param>
			/// <param name="selfForward">�������g�̐��ʕ���</param>
			/// <returns></returns>
			static bool IsTargetInFront(const Vec3& targetPosition, const Vec3& selfPosition, const Vec3& selfForward);

			/// <summary>
			/// �^�[�Q�b�g���~�͈͓��ɂ��邩�ǂ���
			/// </summary>
			/// <param name="selfTarget">�������g</param>
			/// <param name="target">�^�[�Q�b�g</param>
			/// <param name="range">�͈�</param>
			/// <returns>�^�[�Q�b�g���͈͓��Ȃ�true</returns>
			static bool IsInTargetCircleRange(const std::shared_ptr<GameObject>& selfTarget, const std::shared_ptr<GameObject>& target, const float& range);

			//��ڂ̈����̕����Ɍ���
			//��y���ɑΉ����ĂȂ����߁Ay��0�œn���Ă��������B
			//�Ή�����R�����������܂��B
			static void SetLookDirect(const Vec3& direct, const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �^�[�Q�b�g�Ɍ������x�N�g�����v�Z���Ă����B
			/// </summary>
			/// <param name="self">�������g�̃|�C���^</param>
			/// <param name="target">�^�[�Q�b�g�̃|�C���^</param>
			/// <returns>�^�[�Q�b�g�Ɍ������x�N�g��</returns>
			static Vec3 CalcuToTargetVec(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target);

			/// <summary>
			/// �����̕����̒��ň�ԋ߂��������擾
			/// </summary>
			/// <param name="stdDirect">��ƂȂ����</param>
			/// <param name="directs"></param>
			/// <returns></returns>
			static Vec3 CalculateNearDirect(const Vec3& stdDirect, const std::vector<Vec3>& directs);
			static Vec3 CalculateNearDirect(const Vec3& stdDreict,
				const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets);

			/// <summary>
			/// �����^�[�Q�b�g�̒��Ŕ͈͕����̃^�[�Q�b�g���擾
			/// </summary>
			/// <param name="stdDirect">��ƂȂ����</param>
			/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
			/// <param name="targets">�����������^�[�Q�b�g�z��</param>
			/// <param name="degree">�͈͂Ƃ���f�O���[�p</param>
			/// <returns>�͈͓��̃^�[�Q�b�g��z��Ŏ擾</returns>
			static std::vector<std::shared_ptr<GameObject>> Utility::FindNearDirectTargetsDegree(const Vec3& stdDirect,
				const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets,
				const float& degree);

			/// <summary>
			/// �����^�[�Q�b�g�̒��Ŕ͈͕����̃^�[�Q�b�g���擾
			/// </summary>
			/// <param name="stdDirect">��ƂȂ����</param>
			/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
			/// <param name="targets">�����������^�[�Q�b�g�z��</param>
			/// <param name="degree">�͈͂Ƃ��郉�W�A���p</param>
			/// <returns>�͈͓��̃^�[�Q�b�g��z��Ŏ擾</returns>
			static std::vector<std::shared_ptr<GameObject>> Utility::FindNearDirectTargetsRadian(const Vec3& stdDirect,
				const std::shared_ptr<GameObject>& selfObject, const std::vector<std::shared_ptr<GameObject>>& targets,
				const float& radian);

			/// <summary>
			/// ��̃x�N�g���̊p�x�����͈͓����ǂ������f
			/// </summary>
			/// <param name="first">�x�N�g��</param>
			/// <param name="second">�x�N�g��</param>
			/// <param name="degree">�f�O���[�p�x��</param>
			/// <returns>�p�x�����͈͓��Ȃ�true</returns>
			static bool IsInAngleDegree(const Vec3& first, const Vec3& second, const float& degree);

			/// <summary>
			/// ��̃x�N�g���̊p�x�����͈͓����ǂ������f
			/// </summary>
			/// <param name="first">�x�N�g��</param>
			/// <param name="second">�x�N�g��</param>
			/// <param name="degree">���W�A���p�x��</param>
			/// <returns>�p�x�����͈͓��Ȃ�true</returns>
			static bool IsInAngleRadian(const Vec3& first, const Vec3& second, const float& radian);

			/// <summary>
			/// ���ʕ����ɍ��킹��offset�ϊ�
			/// </summary>
			/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
			/// <param name="offset">�ϊ��������I�t�Z�b�g</param>
			/// <returns>���ʕ����ɍ��킹�ĕϊ�����offset</returns>
			static Vec3 ConvertForwardOffset(const std::shared_ptr<GameObject>& selfObject, const Vec3& offset);

			/// <summary>
			/// ���ʕ����ɍ��킹��offset�ϊ�
			/// </summary>
			/// <param name="forward">���ʕ���</param>
			/// <param name="offset">�ϊ��������I�t�Z�b�g</param>
			/// <returns>���ʕ����ɍ��킹�ĕϊ�����offset</returns>
			static Vec3 ConvertForwardOffset(const Vec3& forward, const Vec3& offset);

			/// <summary>
			/// �w�肵��offset���ɉ�]������Quat��Ԃ�
			/// </summary>
			/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
			/// <param name="offset">�I�t�Z�b�g�l</param>
			/// <returns>��]������Quat</returns>
			static Quat ConvertRotationOffset(const std::shared_ptr<GameObject>& selfObject, const Vec3& offset);

			/// <summary>
			/// �w�肵������������Quat��Ԃ��B
			/// </summary>
			/// <param name="forward">������������</param>
			/// <param name="nowPosition">���݂̃|�W�V����</param>
			/// <returns>�w�肵������������Quat</returns>
			static Quat ConvertForwardToQuat(const Vec3& forward, const Vec3& nowPosition);

			/// <summary>
			/// ArrowObject�𐶐�����
			/// </summary>
			/// <param name="startPosition">Arrow�̊J�n�ʒu</param>
			/// <param name="endPosition">Arrow�̏I���ʒu</param>
			/// <param name="stage">��������X�e�[�W</param>
			/// <returns>��������ArrowObject</returns>
			static std::shared_ptr<GameObject> CreateArrow(const Vec3& startPosition, const Vec3& endPosition, const std::shared_ptr<Stage>& stage);

			/// <summary>
			/// �X�^�b�N�̃N���A
			/// </summary>
			/// <param name="clearStack">�N���A�������X�^�b�N</param>
			template<class T>
			static void StackClear(std::stack<T>& clearStack) {
				while (!clearStack.empty()) {
					clearStack.pop();
				}
			}

			/// <summary>
			/// �L���[�̃N���A
			/// </summary>
			/// <param name="clearQueue">�N���A�������L���[</param>
			template<class T>
			static void QueueClear(std::queue<T>& clearQueue) {
				while (!clearQueue.empty()) {
					clearQueue.pop();
				}
			}

			/// <summary>
			/// Ray���΂����Ƃ��A��Q������������true��Ԃ��B
			/// </summary>
			/// <param name="startObj">Ray�̊J�n�ʒu�̃I�u�W�F�N�g</param>
			/// <param name="endObj">Ray�̏I���ʒu�̃I�u�W�F�N�g</param>
			/// <returns>��Q������������true</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj, const std::shared_ptr<GameObject>& endObj);

			/// <summary>
			/// Ray���΂����Ƃ��A��Q������������true��Ԃ��B
			/// </summary>
			/// <param name="startPos">Ray�̊J�n�ʒu</param>
			/// <param name="endPos">Ray�̏I���ʒu</param>
			/// <returns>��Q������������true</returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos);

			/// <summary>
			/// �Ώۂ̃I�u�W�F�N�g�܂łɏ�Q�������邩�ǂ���
			/// </summary>
			/// <param name="startObj">Ray�̃X�^�[�g</param>
			/// <param name="endObj">Ray���΂��Ώ�</param>
			/// <param name="excluteObj">��Q���Ƃ��ďȂ��Ώ�</param>
			/// <returns>��Q������������true</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj,
				const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// �Ώۂ̃I�u�W�F�N�g�܂łɏ�Q�������邩�ǂ���
			/// </summary>
			/// <param name="startPos">Ray�̃X�^�[�g</param>
			/// <param name="endPos">Ray���΂��Ώ�</param>
			/// <param name="excluteObjs">��Q���Ƃ��ďȂ��Ώ�</param>
			/// <returns>��Q������������true</returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos,
				const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// �ΏۃI�u�W�F�N�g�܂łɏ�Q�������邩�ǂ���
			/// </summary>
			/// <param name="startObj">Ray�̃X�^�[�g</param>
			/// <param name="endObj">�ΏۃI�u�W�F�N�g</param>
			/// <param name="obstacleObjs">��Q���̑ΏۂɂȂ�I�u�W�F�N�g</param>
			/// <param name="excluteObjs">��Q���̑ΏۂɂȂ�I�u�W�F�N�g�̒��ł�����̃I�u�W�F�N�g��ΏۊO�ɂ������ꍇ�ɋL�q</param>
			/// <returns>��Q������������true</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj, const std::shared_ptr<GameObject>& endObj,
				const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// �ΏۃI�u�W�F�N�g�܂łɏ�Q�������邩�ǂ���
			/// </summary>
			/// <param name="startPos">Ray�̃X�^�[�g</param>
			/// <param name="endPos">�ΏۃI�u�W�F�N�g</param>
			/// <param name="obstacleObjs">��Q���̑ΏۂɂȂ�I�u�W�F�N�g</param>
			/// <param name="excluteObjs">��Q���̑ΏۂɂȂ�I�u�W�F�N�g�̒��ł�����̃I�u�W�F�N�g��ΏۊO�ɂ������ꍇ�ɋL�q</param>
			/// <returns>��Q������������true</returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos,
				const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// template�Ŏw�肳�ꂽ�N���X��`���āA��Q����������֐��B
			/// </summary>
			/// <param name="startObj">Ray�̊J�n�ʒu�̃I�u�W�F�N�g</param>
			/// <param name="endObj">Ray�̏I���ʒu�̃I�u�W�F�N�g</param>
			/// <returns>��Q������������true</returns>
			template<class ExcluteClass>
			static bool IsRayObstacleExclute(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj)
			{
				auto exclutes = maru::Utility::GetComponents<ExcluteClass>();
				return IsRayObstacle(startObj, endObj, exclutes);
			}

			/// <summary>
			/// template�Ŏw�肵���R���|�[�l���g�̂��Ă���I�u�W�F�N�g���w���Q�����肵�ĒT���B
			/// </summary>
			/// <param name="startObj">Ray�̊J�n�ʒu�̃I�u�W�F�N�g</param>
			/// <param name="endObj">Ray�̏I���ʒu�̃I�u�W�F�N�g</param>
			/// <returns>��Q������������true</returns>
			template<class ObstacleClass>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj)
			{
				auto obstacleObjs = maru::Utility::GetComponents<ObstacleClass>();

				return IsRayObstacle(startObj, endObjs, obstacleObjs, vector<shared_ptr<GameObject>>());
			}

			/// <summary>
			/// ����̃I�u�W�F�N�g��Ray��ɍ�������true��Ԃ��B
			/// </summary>
			/// <param name="startObj">Ray�̊J�n</param>
			/// <param name="endObj">Ray�̏I���</param>
			/// <param name="obstacleObj">�Ԃɂ��邩�ǂ������f�������I�u�W�F�N�g</param>
			/// <returns>Ray��ɍ�������true</returns>
			static bool IsRayObstacle(const std::shared_ptr<GameObject>& startObj,
				const std::shared_ptr<GameObject>& endObj,
				const std::shared_ptr<GameObject>& obstacleObj);
			
			/// <summary>
			/// �Ώۂ̏�Q�������Ray�̊Ԃɍ�������true
			/// </summary>
			/// <param name="startPos">Ray�̊J�n�ʒu</param>
			/// <param name="endPos">Ray�̏I���ʒu</param>
			/// <param name="obstacleObj">��Q���̃I�u�W�F�N�g</param>
			/// <returns></returns>
			static bool IsRayObstacle(const Vec3& startPos, const Vec3& endPos,
				const std::shared_ptr<GameObject>& obstacleObj);

			/// <summary>
			/// �Ώۂ̃I�u�W�F�N�g���ΏۊO���ǂ������f����
			/// </summary>
			/// <param name="targetObj">�Ώۂ̃I�u�W�F�N�g</param>
			/// <param name="excluteObjs">�ΏۊO�ƂȂ�z��I�u�W�F�N�g</param>
			/// <returns>�ΏۊO�Ȃ�true</returns>
			static bool IsExclute(const shared_ptr<GameObject>& targetObj ,const vector<shared_ptr<GameObject>>& excluteObjs);

			/// <summary>
			/// �Ώۂ̃|�W�V��������ǂꂾ������Ă��邩��Ԃ�
			/// </summary>
			/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
			/// <param name="targetPosition">�^�[�Q�b�g�̃|�W�V����</param>
			/// <returns>�^�[�Q�b�g����ǂꂾ������Ă��邩</returns>
			static float CalcuSubRange(const std::shared_ptr<GameObject>& selfObject, const Vec3& targetPosition);

			/// <summary>
			/// �J�����ɍ��킹�������ɍ��킹��
			/// </summary>
			/// <param name="input"></param>
			/// <param name="camera"></param>
			/// <returns></returns>
			static Vec3 CalcuCameraVec(const Vec3& input, const std::shared_ptr<Camera>& camera, const std::shared_ptr<GameObject>& selfObject);

			/// <summary>
			/// World���W��Scrren���W�ɕϊ�
			/// </summary>
			/// <param name="view">�r���[�|�C���^</param>
			/// <param name="position">�ϊ��������|�W�V����</param>
			/// <returns></returns>
			static Vec3 ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position);

			/// <summary>
			/// �F��0.0f �` 1.0f�ɕϊ����s��
			/// </summary>
			/// <param name="color">�ݒ肷��F</param>
			/// <returns></returns>
			static Col4 ConvertColorZeroToOne(const Col4& color);

			/// <summary>
			/// �߂��̃I�u�W�F�N�g��T���B
			/// </summary>
			/// <param name="objects">��������I�u�W�F�N�g�z��</param>
			/// <param name="selfPosition">�������g�̃I�u�W�F�N�g</param>
			/// <returns>��ԋ߂��I�u�W�F�N�g�̎擾</returns>
			template<class T>
			static T CalcuNearObject(const vector<T>& objects, const Vec3& selfPosition) {
				float minLength = 9999999.0f;
				T reObject;

				for (auto& object : objects) {
					auto toVec = object->GetComponent<Transform>()->GetPosition() - selfPosition;
					auto newLength = toVec.length();

					if (newLength < minLength) {  //������������
						reObject = object;
						minLength = newLength;
					}
				}

				return reObject;
			}

			//base�Ƃ��ēn�����҂����������҂ƈꏏ�̏ꍇ��true
			template<class T>
			static bool IsExclude(
				const std::shared_ptr<T>& base,
				const std::vector<std::shared_ptr<T>>& excludeVector
			) {
				//�����I�u�W�F�N�g������Ȃ�true
				for (auto& exclude : excludeVector) {
					if (base == exclude) {
						return true;
					}
				}

				return false;
			}

			//�z�񂩂����̔z����폜�������֐��B
			template<class T>
			static std::vector<std::shared_ptr<T>> ExcludeVector(
				const std::vector<std::shared_ptr<T>>& baseVector,
				const std::vector<std::shared_ptr<T>>& excludeVector
			) {
				std::vector<std::shared_ptr<T>> result;

				for (auto& base : baseVector) {
					//���������I�u�W�F�N�g�łȂ�������A�ǉ�����B
					if (!IsExclude<T>(base, excludeVector)) {
						result.push_back(base);
					}
				}

				return result;
			}

			/// <summary>
			/// �Q�[���I�u�W�F�N�gVec����w��̃I�u�W�F�N�g�̂ݎ擾
			/// �ŏ��Ɏ擾�ł������̂����Ԃ��B
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindGameObject() {
				auto stage = GetStage();

				return FindGameObject<T>(stage);
			}

			/// <summary>
			/// �Q�[���I�u�W�F�N�gVec����w��̃I�u�W�F�N�g�̂ݎ擾
			/// �ŏ��Ɏ擾�ł������̂����Ԃ��B
			/// </summary>
			/// <param name="stage">��������X�e�[�W</param>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindGameObject(const std::shared_ptr<Stage>& stage) {
				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = dynamic_pointer_cast<T>(obj);
					if (t) {
						return t;
					}
				}

				return nullptr;
			}

			
			/// <summary>
			/// �Q�[���I�u�W�F�N�gVec����w��̃I�u�W�F�N�g�̂ݎ擾
			/// �x�N�^�[�z��Ƃ��Č��������̑S�ĕԂ��B
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindGameObjects() {
				auto stage = GetStage();

				return FindGameObjects<T>(stage);
			}

			
			/// <summary>
			/// �Q�[���I�u�W�F�N�gVec����w��̃I�u�W�F�N�g�̂ݎ擾
			/// �x�N�^�[�z��Ƃ��Č��������̑S�ĕԂ��B
			/// </summary>
			/// <param name="stage">��������X�e�[�W</param>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindGameObjects(const std::shared_ptr<Stage>& stage) {
				vector<shared_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = dynamic_pointer_cast<T>(obj);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// �w�肵���R���|�[�l���g��������
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindComponent() {
				auto stage = GetStage();

				return FindComponent<T>(stage);
			}

			/// <summary>
			/// �w�肵���R���|�[�l���g��������
			/// </summary>
			/// <param name="stage">��������X�e�[�W</param>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static shared_ptr<T> FindComponent(const std::shared_ptr<Stage>& stage) {
				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						return t;
					}
				}

				return nullptr;
			}

			/// <summary>
			/// �w�肵���R���|�\�l���g�𕡐�������
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindComponents() {
				auto stage = GetStage();
				return FindComponents<T>(stage);
			}

			/// <summary>
			/// �w�肵���R���|�\�l���g�𕡐�������
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static vector<shared_ptr<T>> FindComponents(const std::shared_ptr<Stage>& stage) {
				vector<shared_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// �R���|�[�l���g�̃A�^�b�`���ꂽ�I�u�W�F�N�g���擾����
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
				static vector<shared_ptr<GameObject>> FindComponentObjects() {
				auto stage = GetStage();

				vector<shared_ptr<GameObject>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t->GetGameObject());
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// �w�肵���R���|�[�l���g��S�Č����āAweak_ptr�ɂ��ĕԂ�
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static vector<std::weak_ptr<T>> FindWeakPtrComponents() {
				auto stage = GetStage();

				vector<std::weak_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// �w�肵���R���|�[�l���g��S�Č����āAex_weak_ptr�ɂ��ĕԂ�
			/// </summary>
			template<class T,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
				static vector<ex_weak_ptr<T>> FindExWeakPtrComponents(const std::shared_ptr<Stage>& stage) {

				vector<ex_weak_ptr<T>> returnObjs;

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						returnObjs.push_back(t);
					}
				}

				return returnObjs;
			}

			/// <summary>
			/// �w�肵���C���^�[�t�F�[�X��������
			/// </summary>
			/// <param name="stage">��������X�e�[�W</param>
			template<class T>
			static std::shared_ptr<T> FindInterface(const std::shared_ptr<Stage>& stage) {
				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						return t;
					}
				}

				return nullptr;
			}

			/// <summary>
			/// �n���ꂽ�z���template�Ŏw�肵���I�u�W�F�N�g�^�̃I�u�W�F�N�g��ǉ�����B
			/// </summary>
			/// <param name="addVec">�ǉ��������I�u�W�F�N�g�̔z��</param>
			template<class T,
				enable_if_t<is_base_of_v<GameObject, T>, std::nullptr_t> = nullptr >
			static void AddObjects(vector<shared_ptr<GameObject>>& addVec) {
				auto stage = GetStage();

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = dynamic_pointer_cast<T>(obj);
					if (t) {
						addVec.push_back(t);  //�I�u�W�F�N�g�̒ǉ�
					}
				}
			}

			/// <summary>
			/// �n���ꂽ�z���template�Ŏw�肵���R���|�[�l���g�����I�u�W�F�N�g��ǉ�����B
			/// </summary>
			/// <param name="addVec">�ǉ��������I�u�W�F�N�g�̔z��</param>
			template<class T ,
				enable_if_t<is_base_of_v<Component, T>, std::nullptr_t> = nullptr >
			static void AddComponents(vector<shared_ptr<GameObject>>& addVec) {
				auto stage = GetStage();

				auto objs = stage->GetGameObjectVec();
				for (auto& obj : objs) {
					auto t = obj->GetComponent<T>(false);
					if (t) {
						addVec.push_back(obj);  //�I�u�W�F�N�g�̒ǉ�
					}
				}
			}

			/// <summary>
			/// �x�N�^�[�z�񂩂����̃I�u�W�F�N�g���폜����֐��B
			/// </summary>
			/// <param name="vec">�폜�������f�[�^�̂���z��</param>
			/// <param name="removePtr">�폜�������f�[�^</param>
			template<class T>
			static bool RemoveVec(vector<T>& vec, const T& removePtr) {
				auto iter = vec.begin();
				while (iter != vec.end()) {
					if (*iter == removePtr) {
						iter = vec.erase(iter);
						return true;
						//break;
					}
					iter++;
				}

				return false;
			}

			/// <summary>
			/// �z��ɂ��̃f�[�^���o�^����Ă��邩�𔻒f
			/// </summary>
			/// <param name="tList">�o�^����Ă��邩�m�F�������z��</param>
			/// <param name="value">�o�^����Ă��邩�m�F�������f�[�^</param>
			/// <returns>���łɓo�^����Ă���Ȃ�true</returns>
			template<class ArrayType, class DataType>
			static bool IsRegister(const ArrayType& tList, const DataType& value) {
				auto iter = tList.begin();
				while (iter != tList.end()) {
					if (*iter == value) {
						return true;
					}

					iter++;
				}

				return false;
			}

			/// <summary>
			/// �z�񂩂�A�w�肵���f�[�^�Ɠ����f�[�^���擾����B
			/// </summary>
			/// <returns>�����v�f�����݂��Ă���Ȃ�true</returns>
			template<class ArrayType, class DataType>
			static DataType FindSomeArrayDataPtr(const ArrayType& tArray, const DataType& value) {
				auto iter = tArray.begin();
				while (iter != tArray.end()) {
					if (*iter == value) {
						return *iter;
					}

					iter++;
				}

				return nullptr;
			}

			/// <summary>
			/// �z�񂩂�A�w�肵���f�[�^�Ɠ����f�[�^���擾����B
			/// </summary>
			/// <returns>�����v�f�����݂��Ă���Ȃ�true</returns>
			template<class ArrayType, class DataType>
			static bool FindSomeArrayData(const ArrayType& tArray, const DataType& value, DataType& result) {
				auto iter = tArray.begin();
				while (iter != tArray.end()) {
					if (*iter == value) {
						result = *iter;
						return true;
					}

					iter++;
				}

				return false;
			}

			/// <summary>
			/// ���łɔz��ɓ����Ă��邩���f
			/// </summary>
			/// <returns>���łɓ������̂������Ă���Ȃ�true</returns>
			template<class T>
			static bool IsSameVec(const vector<T>& vec, const T& newPtr) {
				for (auto& t : vec) {
					if (t == newPtr) {
						return true;
					}
				}

				return false;
			}

			/// <summary>
			/// �z���ʂ̌^�ɃL���X�g���ē���Ȃ����ĕԂ��B
			/// </summary>
			/// <param name="vector">����Ȃ��������z��</param>
			template<class ConvertType, class OriginalType>
			static std::vector<std::shared_ptr<ConvertType>> ConvertArrayType(
				const std::vector<std::shared_ptr<OriginalType>>& vector
			) {
				std::vector<std::shared_ptr<ConvertType>> result;

				for (const auto& value : vector) {
					auto convert = dynamic_pointer_cast<ConvertType>(value);
					if (convert) {
						result.push_back(convert);
					}
				}

				return result;
			}

			/// <summary>
			/// shared_ptr�̔z���weak_ptr�z��ɕϊ�����
			/// </summary>
			/// <param name="vector">�ϊ��������z��</param>
			template<class T>
			static std::vector<std::weak_ptr<T>> ConvertArraySharedToWeak(const std::vector<std::shared_ptr<T>>& vector) {
				std::vector<std::weak_ptr<T>> result;

				for (auto& value : vector) {
					result.push_back(value);
				}

				return result;
			}

			/// <summary>
			/// shared_ptr�̔z���weak_ptr�z��ɕϊ�����
			/// </summary>
			/// <param name="vector">�ϊ��������z��</param>
			template<class T>
			static std::vector<ex_weak_ptr<T>> ConvertArraySharedToExweak(const std::vector<std::shared_ptr<T>>& vector) {
				std::vector<ex_weak_ptr<T>> result;

				for (auto& value : vector) {
					result.push_back(value);
				}

				return result;
			}

			/// <summary>
			/// Array�̒�����m��
			/// </summary>
			template<class T, std::size_t SIZE>
			static std::size_t Array_Length(const T(&array)[SIZE])
			{
				return SIZE;
			}
		};
	}

	

}

//endbasecross