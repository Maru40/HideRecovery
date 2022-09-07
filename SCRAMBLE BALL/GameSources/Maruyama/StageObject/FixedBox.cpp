/*!
@file FixedBox.cpp
@brief FixedBox����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "FixedBox.h"

#include "Maruyama/Shader/Toon/PNTToonDraw.h"
#include "Maruyama/Shader/Toon/ToonPNTStaticModelDraw.h"

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

        auto testerFloat = 10.0f;  //�����I�ɏ����B
        auto scale = transform->GetScale() / testerFloat;
        float UCount = scale.x; /// m_UPic;
        float VCount = scale.z; /// m_VPic;
        float YVCount = scale.y;

        for (size_t i = 0; i < vertices.size(); i++) {
            //���
            float TopBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 1, 0));
            //����
            float BottomBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, -1, 0));
            //������
            float LeftSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(-1, 0, 0));
            //�E����
            float RightSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(1, 0, 0));
            //��O����
            float ForeSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 0, -1));
            //������
            float BackSideBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 0, 1));
            if (TopBetween < 0.01f || BottomBetween < 0.01f) {
                //�㉺�ʂ̏���
                if (vertices[i].textureCoordinate.x >= 1.0f) {
                    vertices[i].textureCoordinate.x = UCount;
                }
                if (vertices[i].textureCoordinate.y >= 1.0f) {
                    vertices[i].textureCoordinate.y = VCount;
                }
            }
            else {
                //����ȊO�i�܂艡�ʁj
                if (vertices[i].textureCoordinate.x >= 1.0f) {
                    //z�ɒ�����x�ɒ������𔻒f
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

        //�`��R���|�[�l���g�̒ǉ�
        //auto PtrDraw = AddComponent<BcPNTStaticDraw>();
        auto PtrDraw = GetComponent<DrawComp>();
        //PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
        PtrDraw->CreateOriginalMesh(vertices, indices);

        PtrDraw->SetOriginalMeshUse(true);
        //�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
        PtrDraw->SetOwnShadowActive(true);
        //�`��R���|�[�l���g�e�N�X�`���̐ݒ�
        if (m_texture != L"") {
            PtrDraw->SetTextureResource(m_texture);
        }
        else {
            PtrDraw->SetTextureResource(L"Floor_TX");
        }
        //�^�C�����O�ݒ�
        PtrDraw->SetSamplerState(SamplerState::LinearWrap);

        m_vertices = vertices;
        m_indices = indices;

        m_meshCreateScale = transform->GetScale();
    }

}