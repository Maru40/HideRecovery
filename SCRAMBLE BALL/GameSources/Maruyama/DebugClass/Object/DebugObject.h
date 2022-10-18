/*!
@file DebugObject.h
@brief DebugObject
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�f�o�b�O�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class DebugObject : public GameObject
	{
	public:
		static wstringstream sm_wss;  //�\�����镶����
		static bool sm_isResetDelta;  //���t���[���e�N�X�g�����Z�b�g���邩�ǂ����𔻒f

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		DebugObject(const std::shared_ptr<Stage>& stage):
			GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnUpdate2() override;

		/// <summary>
		/// �L���[�u�̐���
		/// </summary>
		/// <param name="stage">��������X�e�[�W</param>
		/// <param name="position">�����ʒu</param>
		/// <returns>���������L���[�u</returns>
		static std::shared_ptr<GameObject> CreateCube(const std::shared_ptr<Stage>& stage, const Vec3& position) {
			auto object = stage->Instantiate<GameObject>(position, Quat(0.0f));
			object->AddComponent<PNTStaticDraw>()->SetMeshResource(L"DEFAULT_CUBE");
			return object;
		}

		/// <summary>
		/// ������̒ǉ�
		/// </summary>
		/// <param name="string">�ǉ����镶����</param>
		/// <param name="isEndl">���s���邩�ǂ���</param>
		static void AddString(const wstring& string, const bool isEndl = true) {
			sm_wss << string;
			if (isEndl) {
				sm_wss << endl;
			}
		}

		/// <summary>
		/// Vec3�̏o��
		/// </summary>
		/// <param name="vec">�o�͂���Vec3</param>
		static void AddVector(const Vec3& vec) {
			sm_wss << L"x: " << vec.x << L", "
				<< L"y: " << vec.y << L", "
				<< L"z: " << vec.z << endl;
		}

		/// <summary>
		/// �����̏o��
		/// </summary>
		/// <param name="value">�o�͂��鐔��</param>
		/// <param name="isEndl">���s���邩�ǂ���</param>
		template<class T>
		static void AddValue(const T& value , const bool isEndl = true) {
			sm_wss << to_wstring(value);
			if (isEndl) {
				sm_wss << endl;
			}
		}

	};

}