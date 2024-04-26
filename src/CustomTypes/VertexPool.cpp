#include "CustomTypes/VertexPool.hpp"
#include "logging.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Rendering/ShadowCastingMode.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Transform.hpp"


#include "CustomTypes/TrailComponent.hpp"

DEFINE_TYPE(Qosmetics::Sabers, VertexPool);

using namespace UnityEngine;
using namespace Sombrero;

namespace Qosmetics::Sabers
{
    void VertexPool::ctor(Material* material, TrailComponent* owner)
    {
        INVOKE_CTOR();
        vertexTotal = vertexUsed = indexUsed = indexTotal = 0;
        vertCountChanged = false;
        this->owner = owner;
        CreateMeshObj(owner, material);
        _material = material;
        InitArrays();
        IndiceChanged = ColorChanged = UVChanged = UV2Changed = VertChanged = true;
    }

    void VertexPool::RecalculateBounds()
    {
        auto mesh = get_MyMesh();
        if (mesh && mesh->m_CachedPtr)
            mesh->RecalculateBounds();
    }

    Mesh* VertexPool::get_MyMesh() const
    {
        if (_meshFilter && _meshFilter->m_CachedPtr)
            return _meshFilter->get_sharedMesh();
        return nullptr;
    }

    void VertexPool::SetMeshObjectActive(bool flag) const
    {
        if (!_meshFilter || !_meshFilter->m_CachedPtr)
            return;
        _meshFilter->get_gameObject()->SetActive(flag);
    }

    void VertexPool::Destroy() const
    {
        if (_gameObject && _gameObject->m_CachedPtr)
            UnityEngine::Object::Destroy(_gameObject);
    }

    VertexSegment VertexPool::GetVertices(int vcount, int icount)
    {
        int vertNeed = 0;
        int indexNeed = 0;
        if ((vertexUsed + vcount) >= vertexTotal)
        {
            vertNeed = ((vcount / BlockSize) + 1) * BlockSize;
        }

        if ((indexUsed + icount) >= indexTotal)
        {
            indexNeed = ((icount / BlockSize) + 1) * BlockSize;
        }

        vertexUsed += vcount;
        indexUsed += icount;

        if (vertNeed != 0 || indexNeed != 0)
        {
            EnlargeArrays(vertNeed, indexNeed);
            vertexTotal += vertNeed;
            indexTotal += indexNeed;
        }
        return VertexSegment(vertexUsed - vcount, vcount, indexUsed - icount, icount, this);
    }

    void VertexPool::EnlargeArrays(int count, int icount)
    {
        int length = Vertices.size();
        auto tempVertices = Vertices;
        _vertices = ArrayW<Sombrero::FastVector3>(il2cpp_array_size_t(length + icount));
        memcpy(Vertices.begin(), tempVertices.begin(), tempVertices.size() * sizeof(Sombrero::FastVector3));

        length = UVs.size();
        auto tempUVs = UVs;
        _uvs = ArrayW<Sombrero::FastVector2>(il2cpp_array_size_t(length + icount));
        memcpy(UVs.begin(), tempUVs.begin(), tempUVs.size() * sizeof(Sombrero::FastVector2));

        length = Colors.size();
        auto tempColors = Colors;
        _colors = ArrayW<Sombrero::FastColor>(il2cpp_array_size_t(length + icount));
        memcpy(Colors.begin(), tempColors.begin(), tempColors.size() * sizeof(Sombrero::FastColor));

        length = Indices.size();
        auto tempIndices = Indices;
        _indices = ArrayW<int>(il2cpp_array_size_t(length + icount));
        memcpy(Indices.begin(), tempIndices.begin(), tempIndices.size() * sizeof(int));

        vertCountChanged = true;
        IndiceChanged = true;
        ColorChanged = true;
        UVChanged = true;
        VertChanged = true;
        UV2Changed = true;
    }

    void VertexPool::LateUpdate()
    {
        auto mymesh = get_MyMesh();
        if (!mymesh || !mymesh->m_CachedPtr)
            return;

        if (vertCountChanged)
            mymesh->Clear();

        mymesh->set_vertices(reinterpret_cast<Array<Vector3>*>(Vertices.convert()));
        if (UVChanged)
            mymesh->set_uv(reinterpret_cast<Array<Vector2>*>(UVs.convert()));
        if (ColorChanged)
            mymesh->set_colors(reinterpret_cast<Array<Color>*>(Colors.convert()));
        if (IndiceChanged)
            mymesh->set_triangles(Indices);

        ElapsedTime += Time::get_deltaTime();

        if (ElapsedTime > BoundsScheduleTime || FirstUpdate)
        {
            RecalculateBounds();
            ElapsedTime = 0.0f;
        }

        if (ElapsedTime > BoundsScheduleTime)
            FirstUpdate = false;

        vertCountChanged = false;
        IndiceChanged = false;
        ColorChanged = false;
        UVChanged = false;
        UV2Changed = false;
        VertChanged = false;
    }

    void VertexPool::CreateMeshObj(TrailComponent* owner, Material* material)
    {
        static ConstString saberTrailName("SaberTrail");
        _gameObject = GameObject::New_ctor(saberTrailName);
        _gameObject->set_layer(owner->get_gameObject()->get_layer());
        _meshFilter = _gameObject->AddComponent<MeshFilter*>();
        auto meshrenderer = _gameObject->AddComponent<MeshRenderer*>();

        _gameObject->get_transform()->set_position(FastVector3::zero());
        _gameObject->get_transform()->set_rotation(Quaternion::get_identity());

        meshrenderer->set_shadowCastingMode(UnityEngine::Rendering::ShadowCastingMode::Off);
        meshrenderer->set_receiveShadows(false);
        meshrenderer->set_sharedMaterial(material);
        // not available on quest
        // meshrenderer->set_sortingLayerName(this->owner->SortingLayerName);
        meshrenderer->set_sortingOrder(this->owner->SortingOrder);
        _meshFilter->set_sharedMesh(Mesh::New_ctor());

        SetMeshObjectActive(false);
    }

    void VertexPool::InitArrays()
    {
        _vertices = ArrayW<Sombrero::FastVector3>(il2cpp_array_size_t(4));
        _uvs = ArrayW<Sombrero::FastVector2>(il2cpp_array_size_t(4));
        _colors = ArrayW<Sombrero::FastColor>(il2cpp_array_size_t(4));
        _indices = ArrayW<int>(il2cpp_array_size_t(6));
        vertexTotal = 4;
        indexTotal = 6;
    }

    void VertexPool::SetMaterial(Material* material)
    {
        if (_gameObject)
        {
            _material = material;
            _gameObject->GetComponent<Renderer*>()->set_sharedMaterial(material);
        }
    }

    ArrayW<Sombrero::FastVector3> VertexPool::get_Vertices() { return _vertices; };
    ArrayW<int> VertexPool::get_Indices() { return _indices; };
    ArrayW<Sombrero::FastVector2> VertexPool::get_UVs() { return _uvs; };
    ArrayW<Sombrero::FastColor> VertexPool::get_Colors() { return _colors; };

}
