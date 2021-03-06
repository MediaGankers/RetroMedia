#define LOG_TAG "Region"

#include <inttypes.h>
#include <limits.h>

#include <gl/Rect.h>
#include <gl/Region.h>
#include <gl/Point.h>

#include "RegionHelper.h"

// ----------------------------------------------------------------------------
#define VALIDATE_REGIONS        (false)
#define VALIDATE_WITH_CORECG    (false)
// ----------------------------------------------------------------------------

#if VALIDATE_WITH_CORECG
#include "SkRegion.h"
#endif

namespace openGl {
// ----------------------------------------------------------------------------

enum {
    op_nand = region_operator<Rect>::op_nand,
    op_and  = region_operator<Rect>::op_and,
    op_or   = region_operator<Rect>::op_or,
    op_xor  = region_operator<Rect>::op_xor
};

enum {
    direction_LTR,
    direction_RTL
};

// ----------------------------------------------------------------------------

Region::Region() {
    mStorage.push_back(Rect(0,0));
}

Region::Region(const Region& rhs)
    : mStorage(rhs.mStorage)
{
#if VALIDATE_REGIONS
    validate(rhs, "rhs copy-ctor");
#endif
}

Region::Region(const Rect& rhs) {
    mStorage.push_back(rhs);
}

Region::~Region()
{
}

/**
 * Copy rects from the src vector into the dst vector, resolving vertical T-Junctions along the way
 *
 * First pass through, divideSpanRTL will be set because the 'previous span' (indexing into the dst
 * vector) will be reversed. Each rectangle in the original list, starting from the bottom, will be
 * compared with the span directly below, and subdivided as needed to resolve T-junctions.
 *
 * The resulting temporary vector will be a completely reversed copy of the original, without any
 * bottom-up T-junctions.
 *
 * Second pass through, divideSpanRTL will be false since the previous span will index into the
 * final, correctly ordered region buffer. Each rectangle will be compared with the span directly
 * above it, and subdivided to resolve any remaining T-junctions.
 */
static void reverseRectsResolvingJunctions(const Rect* begin, const Rect* end,
        std::vector<Rect>& dst, int spanDirection) {
    dst.clear();

    const Rect* current = end - 1;
    int lastTop = current->top;

    // add first span immediately
    do {
        dst.push_back(*current);
        current--;
    } while (current->top == lastTop && current >= begin);

    unsigned int beginLastSpan = -1;
    unsigned int endLastSpan = -1;
    int top = -1;
    int bottom = -1;

    // for all other spans, split if a t-junction exists in the span directly above
    while (current >= begin) {
        if (current->top != (current + 1)->top) {
            // new span
            if ((spanDirection == direction_RTL && current->bottom != (current + 1)->top) ||
                    (spanDirection == direction_LTR && current->top != (current + 1)->bottom)) {
                // previous span not directly adjacent, don't check for T junctions
                beginLastSpan = INT_MAX;
            } else {
                beginLastSpan = endLastSpan + 1;
            }
            endLastSpan = dst.size() - 1;

            top = current->top;
            bottom = current->bottom;
        }
        int left = current->left;
        int right = current->right;

        for (unsigned int prevIndex = beginLastSpan; prevIndex <= endLastSpan; prevIndex++) {
            const Rect* prev = &dst[prevIndex];
            if (spanDirection == direction_RTL) {
                // iterating over previous span RTL, quit if it's too far left
                if (prev->right <= left) break;

                if (prev->right > left && prev->right < right) {
                    dst.push_back(Rect(prev->right, top, right, bottom));
                    right = prev->right;
                }

                if (prev->left > left && prev->left < right) {
                    dst.push_back(Rect(prev->left, top, right, bottom));
                    right = prev->left;
                }

                // if an entry in the previous span is too far right, nothing further left in the
                // current span will need it
                if (prev->left >= right) {
                    beginLastSpan = prevIndex;
                }
            } else {
                // iterating over previous span LTR, quit if it's too far right
                if (prev->left >= right) break;

                if (prev->left > left && prev->left < right) {
                    dst.push_back(Rect(left, top, prev->left, bottom));
                    left = prev->left;
                }

                if (prev->right > left && prev->right < right) {
                    dst.push_back(Rect(left, top, prev->right, bottom));
                    left = prev->right;
                }
                // if an entry in the previous span is too far left, nothing further right in the
                // current span will need it
                if (prev->right <= left) {
                    beginLastSpan = prevIndex;
                }
            }
        }

        if (left < right) {
            dst.push_back(Rect(left, top, right, bottom));
        }

        current--;
    }
}

/**
 * Creates a new region with the same data as the argument, but divides rectangles as necessary to
 * remove T-Junctions
 *
 * Note: the output will not necessarily be a very efficient representation of the region, since it
 * may be that a triangle-based approach would generate significantly simpler geometry
 */
Region Region::createTJunctionFreeRegion(const Region& r) {
    if (r.isEmpty()) return r;
    if (r.isRect()) return r;

    std::vector<Rect> reversed;
    reverseRectsResolvingJunctions(r.begin(), r.end(), reversed, direction_RTL);

    Region outputRegion;
    const Rect *begin = &(reversed.front());
    const Rect *end = &(reversed.back());
    reverseRectsResolvingJunctions(begin, end,
            outputRegion.mStorage, direction_LTR);
    outputRegion.mStorage.push_back(r.getBounds()); // to make region valid, mStorage must end with bounds

#if VALIDATE_REGIONS
    validate(outputRegion, "T-Junction free region");
#endif

    return outputRegion;
}

Region& Region::operator = (const Region& rhs)
{
#if VALIDATE_REGIONS
    validate(*this, "this->operator=");
    validate(rhs, "rhs.operator=");
#endif
    mStorage = rhs.mStorage;
    return *this;
}

Region& Region::makeBoundsSelf()
{
    if (mStorage.size() >= 2) {
        const Rect bounds(getBounds());
        mStorage.clear();
        mStorage.push_back(bounds);
    }
    return *this;
}

bool Region::contains(const Point& point) const {
    return contains(point.x, point.y);
}

bool Region::contains(int x, int y) const {
    const_iterator cur = begin();
    const_iterator const tail = end();
    while (cur != tail) {
        if (y >= cur->top && y < cur->bottom && x >= cur->left && x < cur->right) {
            return true;
        }
        cur++;
    }
    return false;
}

void Region::clear()
{
    mStorage.clear();
    mStorage.push_back(Rect(0,0));
}

void Region::set(const Rect& r)
{
    mStorage.clear();
    mStorage.push_back(r);
}

void Region::set(uint32_t w, uint32_t h)
{
    mStorage.clear();
    mStorage.push_back(Rect(w,h));
}

bool Region::isTriviallyEqual(const Region& region) const {
    return begin() == region.begin();
}

// ----------------------------------------------------------------------------

void Region::addRectUnchecked(int l, int t, int r, int b)
{
    Rect rect(l,t,r,b);
    //size_t where = mStorage.size() - 1;
    mStorage.push_back(rect);
}

// ----------------------------------------------------------------------------

Region& Region::orSelf(const Rect& r) {
    return operationSelf(r, op_or);
}
Region& Region::xorSelf(const Rect& r) {
    return operationSelf(r, op_xor);
}
Region& Region::andSelf(const Rect& r) {
    return operationSelf(r, op_and);
}
Region& Region::subtractSelf(const Rect& r) {
    return operationSelf(r, op_nand);
}
Region& Region::operationSelf(const Rect& r, int op) {
    Region lhs(*this);
    boolean_operation(op, *this, lhs, r);
    return *this;
}

// ----------------------------------------------------------------------------

Region& Region::orSelf(const Region& rhs) {
    return operationSelf(rhs, op_or);
}
Region& Region::xorSelf(const Region& rhs) {
    return operationSelf(rhs, op_xor);
}
Region& Region::andSelf(const Region& rhs) {
    return operationSelf(rhs, op_and);
}
Region& Region::subtractSelf(const Region& rhs) {
    return operationSelf(rhs, op_nand);
}
Region& Region::operationSelf(const Region& rhs, int op) {
    Region lhs(*this);
    boolean_operation(op, *this, lhs, rhs);
    return *this;
}

Region& Region::translateSelf(int x, int y) {
    if (x|y) translate(*this, x, y);
    return *this;
}

// ----------------------------------------------------------------------------

const Region Region::merge(const Rect& rhs) const {
    return operation(rhs, op_or);
}
const Region Region::mergeExclusive(const Rect& rhs) const {
    return operation(rhs, op_xor);
}
const Region Region::intersect(const Rect& rhs) const {
    return operation(rhs, op_and);
}
const Region Region::subtract(const Rect& rhs) const {
    return operation(rhs, op_nand);
}
const Region Region::operation(const Rect& rhs, int op) const {
    Region result;
    boolean_operation(op, result, *this, rhs);
    return result;
}

// ----------------------------------------------------------------------------

const Region Region::merge(const Region& rhs) const {
    return operation(rhs, op_or);
}
const Region Region::mergeExclusive(const Region& rhs) const {
    return operation(rhs, op_xor);
}
const Region Region::intersect(const Region& rhs) const {
    return operation(rhs, op_and);
}
const Region Region::subtract(const Region& rhs) const {
    return operation(rhs, op_nand);
}
const Region Region::operation(const Region& rhs, int op) const {
    Region result;
    boolean_operation(op, result, *this, rhs);
    return result;
}

const Region Region::translate(int x, int y) const {
    Region result;
    translate(result, *this, x, y);
    return result;
}

// ----------------------------------------------------------------------------

Region& Region::orSelf(const Region& rhs, int dx, int dy) {
    return operationSelf(rhs, dx, dy, op_or);
}
Region& Region::xorSelf(const Region& rhs, int dx, int dy) {
    return operationSelf(rhs, dx, dy, op_xor);
}
Region& Region::andSelf(const Region& rhs, int dx, int dy) {
    return operationSelf(rhs, dx, dy, op_and);
}
Region& Region::subtractSelf(const Region& rhs, int dx, int dy) {
    return operationSelf(rhs, dx, dy, op_nand);
}
Region& Region::operationSelf(const Region& rhs, int dx, int dy, int op) {
    Region lhs(*this);
    boolean_operation(op, *this, lhs, rhs, dx, dy);
    return *this;
}

// ----------------------------------------------------------------------------

const Region Region::merge(const Region& rhs, int dx, int dy) const {
    return operation(rhs, dx, dy, op_or);
}
const Region Region::mergeExclusive(const Region& rhs, int dx, int dy) const {
    return operation(rhs, dx, dy, op_xor);
}
const Region Region::intersect(const Region& rhs, int dx, int dy) const {
    return operation(rhs, dx, dy, op_and);
}
const Region Region::subtract(const Region& rhs, int dx, int dy) const {
    return operation(rhs, dx, dy, op_nand);
}
const Region Region::operation(const Region& rhs, int dx, int dy, int op) const {
    Region result;
    boolean_operation(op, result, *this, rhs, dx, dy);
    return result;
}

// ----------------------------------------------------------------------------

// This is our region rasterizer, which merges rects and spans together
// to obtain an optimal region.
class Region::rasterizer : public region_operator<Rect>::region_rasterizer 
{
    Rect bounds;
    std::vector<Rect>& storage;
    Rect* head;
    Rect* tail;
    std::vector<Rect> span;
    Rect* cur;
public:
    rasterizer(Region& reg) 
        : bounds(INT_MAX, 0, INT_MIN, 0), storage(reg.mStorage), head(), tail(), cur() {
        storage.clear();
    }

