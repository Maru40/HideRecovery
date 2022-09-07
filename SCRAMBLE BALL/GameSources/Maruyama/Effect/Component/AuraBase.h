/*!
@file AuraBase.h
@brief AuraBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// �I�[���X�V�p�����[�^
		//--------------------------------------------------------------------------------------
		struct Aura_UpdateParametor {
			float speed;  //�ړ��X�s�[�h
			Vec2 direct;  //�ړ�����

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Aura_UpdateParametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ړ��X�s�[�h</param>
			/// <param name="direct">�ړ�����</param>
			Aura_UpdateParametor(const float& speed, const Vec2& direct);
		};

		//--------------------------------------------------------------------------------------
		/// �I�[���p�����[�^
		//--------------------------------------------------------------------------------------
		struct Aura_Parametor
		{
			int sides;                         //�T�C�h�X  
			float height;                      //����
			float topRadius;                   //��̔��a
			float bottomRadius;                //���̔��a
			Col4 topColor;                     //��̃J���[
			Col4 bottomColor;                  //���̃J���[
			wstring texture;                   //�e�N�X�`����
			Aura_UpdateParametor updateParam;  //�X�V�p�p�����[�^
			BlendState blendStage;             //�u�����h�X�e�[�g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Aura_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="sides">�T�C�h�X</param>
			/// <param name="height">����</param>
			/// <param name="topRadius">��̔��a</param>
			/// <param name="bottomRadius">���̔��a</param>
			/// <param name="color">�J���[</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="updateParam">�X�V�p�����[�^</param>
			Aura_Parametor(
				const int& sides, const float& height,
				const float& topRadius, const float& bottomRadius,
				const Col4& color, 
				const wstring& texture,
				const Aura_UpdateParametor& updateParam
			);

		};

		//--------------------------------------------------------------------------------------
		/// �I�[���̊��N���X
		//--------------------------------------------------------------------------------------
		class AuraBase : public Component
		{
		public:
			using Parametor = Aura_Parametor;
			using UpdateParametor = Aura_UpdateParametor;
			using DrawComp = BcPCTStaticDraw;

		private:
			Parametor m_param;                                   //�p�����[�^
			std::vector<VertexPositionColorTexture> m_vertices;  //���_�z��

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			AuraBase(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			AuraBase(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// UV�A�b�v�f�[�g
			/// </summary>
			void UpdateUV();

			/// <summary>
			/// ���_�̐���
			/// </summary>
			/// <returns>���_�z��</returns>
			std::vector<VertexPositionColorTexture> CreateVertices();

			/// <summary>
			/// �C���f�B�Z�X�̐���
			/// </summary>
			/// <returns>�C���f�B�Z�X</returns>
			std::vector<uint16_t> CreateIndices();

		};

	}
}