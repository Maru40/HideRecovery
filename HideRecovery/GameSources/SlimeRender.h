/*!
@file SlimeRender.h
@brief SlimeRender�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BcPNTSlimeDraw;
	class RayMarchingFilterObject;
	struct PressData;

	namespace maru {
		enum class DeltaType;
	}

	namespace Metaball {
		class ChildrenRender;
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �X���C���̃����_�[�N���X
		//--------------------------------------------------------------------------------------
		class SlimeRender : public basecross::Metaball::RenderBase
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			SlimeRender(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="numCreate">�������鐔</param>
			SlimeRender(const std::shared_ptr<GameObject>& objPtr, const int& numCreate);
		};

	}
}