    ~rasterizer() {
        if (span.size()) {
            flushSpan();
        }
        if (storage.size()) {
            //bounds.top = storage.itemAt(0).top;
            //bounds.bottom = storage.top().bottom;
            if (storage.size() == 1) {
                storage.clear();
            }
        } else {
            bounds.left  = 0;
            bounds.right = 0;
        }
        storage.push_back(bounds);
    }
    
    virtual void operator()(const Rect& rect) {
        //ALOGD(">>> %3d, %3d, %3d, %3d",
        //        rect.left, rect.top, rect.right, rect.bottom);
        if (span.size()) {
            if (cur->top != rect.top) {
                flushSpan();
            } else if (cur->right == rect.left) {
                cur->right = rect.right;
                return;
            }
        }
        span.push_back(rect);
        cur = &(span.front()) + (span.size() - 1);
    }
private:
    template<typename T> 
    static inline T min(T rhs, T lhs) { return rhs < lhs ? rhs : lhs; }
    template<typename T> 
    static inline T max(T rhs, T lhs) { return rhs > lhs ? rhs : lhs; }
    void flushSpan() {
        bool merge = false;
        if (tail-head == ssize_t(span.size())) {
            Rect const* p = &(span.front());
            Rect const* q = head;
            if (p->top == q->bottom) {
                merge = true;
                while (q != tail) {
                    if ((p->left != q->left) || (p->right != q->right)) {
                        merge = false;
                        break;
                    }
                    p++, q++;
                }
            }
        }
        if (merge) {
            const int bottom = span[0].bottom;
            Rect* r = head;
            while (r != tail) {
                r->bottom = bottom;
                r++;
            }
        } else {
            //bounds.left = min(span.itemAt(0).left, bounds.left);
            //bounds.right = max(span.top().right, bounds.right);
            storage.insert(storage.end(),span.begin(),span.end());
            tail = (&storage.front()) + storage.size();
            head = tail - span.size();
        }
        span.clear();
    }
};

bool Region::validate(const Region& reg, const char* name, bool silent)
{
    bool result = true;
    const_iterator cur = reg.begin();
    const_iterator const tail = reg.end();
    const_iterator prev = cur;
    Rect b(*prev);
    while (cur != tail) {
        if (cur->isValid() == false) {
            ALOGE_IF(!silent, "%s: region contains an invalid Rect", name);
            result = false;
        }
        if (cur->right > region_operator<Rect>::max_value) {
            ALOGE_IF(!silent, "%s: rect->right > max_value", name);
            result = false;
        }
        if (cur->bottom > region_operator<Rect>::max_value) {
            ALOGE_IF(!silent, "%s: rect->right > max_value", name);
            result = false;
        }
        if (prev != cur) {
            b.left   = b.left   < cur->left   ? b.left   : cur->left;
            b.top    = b.top    < cur->top    ? b.top    : cur->top;
            b.right  = b.right  > cur->right  ? b.right  : cur->right;
            b.bottom = b.bottom > cur->bottom ? b.bottom : cur->bottom;
            if ((*prev < *cur) == false) {
                ALOGE_IF(!silent, "%s: region's Rects not sorted", name);
                result = false;
            }
            if (cur->top == prev->top) {
                if (cur->bottom != prev->bottom) {
                    ALOGE_IF(!silent, "%s: invalid span %p", name, cur);
                    result = false;
                } else if (cur->left < prev->right) {
                    ALOGE_IF(!silent,
                            "%s: spans overlap horizontally prev=%p, cur=%p",
                            name, prev, cur);
                    result = false;
                }
            } else if (cur->top < prev->bottom) {
                ALOGE_IF(!silent,
                        "%s: spans overlap vertically prev=%p, cur=%p",
                        name, prev, cur);
                result = false;
            }
            prev = cur;
        }
        cur++;
    }
    if (b != reg.getBounds()) {
        result = false;
        ALOGE_IF(!silent,
                "%s: invalid bounds [%d,%d,%d,%d] vs. [%d,%d,%d,%d]", name,
                b.left, b.top, b.right, b.bottom,
                reg.getBounds().left, reg.getBounds().top, 
                reg.getBounds().right, reg.getBounds().bottom);
    }
    if (reg.mStorage.size() == 2) {
        result = false;
        ALOGE_IF(!silent, "%s: mStorage size is 2, which is never valid", name);
    }
    if (result == false && !silent) {
        reg.dump(name);
        //CallStack stack(LOG_TAG);
    }
    return result;
}

void Region::boolean_operation(int op, Region& dst,
        const Region& lhs,
        const Region& rhs, int dx, int dy)
{
#if VALIDATE_REGIONS
    validate(lhs, "boolean_operation (before): lhs");
    validate(rhs, "boolean_operation (before): rhs");
    validate(dst, "boolean_operation (before): dst");
#endif

    size_t lhs_count;
    Rect const * const lhs_rects = lhs.getArray(&lhs_count);

    size_t rhs_count;
    Rect const * const rhs_rects = rhs.getArray(&rhs_count);

    region_operator<Rect>::region lhs_region(lhs_rects, lhs_count);
    region_operator<Rect>::region rhs_region(rhs_rects, rhs_count, dx, dy);
    region_operator<Rect> operation(op, lhs_region, rhs_region);
    { // scope for rasterizer (dtor has side effects)
        rasterizer r(dst);
        operation(r);
    }

#if VALIDATE_REGIONS
    validate(lhs, "boolean_operation: lhs");
    validate(rhs, "boolean_operation: rhs");
    validate(dst, "boolean_operation: dst");
#endif

#if VALIDATE_WITH_CORECG
    SkRegion sk_lhs;
    SkRegion sk_rhs;
    SkRegion sk_dst;
    
    for (size_t i=0 ; i<lhs_count ; i++)
        sk_lhs.op(
                lhs_rects[i].left   + dx,
                lhs_rects[i].top    + dy,
                lhs_rects[i].right  + dx,
                lhs_rects[i].bottom + dy,
                SkRegion::kUnion_Op);
    
    for (size_t i=0 ; i<rhs_count ; i++)
        sk_rhs.op(
                rhs_rects[i].left   + dx,
                rhs_rects[i].top    + dy,
                rhs_rects[i].right  + dx,
                rhs_rects[i].bottom + dy,
                SkRegion::kUnion_Op);
 
    const char* name = "---";
    SkRegion::Op sk_op;
    switch (op) {
        case op_or: sk_op = SkRegion::kUnion_Op; name="OR"; break;
        case op_xor: sk_op = SkRegion::kUnion_XOR; name="XOR"; break;
        case op_and: sk_op = SkRegion::kIntersect_Op; name="AND"; break;
        case op_nand: sk_op = SkRegion::kDifference_Op; name="NAND"; break;
    }
    sk_dst.op(sk_lhs, sk_rhs, sk_op);

    if (sk_dst.isEmpty() && dst.isEmpty())
        return;
    
    bool same = true;
    Region::const_iterator head = dst.begin();
    Region::const_iterator const tail = dst.end();
    SkRegion::Iterator it(sk_dst);
    while (!it.done()) {
        if (head != tail) {
            if (
                    head->left != it.rect().fLeft ||     
                    head->top != it.rect().fTop ||     
                    head->right != it.rect().fRight ||     
                    head->bottom != it.rect().fBottom
            ) {
                same = false;
                break;
            }
        } else {
            same = false;
            break;
        }
        head++;
        it.next();
    }
    
    if (head != tail) {
        same = false;
    }
    
    if(!same) {
        ALOGD("---\nregion boolean %s failed", name);
        lhs.dump("lhs");
        rhs.dump("rhs");
        dst.dump("dst");
        ALOGD("should be");
        SkRegion::Iterator it(sk_dst);
        while (!it.done()) {
            ALOGD("    [%3d, %3d, %3d, %3d]",
                it.rect().fLeft,
                it.rect().fTop,
                it.rect().fRight,
                it.rect().fBottom);
            it.next();
        }
    }
#endif
}

void Region::boolean_operation(int op, Region& dst,
        const Region& lhs,
        const Rect& rhs, int dx, int dy)
{
    if (!rhs.isValid()) {
        ALOGE("Region::boolean_operation(op=%d) invalid Rect={%d,%d,%d,%d}",
                op, rhs.left, rhs.top, rhs.right, rhs.bottom);
        return;
    }

#if VALIDATE_WITH_CORECG || VALIDATE_REGIONS
    boolean_operation(op, dst, lhs, Region(rhs), dx, dy);
#else
    size_t lhs_count;
    Rect const * const lhs_rects = lhs.getArray(&lhs_count);

    region_operator<Rect>::region lhs_region(lhs_rects, lhs_count);
    region_operator<Rect>::region rhs_region(&rhs, 1, dx, dy);
    region_operator<Rect> operation(op, lhs_region, rhs_region);
    { // scope for rasterizer (dtor has side effects)
        rasterizer r(dst);
        operation(r);
    }

#endif
}

void Region::boolean_operation(int op, Region& dst,
        const Region& lhs, const Region& rhs)
{
    boolean_operation(op, dst, lhs, rhs, 0, 0);
}

void Region::boolean_operation(int op, Region& dst,
        const Region& lhs, const Rect& rhs)
{
    boolean_operation(op, dst, lhs, rhs, 0, 0);
}

void Region::translate(Region& reg, int dx, int dy)
{
    if ((dx || dy) && !reg.isEmpty()) {
#if VALIDATE_REGIONS
        validate(reg, "translate (before)");
#endif
        size_t count = reg.mStorage.size();
        Rect* rects = &(reg.mStorage.front());
        while (count) {
            rects->offsetBy(dx, dy);
            rects++;
            count--;
        }
#if VALIDATE_REGIONS
        validate(reg, "translate (after)");
#endif
    }
}

void Region::translate(Region& dst, const Region& reg, int dx, int dy)
{
    dst = reg;
    translate(dst, dx, dy);
}

// ----------------------------------------------------------------------------

size_t Region::getFlattenedSize() const {
    return mStorage.size() * sizeof(Rect);
}

status_t Region::flatten(void* buffer, size_t size) const {
#if VALIDATE_REGIONS
    validate(*this, "Region::flatten");
#endif
    if (size < mStorage.size() * sizeof(Rect)) {
        return NO_MEMORY;
    }
    Rect* rects = reinterpret_cast<Rect*>(buffer);
    std::vector<Rect>::const_iterator begin;
    for (begin = mStorage.begin(); begin != mStorage.end(); begin++) {
        memcpy(rects, &(*begin), sizeof(Rect));
        rects++;
    }
    return NO_ERROR;
}

status_t Region::unflatten(void const* buffer, size_t size) {
    Region result;
    if (size >= sizeof(Rect)) {
        Rect const* rects = reinterpret_cast<Rect const*>(buffer);
        size_t count = size / sizeof(Rect);
        if (count > 0) {
            result.mStorage.clear();
            for (size_t c = 0; c < count; c++) {
              mStorage.push_back( *(rects + c));
            }
        }
    }
#if VALIDATE_REGIONS
    validate(result, "Region::unflatten");
#endif

    if (!result.validate(result, "Region::unflatten", true)) {
        ALOGE("Region::unflatten() failed, invalid region");
        return BAD_VALUE;
    }
    mStorage = result.mStorage;
    return NO_ERROR;
}

// ----------------------------------------------------------------------------

Region::const_iterator Region::begin() const {
    std::vector<Rect>::const_iterator it = mStorage.begin();
    return &(*it);
}

Region::const_iterator Region::end() const {
    size_t numRects = isRect() ? 1 : mStorage.size() - 1;
    if (numRects < 0) {
        return NULL;
    }
    std::vector<Rect>::const_iterator it = mStorage.end();
    it--;
    return &(*it);
}

Rect const* Region::getArray(size_t* count) const {
    const_iterator const b(begin());
    const_iterator const e(end());
    if (count) *count = e-b;
    return b;
}
#if 0
SharedBuffer const* Region::getSharedBuffer(size_t* count) const {
    // We can get to the SharedBuffer of a Vector<Rect> because Rect has
    // a trivial destructor.
    SharedBuffer const* sb = SharedBuffer::bufferFromData(mStorage.array());
    if (count) {
        size_t numRects = isRect() ? 1 : mStorage.size() - 1;
        count[0] = numRects;
    }
    if (sb != NULL) {
       sb->acquire();
    }
    return sb;
}
#endif

// ----------------------------------------------------------------------------

void Region::dump(std::string& out, const char* what, uint32_t flags) const
{
    (void)flags;
    const_iterator head = begin();
    const_iterator const tail = end();

    size_t SIZE = 256;
    char buffer[SIZE];

    snprintf(buffer, SIZE, "  Region %s (this=%p, count=%" PRIdPTR ")\n",
            what, this, tail-head);
    out.append(buffer);
    while (head != tail) {
        snprintf(buffer, SIZE, "    [%3d, %3d, %3d, %3d]\n",
                head->left, head->top, head->right, head->bottom);
        out.append(buffer);
        head++;
    }
}

void Region::dump(const char* what, uint32_t flags) const
{
    (void)flags;
    const_iterator head = begin();
    const_iterator const tail = end();
    ALOGD("  Region %s (this=%p, count=%" PRIdPTR ")\n", what, this, tail-head);
    while (head != tail) {
        ALOGD("    [%3d, %3d, %3d, %3d]\n",
                head->left, head->top, head->right, head->bottom);
        head++;
    }
}

// ----------------------------------------------------------------------------

}; // namespace android
