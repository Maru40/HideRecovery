
/*!
@file StateNodeBase.h
@brief StateNodeBase
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

    //--------------------------------------------------------------------------------------
    ///	ステートノードのインターフェース
    //--------------------------------------------------------------------------------------
    class I_StateNode
    {
    public:
        /// <summary>
        /// 開始時に呼び出す処理
        /// </summary>
        virtual void OnStart() = 0;

        /// <summary>
        /// 毎フレーム呼び出す処理(外部呼び出し)
        /// </summary>
        /// <returns>処理が完了したらtrue</returns>
        virtual bool OnUpdate() = 0;

        /// <summary>
        /// 終了時に呼び出す処理
        /// </summary>
        virtual void OnExit() = 0;
    };

    //--------------------------------------------------------------------------------------
    ///	前方宣言
    //--------------------------------------------------------------------------------------
    template<class NodeType>
    class StateNodeBase;

    //--------------------------------------------------------------------------------------
    ///	ステートノードの基底クラス
    //--------------------------------------------------------------------------------------
	template<class NodeType>
	class StateNodeBase : public I_StateNode
	{
        std::weak_ptr<NodeType> m_owner;  //所有者のポインタ

	public:
 
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="owner">所有者</param>
        StateNodeBase(const std::shared_ptr<NodeType>& owner)
            :m_owner(owner)
        {}

    protected:

        /// <summary>
        /// 所有者の取得
        /// </summary>
        /// <returns>所有者</returns>
        std::shared_ptr<NodeType> GetOwner() const {
            auto owner = m_owner.lock();
            return owner ? owner : nullptr;
        }

	};

}

//endbasecross