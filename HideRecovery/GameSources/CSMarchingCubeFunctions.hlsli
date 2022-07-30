//インクルードガード
#ifndef __CSMARCHINGCUBEFUNCTIONS_HLSL__ 
#define __CSMARCHINGCUBEFUNCTIONS_HLSL__ 

#include "CSMarchingCubeParametors.hlsli"

//--------------------------------------------------------------------------------------
// Manager系Function
//--------------------------------------------------------------------------------------

/*

class Point
{
    float m_i;
    int m_cntr;
    
    float3 m_indexVec;
    
    float CalculateI(int managerCntr)
    {
        if (m_cntr < managerCntr)
        {
            m_cntr = managerCntr;
            float pwr = 0.0f;

            for (int i = 0; i < g_sphereFlag.x; i++)
            {
                float4 blob = g_spheres[i];
                float x = (blob.x - m_indexVec.x) * (blob.x - m_indexVec.x);
                float y = (blob.y - m_indexVec.y) * (blob.y - m_indexVec.y);
                float z = (blob.z - m_indexVec.z) * (blob.z - m_indexVec.z);
                float sqBase = (x + y + z);
                pwr += (1.0f / sqrt(x + y + z)) * blob.w;
            }

            m_i = pwr;
        }

        return m_i;
    }
};

class Edge
{
    float3 position;
    int cntr;
    int axisI;
    int index;
};

class Cube
{
    int cntr;
    int3 indexPoint;
    
    Point points[1];
    Edge edges[1];
};

static const uint MaxArraySize = 65536; //配列の最大数

//シェーダ―リソース
Cube sr_CubeTable[256] : register(u1);  

static const uint NumRecurseData = 6;
static int3 sg_RecurseDatas[NumRecurseData] =
{
    int3(+1, +0, +0),
	int3(-1, +0, +0),
	int3(+0, +1, +0),
	int3(+0, -1, +0),
	int3(+0, +0, +1),
	int3(+0, +0, -1)
};

static const uint NumDoCubeDatas = 12;
static uint3 sg_DoCubeDatas[NumDoCubeDatas] =
{
    uint3(0, 0, 1),
	uint3(1, 1, 2),
	uint3(2, 2, 3),
	uint3(3, 3, 0),
	uint3(4, 4, 5),
	uint3(5, 5, 6),
	uint3(6, 6, 7),
	uint3(7, 7, 4),
	uint3(8, 0, 4),
	uint3(9, 1, 5),
	uint3(10, 2, 6),
	uint3(11, 3, 7)
};

class BlobManager
{
    //uint3 m_dimParam;
    int m_cntr;
    int m_pctr;
    
    int m_vertP;
    int m_trisP;
    int m_cubec;
    
    //頂点/法線/トリス用スクラッチバッファ
    float3 m_newVertex[1];
    float3 m_newNormal[1];
    float3 m_newUV[1];
    int m_newIndices[1];

    uint m_tadac;
    uint m_tadac2;
    
    float3 m_tadaVec3s[1];
    float2 m_tadaVec2s[1];
    
    float m_isOLevel;
    
    void March();
    void RenderMesh();
    
    //プライベートにしたい関数の前方宣言------------------------------------------------
    
    float3 mPosition(Point a, Point b, const int axisI);
    float3 CalculateNormal(const float3 vec);
    Cube GetCube(int x, int y, int z);
    void GenEdge(const Cube cube,
				const int edgei, const int p1i, const int p2i);
    bool DoCube(Cube cube);
    void RecurseCube(Cube cube);
    
    //----------------------------------------------------------------------------------
    
    bool GetCube(int x, int y, int z, inout Cube cube)
    {
        if (x < 0 || y < 0 || z < 0 ||
			x >= g_dimSize.x || y >= g_dimSize.y || z >= g_dimSize.z)
        {
            return false;
        }

        int index = z + (y * (g_dimSize.z)) + (x * (g_dimSize.z) * (g_dimSize.y));
        cube = sr_CubeTable[index];
        
        return true;
    }
    
    float3 mPosition(Point a, Point b, const int axisI)
    {
        float mu = (m_isOLevel - a.CalculateI(m_cntr)) / (b.CalculateI(m_cntr) - a.CalculateI(m_cntr));
        float3 result = m_tadaVec3s[m_tadac++];
        result[0] = a.m_indexVec[0];
        result[1] = a.m_indexVec[1];
        result[2] = a.m_indexVec[2];
        result[axisI] = a.m_indexVec[axisI] + (mu * (b.m_indexVec[axisI] - a.m_indexVec[axisI]));

        return result;
    }
    
    float3 CalculateNormal(const float3 vec)
    {
        float3 result = m_tadaVec3s[m_tadac++];
        result.x = 0;
        result.y = 0;
        result.z = 0;
        for (int i = 0; i < g_sphereFlag.x; i++)
        {
            float4 blob = g_spheres[i];
            
            float3 current = m_tadaVec3s[m_tadac++];
            current.x = vec.x - blob.x;
            current.y = vec.y - blob.y;
            current.z = vec.z - blob.z;
            float range = length(current);
            float pwr = 0.5f * (1.0f / (range * range * range)) * blob.w;
            result = result + (current * pwr);
        }

        return normalize(result);
    }
    
    void GenEdge(const Cube cube, const int edgei, const int p1i, const int p2i)
    {
        float3 vec;
        Edge edge = cube.edges[edgei];
        if (edge.cntr < m_pctr)
        {
            vec = mPosition(cube.points[p1i], cube.points[p2i], edge.axisI);
            edge.position = vec;
            edge.index = m_vertP;
            m_newNormal[m_vertP] = CalculateNormal(vec);
            m_newVertex[m_vertP++] = vec;
            edge.cntr = m_pctr;
        }
    }
    
    bool DoCube(Cube cube)
    {
        int edgeCount = 0;
        int vertCount = 0;

        int cubeIndex = 0;
        int comparisonIndex = 1; //ビット代入演算子用

        const int NumCubeLoop = 8;
        for (int i = 0; i < NumCubeLoop; i++)
        {
            if (cube.points[i].CalculateI(m_cntr) > m_isOLevel)
            {
                cubeIndex |= comparisonIndex; //ビット論理和代入 == 「|=」
            }

            comparisonIndex <<= 1; //左にシフト
        }
        
        int edgeIndex = g_EdgeTable[cubeIndex];
        edgeCount += edgeIndex;
        comparisonIndex = 1;

		//インデックスが0なら処理をしないでfalse
        if (edgeIndex == 0)
        {
            return false;
        }
        
        //GenEdge用のループ
        for (int i = 0; i < NumDoCubeDatas; i++)
        {
            if ((edgeIndex & comparisonIndex) > 0)
            {
                uint3 data = sg_DoCubeDatas[i];
                GenEdge(cube, data.x, data.y, data.z);
            }

            comparisonIndex <<= 1;
        }
        
        //TriTableを使ったループ
        int tpi = 0;
        int temp = 0;
        
        while (g_TriTable[cubeIndex][tpi] != -1)
        {
            int tableIndex = g_TriTable[cubeIndex][tpi + 2];
            temp = cube.edges[tableIndex].index;
            m_newIndices[m_trisP++] = temp;
            vertCount += temp;

            tableIndex = g_TriTable[cubeIndex][tpi + 1];
            temp = cube.edges[tableIndex].index;
            m_newIndices[m_trisP++] = temp;
            vertCount += temp;

            tableIndex = g_TriTable[cubeIndex][tpi];
            temp = cube.edges[tableIndex].index;
            m_newIndices[m_trisP++] = temp;
            vertCount += temp;

            tpi += 3;
        }
        
        return true;
    }
    
    void RecurseCube(Cube cube)
    {
        m_cubec++;

        for (int i = 0; i < NumRecurseData; i++)
        {
			//6軸のケースをテストします。これでうまくいきそうだ。26ケースすべてをテストする必要はない
            Cube nCube;
            bool isCube = GetCube(
                (int) cube.indexPoint.x + sg_RecurseDatas[i].x,
                (int) cube.indexPoint.y + sg_RecurseDatas[i].y,
                (int) cube.indexPoint.z + sg_RecurseDatas[i].z,
                nCube
            );
            
            if (isCube && nCube.cntr < m_pctr)
            {
                nCube.cntr = m_pctr;
                if (DoCube(nCube))
                {
                    RecurseCube(nCube);
                }
            }
        }
    }
    
    void March()
    {
        for (int i = 0; i < g_sphereFlag.x; i++)
        {
            float4 blob = g_spheres[i];
            int x = (int) ((blob.x + 0.5f) * g_dimSize.x);
            int y = (int) ((blob.y + 0.5f) * g_dimSize.y);
            int z = (int) ((blob.z + 0.5f) * g_dimSize.z);

            while (z >= 0)
            {
                Cube cube;
                bool isCube = GetCube(x, y, z, cube);

                if (isCube && cube.cntr < m_pctr)
                {
                    if (DoCube(cube))
                    {
                        RecurseCube(cube);
                        z = -1;
                    }

                    cube.cntr = m_pctr;
                }
                else
                {
                    z = -1;
                }

                z--;
            }
        }
    }
    
    void RenderMesh()
    {
        VertexPositionNormalTexture vertices[1];
        uint indices[1];

		//m_vertices.resize(m_vertP);
        for (int i = 0; i < m_vertP; i++)
        {
            VertexPositionNormalTexture tempV;
            tempV.position = m_newVertex[i];
            tempV.normal = m_newNormal[i];
            tempV.texcode = m_tadaVec2s[m_tadac2++];
			//ローカル座標からワールド座標に変換
            //float3 fuvt = (transform - > GetPosition() + tempV.normal).GetNormalized();
            //tempV.textureCoordinate.x = (fuvt.x + 1.0f) * 0.5f;
            //tempV.textureCoordinate.y = (fuvt.y + 1.0f) * 0.5f;

            vertices[i] = tempV;
        }
        
        //indices
        for (int i = 0; i < m_trisP; i++)
        {
            //m_indices[i] = m_newTris[i];
            indices[i] = m_newIndices[i];
        }
    }
    
};

*/

#endif