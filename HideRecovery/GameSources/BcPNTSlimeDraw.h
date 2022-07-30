/*!
@file BcPNTSlimeDraw.h
@brief BcPNTSlimeDraw
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �X�t�B�A�f�[�^
	//--------------------------------------------------------------------------------------
	struct SphereData {
		Vec3 position; //�ʒu
		float radius;  //���a
		Col4 color;    //�F
	};

	struct SlimeConstants {
		Col4 diffuseColor;
		Col4 emissiveColor;
		Col4 specularColorAndPower;

		Col4 lightDirection[3];
		Col4 lightDiffuseColor[3];
		Col4 lightSpecularColor[3];

		Vec4 eyePosition;

		Col4 fogColor;
		Vec4 fogVector;

		Mat4x4 world;
		Vec4 worldInverseTranspose[3];
		Mat4x4 worldViewProj;
		//�ėp�t���O
		XMUINT4 activeFlg;
		//�ȉ��e
		Vec4 lightPos;
		Vec4 eyePos;
		Mat4x4 lightView;
		Mat4x4 lightProjection;

		int useNumSpheres;                     //�g�p����X�t�B�A�̐�
		int numRayLoop = 10;                   //Ray���[�v��
		float rayHitDistance = 0.025f;         //rayHitDistance���グ��Ί��炩���������B(�@���Ɛ[�x�̐��x��������B)
		float smoothMinConnectValue = 1.25f;   //�R�l�N�g�p�̒萔�B�傫����Ί��炩�Ɍq����A��������Βʏ�̃X�t�B�A�ɋ߂Â��B

		Col4 rimColor = Col4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f);  //�����J���[
		float rimPower = 1.75f;                //�����p���[
		float minRimRate = 0.2f;               //�ŏ��������[�g
		float maxRimRate = 4.0f;               //�ő僊�����[�g
		float colorDistinctness = 0.7f;        //�F�̕�����B�������قǐF��������A�傫���قǐF���͂����肷��B

		SphereData sphereDatas[256];           //�X�t�B�A�f�[�^�z��
	};

	//�R���X�^���g�o�b�t�@�̎���
	DECLARE_DX11_CONSTANT_BUFFER(SlimeBuffer, SlimeConstants)

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class ChildrenRender;
	}

	//--------------------------------------------------------------------------------------
	/// �X���C���̃h���[�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPNTSlimeDraw :public BcBaseDraw
	{
		//�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

		vector<ex_weak_ptr<Metaball::ChildrenRender>> m_renders;  //�������ރf�[�^�z��

		bool m_isDraw = false;  //�ʏ탌���_�[�ŕ`�悵�Ȃ��悤�ɂ��邽�߂̏���

	public :
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		BcPNTSlimeDraw(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override{}

		void OnDraw() override;

	private:
		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="data">���b�V���f�[�^</param>
		void DrawStatic2(const MeshPrimData& data);

		/// <summary>
		/// �R���X�^���g�o�b�t�@�̐ݒ�
		/// </summary>
		/// <param name="pointCb">�ݒ���������ޕϐ�</param>
		/// <param name="data">���b�V���f�[�^</param>
		void SetSlimeConstants(SlimeConstants& pointCb, const MeshPrimData& data);

		/// <summary>
		/// �`�悷�邩�ǂ���
		/// </summary>
		/// <param name="targetPosition">�^�[�Q�b�g�̈ʒu</param>
		/// <returns>�`�悷��Ȃ�true</returns>
		bool IsMetaballDraw(const Vec3& targetPosition);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �A�N�e�B�u�ɂ���
		/// </summary>
		void Active() {
			m_isDraw = true;
		}

		/// <summary>
		/// �`�悷��^�[�Q�b�g�̔z��ݒ�
		/// </summary>
		/// <param name="renders">�`�悷��^�[�Q�b�g�z��</param>
		void SetRenders(const vector<ex_weak_ptr<Metaball::ChildrenRender>>& renders);

		/// <summary>
		/// �`�悷��^�[�Q�b�g�̒ǉ�
		/// </summary>
		/// <param name="render">�ǉ�����^�[�Q�b�g</param>
		void AddRender(const std::shared_ptr<Metaball::ChildrenRender>& render);

		/// <summary>
		/// �`�悷��^�[�Q�b�g�̍폜
		/// </summary>
		/// <param name="render">�폜����^�[�Q�b�g</param>
		void RemoveRender(const ex_weak_ptr<Metaball::ChildrenRender>& render);

		/// <summary>
		/// �S�Ẵ��^�{�[���̒��S�ʒu���擾
		/// </summary>
		/// <returns>�S�Ẵ��^�{�[���̒��S�ʒu</returns>
		Vec3 GetChildrenCenterPosition() const;
	};

}

//endbasecross