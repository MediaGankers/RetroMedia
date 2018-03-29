#define LOG_TAG "MetaData"
#include <inttypes.h>
#include <algorithm>
#include "log_defs.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "MetaData.h"

MetaData::MetaData() {
}

MetaData::MetaData(const MetaData &from)
      :mItems(from.mItems) {
}

MetaData::~MetaData() {
    clear();
}

void MetaData::clear() {
    mItems.clear();
}

bool MetaData::remove(uint32_t key) {
    ItemType::iterator i = mItems.find(key);

    if (i == mItems.end()) {
        return false;
    }

    mItems.erase(i);
    return true;
}

bool MetaData::setCString(uint32_t key, const char *value) {
    return setData(key, TYPE_C_STRING, value, strlen(value) + 1);
}

bool MetaData::setInt32(uint32_t key, int32_t value) {
    return setData(key, TYPE_INT32, &value, sizeof(value));
}

bool MetaData::setInt64(uint32_t key, int64_t value) {
    return setData(key, TYPE_INT64, &value, sizeof(value));
}

bool MetaData::setFloat(uint32_t key, float value) {
    return setData(key, TYPE_FLOAT, &value, sizeof(value));
}

bool MetaData::setPointer(uint32_t key, void *value) {
    return setData(key, TYPE_POINTER, &value, sizeof(value));
}

bool MetaData::setRect(
        uint32_t key,
        int32_t left, int32_t top,
        int32_t right, int32_t bottom) {
    Rect r;
    r.mLeft = left;
    r.mTop = top;
    r.mRight = right;
    r.mBottom = bottom;

    return setData(key, TYPE_RECT, &r, sizeof(r));
}

/**
 * Note that the returned pointer becomes invalid when additional metadata is set.
 */
bool MetaData::findCString(uint32_t key, const char **value) {
    uint32_t type;
    const void *data;
    size_t size;
    if (!findData(key, &type, &data, &size) || type != TYPE_C_STRING) {
        return false;
    }

    *value = (const char *)data;

    return true;
}

bool MetaData::findInt32(uint32_t key, int32_t *value) {
    uint32_t type = 0;
    const void *data;
    size_t size;
    if (!findData(key, &type, &data, &size) || type != TYPE_INT32) {
        return false;
    }

    CHECK_EQ(size, sizeof(*value));

    *value = *(int32_t *)data;

    return true;
}

bool MetaData::findInt64(uint32_t key, int64_t *value) {
    uint32_t type = 0;
    const void *data;
    size_t size;
    if (!findData(key, &type, &data, &size) || type != TYPE_INT64) {
        return false;
    }

    CHECK_EQ(size, sizeof(*value));

    *value = *(int64_t *)data;

    return true;
}

bool MetaData::findFloat(uint32_t key, float *value) {
    uint32_t type = 0;
    const void *data;
    size_t size;
    if (!findData(key, &type, &data, &size) || type != TYPE_FLOAT) {
        return false;
    }

    CHECK_EQ(size, sizeof(*value));

    *value = *(float *)data;

    return true;
}

bool MetaData::findPointer(uint32_t key, void **value) {
    uint32_t type = 0;
    const void *data;
    size_t size;
    if (!findData(key, &type, &data, &size) || type != TYPE_POINTER) {
        return false;
    }

    CHECK_EQ(size, sizeof(*value));

    *value = *(void **)data;

    return true;
}

bool MetaData::findRect(
        uint32_t key,
        int32_t *left, int32_t *top,
        int32_t *right, int32_t *bottom) {
    uint32_t type = 0;
    const void *data;
    size_t size;
    if (!findData(key, &type, &data, &size) || type != TYPE_RECT) {
        return false;
    }

    CHECK_EQ(size, sizeof(Rect));

    const Rect *r = (const Rect *)data;
    *left = r->mLeft;
    *top = r->mTop;
    *right = r->mRight;
    *bottom = r->mBottom;

    return true;
}

bool MetaData::setData(
        uint32_t key, uint32_t type, const void *data, size_t size) {
    bool overwrote_existing = true;

    ItemType::iterator i = mItems.find(key);
    if (i == mItems.end()) {
        typed_data item;
        mItems.insert(std::make_pair(key, item));

        overwrote_existing = false;
    }

    typed_data &item = mItems[key];

    item.setData(type, data, size);

    return overwrote_existing;
}

bool MetaData::findData(uint32_t key, uint32_t *type,
                        const void **data, size_t *size) const {
    ItemType::const_iterator i = mItems.find(key);

    if (i == mItems.end()) {
        return false;
    }

    const typed_data &item = i->second;

    item.getData(type, data, size);

    return true;
}

