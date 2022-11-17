/*!
@file UtilityObstacle.h
@brief UtilityObstacle�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Utility.h"

namespace basecross {
	namespace maru
	{
		struct HitData;

		class UtilityObstacle
		{
		public:
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_TAGS;						//�f�t�H���g�̏�Q���^�O
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_AND_FLOOR_TAGS;				//�f�t�H���g�̏�Q���^�O�Ə��^�O
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_AND_FLOOR_AND_ENEMY_TAGS;	//�f�t�H���g�̏�Q���^�O�Ə��^�O�ƃG�l�~�[�^�O
			static const std::vector<std::wstring> DEFAULT_OBSTACLE_AND_PLAYER_AND_HIDEPLACE;	//�f�t�H���g�̏�Q���^�O��player��HidePlace

			/// <summary>
			/// ��Q�����ǂ������f����
			/// </summary>
			/// <param name="object">���f�������I�u�W�F�N�g</param>
			/// <param name="tags">��Q���^�O</param>
			/// <returns>��Q���Ȃ�true</returns>
			static bool IsObstacle(const std::shared_ptr<GameObject>& object, const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS);

			/// <summary>
			/// Ray���΂��ăq�b�g���Ă��邩���f
			/// </summary>
			/// <param name="startPosition">Ray�̊J�n�ʒu</param>
			/// <param name="endPosition">Ray�̏I���ʒu</param>
			/// <param name="object">�Ώۂ̃I�u�W�F�N�g</param>
			/// <returns>�q�b�g������true</returns>
			static bool IsRayHit(
				const Vec3& startPosition,
				const Vec3& endPosition,
				const std::shared_ptr<GameObject>& object
			);

			/// <summary>
			/// Ray���΂��ăq�b�g���Ă��邩���f
			/// </summary>
			/// <param name="startPosition">Ray�̊J�n�ʒu</param>
			/// <param name="endPosition">Ray�̏I���ʒu</param>
			/// <param name="object">�Ώۂ̃I�u�W�F�N�g</param>
			/// <param name="hitData">�q�b�g�f�[�^</param>
			/// <returns>�q�b�g������true</returns>
			static bool IsRayHit(
				const Vec3& startPosition,
				const Vec3& endPosition,
				const std::shared_ptr<GameObject>& object,
				HitData& hitData
			);

			/// <summary>
			/// Ray���΂��ď�Q���Ƀq�b�g���������f
			/// </summary>
			/// <param name="startPosition">Ray�J�n�ʒu</param>
			/// <param name="endPosition">Ray�̏I���ʒu</param>
			/// <param name="objects">���f��������Q���z��</param>
			/// <param name="tags">��Q���Ƃ���^�O</param>
			/// <returns>�q�b�g������true</returns>
			static bool IsRayObstacle(const Vec3& startPosition, const Vec3& endPosition,
				const vector<std::shared_ptr<GameObject>>& objects, const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			/// <summary>
			/// Ray���΂��ď�Q����������
			/// </summary>
			static bool IsRayObstacle(
				const Vec3& startPosition, 
				const Vec3& endPosition,
				const vector<std::weak_ptr<GameObject>>& objects
			);

			/// <summary>
			/// ��ԋ߂���Q����������
			/// </summary>
			/// <param name="startPosition">Ray�̊J�n�ʒu</param>
			/// <param name="endPosition">Ray�̏I���ʒu</param>
			/// <param name="objects">���f��������Q���z��</param>
			/// <param name="tags">��Q���Ƃ���^�O</param>
			/// <returns>�q�b�g������true</returns>
			static std::shared_ptr<GameObject> FindRayHitNearObstacle(
				const Vec3& startPosition, 
				const Vec3& endPosition,
				const vector<std::shared_ptr<GameObject>>& objects,
				const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			/// <summary>
			/// ��ԋ߂���Q����������
			/// </summary>
			/// <param name="startPosition">Ray�̊J�n�ʒu�ɂ������I�u�W�F�N�g</param>
			/// <param name="endPosition">Ray�̏I���ʒu�ɂ������I�u�W�F�N�g</param>
			/// <param name="objects">���f��������Q���z��</param>
			/// <param name="tags">��Q���Ƃ���^�O</param>
			/// <returns>�q�b�g������true</returns>
			static std::shared_ptr<GameObject> FindRayHitNearObstacle(
				const std::shared_ptr<GameObject>& selfObject, 
				const std::shared_ptr<GameObject>& targetObject,
				const vector<std::shared_ptr<GameObject>>& objects,
				const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			//���ݎg�p�֎~
			static bool FindRayHitNearData(
				const Vec3& startPosition,
				const Vec3& endPosition,
				const vector<std::shared_ptr<GameObject>>& objects,
				HitData& hitData,
				const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS
			);

			/// <summary>
			/// �w�肵���^�O�̏�Q�����擾����B
			/// </summary>
			/// <param name="tVec">���f��������Q���z��</param>
			/// <param name="tags">��Q���Ƃ���^�O</param>
			/// <returns>�q�b�g�����I�u�W�F�N�g�̔z��</returns>
			template<class T>
			static vector<T> FindObstacles(const vector<T>& tVec, const std::vector<wstring>& tags = DEFAULT_OBSTACLE_TAGS)
			{
				vector<T> reVec;

				for (const auto& t : tVec)
				{
					for (const auto& tag : tags)
					{
						if (t->FindTag(tag)) {  //�^�O������������
							reVec.push_back(t);
						}
					}
				}

				return reVec;
			}
			
		};
	}
}

//endbasecross