#include <gl/Mesh.h>
#include <gl/util.h>

namespace openGl {

Mesh::Mesh(Primitive primitive, size_t vertexCount, size_t vertexSize, size_t texCoordSize)
    : mVertexCount(vertexCount), mVertexSize(vertexSize), mTexCoordsSize(texCoordSize),
      mPrimitive(primitive)
{
    if (vertexCount == 0) {
        mVertices = new float[1];
        mVertices[0] = 0.0f;
        mStride = 0;
        return;
    }

    size_t stride = vertexSize + texCoordSize;
    size_t remainder = (stride * vertexCount) / vertexCount;
    // Since all of the input parameters are unsigned, if stride is less than
    // either vertexSize or texCoordSize, it must have overflowed. remainder
    // will be equal to stride as long as stride * vertexCount doesn't overflow.
    if ((stride < vertexSize) || (remainder != stride)) {
        ALOGE("Overflow in Mesh(..., %zu, %zu, %zu)", vertexCount, vertexSize,
                texCoordSize);
        mVertices = new float[1];
        mVertices[0] = 0.0f;
        mVertexCount = 0;
        mVertexSize = 0;
        mTexCoordsSize = 0;
        mStride = 0;
        return;
    }

    mVertices = new float[stride * vertexCount];
    mStride = stride;
}

Mesh::~Mesh() {
    delete [] mVertices;
}

Mesh::Primitive Mesh::getPrimitive() const {
    return mPrimitive;
}


float const* Mesh::getPositions() const {
    return mVertices;
}
float* Mesh::getPositions() {
    return mVertices;
}

float const* Mesh::getTexCoords() const {
    return mVertices + mVertexSize;
}
float* Mesh::getTexCoords() {
    return mVertices + mVertexSize;
}


size_t Mesh::getVertexCount() const {
    return mVertexCount;
}

size_t Mesh::getVertexSize() const {
    return mVertexSize;
}

size_t Mesh::getTexCoordsSize() const {
    return mTexCoordsSize;
}

size_t Mesh::getByteStride() const {
    return mStride*sizeof(float);
}

size_t Mesh::getStride() const {
    return mStride;
}

} /* namespace android */
