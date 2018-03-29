#ifndef SF_RENDER_ENGINE_MESH_H
#define SF_RENDER_ENGINE_MESH_H

#include <stdint.h>

namespace openGl{

class Mesh {
public:
    enum Primitive {
        TRIANGLES       = 0x0004,       // GL_TRIANGLES
        TRIANGLE_STRIP  = 0x0005,       // GL_TRIANGLE_STRIP
        TRIANGLE_FAN    = 0x0006        // GL_TRIANGLE_FAN
    };

    Mesh(Primitive primitive, size_t vertexCount, size_t vertexSize, size_t texCoordsSize = 0);
    ~Mesh();

    /*
     * VertexArray handles the stride automatically.
     */
    template <typename TYPE>
    class VertexArray {
        friend class Mesh;
        float* mData;
        size_t mStride;
        VertexArray(float* data, size_t stride) : mData(data), mStride(stride) { }
    public:
        TYPE& operator[](size_t index) {
            return *reinterpret_cast<TYPE*>(&mData[index*mStride]);
        }
        TYPE const& operator[](size_t index) const {
            return *reinterpret_cast<TYPE const*>(&mData[index*mStride]);
        }
    };

    template <typename TYPE>
    VertexArray<TYPE> getPositionArray() { return VertexArray<TYPE>(getPositions(), mStride); }

    template <typename TYPE>
    VertexArray<TYPE> getTexCoordArray() { return VertexArray<TYPE>(getTexCoords(), mStride); }

    Primitive getPrimitive() const;

    // returns a pointer to the vertices positions
    float const* getPositions() const;

    // returns a pointer to the vertices  texture coordinates
    float const* getTexCoords() const;

    // number of vertices in this mesh
    size_t getVertexCount() const;

    // dimension of vertices
    size_t getVertexSize() const;

    // dimension of texture coordinates
    size_t getTexCoordsSize() const;

    // return stride in bytes
    size_t getByteStride() const;

    // return stride in floats
    size_t getStride() const;

private:
    Mesh(const Mesh&);
    Mesh& operator = (const Mesh&);
    Mesh const& operator = (const Mesh&) const;

    float* getPositions();
    float* getTexCoords();
    float* mVertices;
    size_t mVertexCount;
    size_t mVertexSize;
    size_t mTexCoordsSize;
    size_t mStride;
    Primitive mPrimitive;
};


} /* namespace android */
#endif /* SF_RENDER_ENGINE_MESH_H */
