/*!
@file BuilderVertexPCT.h
@brief BuilderVertexPCT
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Builder
	{
		//--------------------------------------------------------------------------------------
		///	UI�̎�
		//--------------------------------------------------------------------------------------
		enum class UIPivot {
			Center,		//���S
			Up,			//��
			Down,		//��
			Left,		//��
			LeftUp,		//����
			LeftDown,	//����
			Right,		//�E
			RightUp,	//�E��
			RightDown,	//�E��
			Forward,	//����
			Back		//���
		};

		//--------------------------------------------------------------------------------------
		///	�g�p����UV�͈�
		//--------------------------------------------------------------------------------------
		struct UseUVRange
		{
			float start;  //�J�n�ʒu
			float end;    //�I���ʒu

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="start">�J�n�ʒu</param>
			/// <param name="end">�I���ʒu</param>
			UseUVRange(const float start, const float end)
				:start(start), end(end)
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="range">�͈�</param>
			UseUVRange(const Vec2& range)
				:start(range.x), end(range.y)
			{}
		};

		//--------------------------------------------------------------------------------------
		///	�X�v���C�g�����p�����[�^
		//--------------------------------------------------------------------------------------
		struct VertexPCTParametor
		{
			Vec3 scale;               //�X�P�[��
			float size;               //�S�̃T�C�Y

			Col4 color;               //�F

			UseUVRange useUVRange_X;  //�g�p����UV�͈͂�X
			UseUVRange useUVRange_Y;  //�g�p����UV�͈͂�Y

			Vec2 useSpriteSize;       //�摜�̑傫��
			wstring texture;          //�e�N�X�`��
			UIPivot pivot;            //�s�{�b�g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scale">�傫��</param>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec3& scale, 
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scale">�傫��</param>
			/// <param name="size">�S�̃T�C�Y</param>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec3& scale, const float& size,
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scale">�傫��</param>
			/// <param name="color">�F</param>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec3& scale, const Col4& color,
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scale">�傫��</param>
			/// <param name="size">�S�̃T�C�Y</param>
			/// <param name="color">�F</param>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec3& scale, const float& size, const Col4& color,
				const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scale">�傫��</param>
			/// <param name="color">�F</param>
			/// <param name="useUVRange_X">�g�p����UV�͈͂�X</param>
			/// <param name="useUVRange_Y">�g�p����UV�͈͂�Y</param>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec3& scale, const Col4& color,
				const UseUVRange& useUVRange_X, const UseUVRange& useUVRange_Y, const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="scale">�傫��</param>
			/// <param name="size">�S�̃T�C�Y</param>
			/// <param name="color">�F</param>
			/// <param name="useUVRange_X">�g�p����UV�͈͂�X</param>
			/// <param name="useUVRange_Y">�g�p����UV�͈͂�Y</param>
			/// <param name="useSpriteSize">�摜�̑傫��</param>
			/// <param name="texture">�e�N�X�`����</param>
			/// <param name="pivot">��</param>
			VertexPCTParametor(const Vec3& scale, const float& size, const Col4& color,
				const UseUVRange& useUVRange_X, const UseUVRange& useUVRange_Y, const Vec2& useSpriteSize,
				const wstring& texture, const UIPivot pivot = UIPivot::Center);
		};

		//--------------------------------------------------------------------------------------
		///	�X�v���C�g�����N���X
		//--------------------------------------------------------------------------------------
		class BuilderVertexPCT
		{
		public:

			vector<VertexPositionColorTexture> m_vertices;  //���_�z��
			vector<uint16_t> m_indices;						//�C���f�b�N�X�z��

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="parametor">�X�v���C�g�����p�p�����[�^</param>
			BuilderVertexPCT(const VertexPCTParametor& parametor);

		private:
			/// <summary>
			/// �g�p����UV�͈͂𐶐�
			/// </summary>
			/// <param name="parametor">�����p�p�����[�^</param>
			/// <returns>�g�p����UV�͈�</returns>
			vector<Vec2> CreateUseUV(const VertexPCTParametor& parametor);

			/// <summary>
			/// ���_�f�[�^�𐶐�����B
			/// </summary>
			/// <param name="parametor">�����p�p�����[�^</param>
			void CreateVertices(const VertexPCTParametor& parametor);

		};

	}
}

//endbasecross