bool MetaData::hasData(uint32_t key) const {
    ItemType::const_iterator i = mItems.find(key);

    if (i == mItems.end()) {
        return false;
    }

    return true;
}

MetaData::typed_data::typed_data()
    : mType(0),
      mSize(0) {
}

MetaData::typed_data::~typed_data() {
    clear();
}

MetaData::typed_data::typed_data(const typed_data &from)
    : mType(from.mType),
      mSize(0) {

    void *dst = allocateStorage(from.mSize);
    if (dst) {
        memcpy(dst, from.storage(), mSize);
    }
}

MetaData::typed_data &MetaData::typed_data::operator=(
        const MetaData::typed_data &from) {
    if (this != &from) {
        clear();
        mType = from.mType;
        void *dst = allocateStorage(from.mSize);
        if (dst) {
            memcpy(dst, from.storage(), mSize);
        }
    }

    return *this;
}

void MetaData::typed_data::clear() {
    freeStorage();

    mType = 0;
}

void MetaData::typed_data::setData(
        uint32_t type, const void *data, size_t size) {
    clear();

    mType = type;

    void *dst = allocateStorage(size);
    if (dst) {
        memcpy(dst, data, size);
    }
}

void MetaData::typed_data::getData(
        uint32_t *type, const void **data, size_t *size) const {
    *type = mType;
    *size = mSize;
    *data = storage();
}

void *MetaData::typed_data::allocateStorage(size_t size) {
    mSize = size;

    if (usesReservoir()) {
        return &u.reservoir;
    }

    u.ext_data = malloc(mSize);
    if (u.ext_data == NULL) {
        ALOGE("Couldn't allocate %zu bytes for item", size);
        mSize = 0;
    }
    return u.ext_data;
}

void MetaData::typed_data::freeStorage() {
    if (!usesReservoir()) {
        if (u.ext_data) {
            free(u.ext_data);
            u.ext_data = NULL;
        }
    }

    mSize = 0;
}

std::string MetaData::typed_data::asString(bool verbose) const {
    char buffer[1024] = {0};
    const void *data = storage();
    switch(mType) {
        case TYPE_NONE:
            sprintf(buffer, "no type, size %zu)", mSize);
            break;
        case TYPE_C_STRING:
            sprintf(buffer, "(char*) %s", (const char *)data);
            break;
        case TYPE_INT32:
            sprintf(buffer, "(int32_t) %d", *(int32_t *)data);
            break;
        case TYPE_INT64:
            sprintf(buffer, "(int64_t) %" PRId64, *(int64_t *)data);
            break;
        case TYPE_FLOAT:
            sprintf(buffer, "(float) %f", *(float *)data);
            break;
        case TYPE_POINTER:
            sprintf(buffer, "(void*) %p", *(void **)data);
            break;
        case TYPE_RECT:
        {
            const Rect *r = (const Rect *)data;
            sprintf(buffer, "Rect(%d, %d, %d, %d)",
                                  r->mLeft, r->mTop, r->mRight, r->mBottom);
            break;
        }

        default:
            sprintf(buffer, "(unknown type %d, size %zu)", mType, mSize);
            if (verbose && mSize <= 48) { // if it's less than three lines of hex data, dump it
                //Dump ?
            }
            break;
    }
    return buffer;
}

static void MakeFourCCString(uint32_t x, char *s) {
    s[0] = x >> 24;
    s[1] = (x >> 16) & 0xff;
    s[2] = (x >> 8) & 0xff;
    s[3] = x & 0xff;
    s[4] = '\0';
}

std::string MetaData::toString() const {
    std::ostringstream s;
    for (ItemType::const_iterator i = mItems.begin(); i != mItems.end(); i++) {
        int32_t key = i->first;
        char cc[5];
        MakeFourCCString(key, cc);
        const typed_data &item = i->second;
        s << cc << ", " << item.asString(false);
        if (i != mItems.begin()) {
            s << ", ";
        }
    }
    return s.str();
}

void MetaData::dumpToLog() const {
    for (ItemType::const_iterator i = mItems.begin(); i != mItems.end(); i++) {
        int32_t key = i->first;
        char cc[5];
        MakeFourCCString(key, cc);
        const typed_data &item = i->second;
        ALOGI("%s: %s", cc, item.asString(true /* verbose */).c_str());
    }
}

int MetaData::decRef() {
    int cnt = Reference::decRef();
    assert(cnt >= 1);
    if (cnt == 1) {
        delete this;
    }
    return cnt - 1;
}


