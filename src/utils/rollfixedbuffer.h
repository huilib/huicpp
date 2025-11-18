

#ifndef __H_UTIL_ROLLFIXEDBUFFER_H__
#define __H_UTIL_ROLLFIXEDBUFFER_H__

#include <queue>
#include "../huicpp.h"
#include "../hmutex.h"


namespace HUICPP {


class RollFixBuffer;

class BufferItemRef {
public:
    /*
        构造函数. 
        在构造函数中, 我们不给m_buffer赋值, 因为RollFixBuffer可能还没有构造完成. 此时指定this指针可能会有未知异常.
        在pop引用内存时, 指定buffer.
    */
    BufferItemRef(HN iGroupId, HN index, HN iSize, HPTR ptr) noexcept;

    ~ BufferItemRef() noexcept;

public:
    HN GetGroupId() const noexcept { return m_iGroupId; }

    HN GetIndex() const noexcept { return m_index; }

    HPTR GetData() noexcept {  return m_ptr; }

    HCPTR GetData() const noexcept {  return m_ptr; }

    void SetBuffer(RollFixBuffer* rfb) noexcept { m_buffer = rfb; }

    void SetLen(HN len) noexcept { m_datalen = len; }

    HN GetDataLen() const noexcept { return m_datalen; }

public:
    HN Write(HCPTR pdata, HN len);

    /*
        告知RollFixedBuffer, 内存引用释放;
    */
    void PullBack();

private:
    const HN m_iGroupId;
    const HN m_index;
    const HN m_iSize;
    HPTR m_ptr;
    RollFixBuffer* m_buffer;
    HN m_datalen;
};


using BufferRefPtr = BufferItemRef*;



class RollFixBuffer {
public:
    /*
        构造函数. 
        构造函数会分配内存, 和队列.
    */
    RollFixBuffer(HN iGroupCount, HN GroupItemCount, HN ItemSize);

    /*
        释放内存和队列.
    */
    ~ RollFixBuffer();


public:
    void PullBack(BufferRefPtr ptr);

    BufferRefPtr Pop(HN iGroupId);

private:
    HPTR get_mem_item(HN iGroupId, HN index);

    HCPTR get_mem_item(HN iGroupId, HN index) const;

private:
    /*
        内存引用的FIFO队列.
    */
    class RefQueue {
    private:
        using ref_queue_t = std::queue<BufferRefPtr>;
    
    public:
        RefQueue();

        ~ RefQueue();

    public:
        /*
            释放引用. 
        */
        void Push(BufferRefPtr ptr);

        /*
            引用内存.
        */
        BufferRefPtr Pop();    

    private:
        mutable HConLock m_locker;
        ref_queue_t m_queue;
    };
    using queues_t = std::vector<RefQueue*>;

private:
    const HN m_iGroupCount;
    const HN m_iGroupItemCount;
    const HN m_iItemSize;
    HPTR m_data;
    queues_t m_queues;    

    /*
        所有引用的指针, 由buffer统一释放.
    */
    std::vector<BufferRefPtr> m_all_ref_pts;
};


}

#endif // __H_UTIL_ROLLFIXEDBUFFER_H__



