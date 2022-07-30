/*!
@file FixedBox.cpp
@brief FixedBox実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FixedBox.h"

#include "PNTToonDraw.h"
#include "ToonPNTStaticModelDraw.h"

namespace basecross {

    //using DrawComp = PNTToonDraw;
    using DrawComp = PNTStaticDraw;

    void FixedBox::OnCreate()
    {
        StageObjectBase::OnCreate();

        auto coll = AddComponent<CollisionObb>();
        coll->SetFixed(true);

        AddTag(L"FixedBox");

        AddTag(m_name);
        coll->AddExcludeCollisionTag(L"FixedBox");

        auto draw = AddComponent<DrawComp>();
        
        auto shadow = AddComponent<Shadowmap>();
        draw->SetOwnShadowActive(true);

        UpdateMesh();
    }

    void FixedBox::OnUpdate()
    {
        if (m_meshCreateScale != transform->GetScale()) {
            UpdateMesh();
        }
    }

    void FixedBox::UpdateMesh() {
        vector<VertexPositionNormalTexture> vertices;
        vector<uint16_t> indices;
        MeshUtill::CreateCube(1.0f, vertices, indices);

        auto testerFloat = 15.0f;  //将来的に消す。
        auto scale = transform->GetScale() / testerFloat;
        float UCount = scale.x; /// m_UPic;
        float VCount = scale.z; /// m_VPic;
        float YVCount = scale.y;

        for (size_t i = 0; i < vertices.size(); i++) {
            //上面
            float TopBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 1, 0));
            //下面
            float BottomBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, -1, 0));
            //左横面
            float LeftSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(-1, 0, 0));
            //右横面
            float RightSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(1, 0, 0));
            //手前横面
            float ForeSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 0, -1));
            //奥横面
            float BackSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 0, 1));
            if (TopBetween < 0.01f || BottomBetween < 0.01f) {
                //上下面の処理
                if (vertices[i].textureCoordinate.x >= 1.0f) {
                    vertices[i].textureCoordinate.x = UCount;
                }
                if (vertices[i].textureCoordinate.y >= 1.0f) {
                    vertices[i].textureCoordinate.y = VCount;
                }
            }
            else {
                //それ以外（つまり横面）
                if (vertices[i].textureCoordinate.x >= 1.0f) {
                    //zに長いかxに長いかを判断
                    if (LeftSideBetween < 0.01f || RightSideBetween < 0.01f) {
                        vertices[i].textureCoordinate.x = scale.z;
                    }
                    else {
                        vertices[i].textureCoordinate.x = scale.x;
                    }
                }

                if (vertices[i].textureCoordinate.y >= 1.0f) {
                    vertices[i].textureCoordinate.y = YVCount;
                }
            }
        }

        //描画コンポーネントの追加
        //auto PtrDraw = AddComponent<BcPNTStaticDraw>();
        auto PtrDraw = GetComponent<DrawComp>();
        //PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
        PtrDraw->CreateOriginalMesh(vertices, indices);

        PtrDraw->SetOriginalMeshUse(true);
        //描画コンポーネントに形状（メッシュ）を設定
        PtrDraw->SetOwnShadowActive(true);
        //描画コンポーネントテクスチャの設定
        if (m_texture != L"") {
            PtrDraw->SetTextureResource(m_texture);
        }
        else {
            PtrDraw->SetTextureResource(L"Floor_TX");
        }
        //タイリング設定
        PtrDraw->SetSamplerState(SamplerState::LinearWrap);

        m_vertices = vertices;
        m_indices = indices;

        m_meshCreateScale = transform->GetScale();
    }

}