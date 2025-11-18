


#include "rollfixedbuffer.h"

namespace HUICPP {


BufferItemRef::BufferItemRef(HN iGroupId, HN index, HN iSize, HPTR ptr) noexcept
    : m_iGroupId(iGroupId), m_index(index), m_iSize(iSize), m_ptr(ptr), 
    m_buffer(nullptr), m_datalen(0) {

}


BufferItemRef::~ BufferItemRef() noexcept {

}


HN BufferItemRef::Write(HCPTR pdata, HN len) {

    HASSERT_THROW(len < m_iSize, INVL_PARA);

    memcpy(m_ptr, pdata, len);

    SetLen(len);

    return len;

}


void BufferItemRef::PullBack() {

    HASSERT_THROW(m_buffer != nullptr, SRC_ERR);

    m_buffer->PullBack(this);

}



RollFixBuffer::RollFixBuffer(HN iGroupCount, HN GroupItemCount, HN ItemSize) 
    : m_iGroupCount(iGroupCount), m_iGroupItemCount(GroupItemCount), m_iItemSize(ItemSize),
    m_data(nullptr), m_queues(), m_all_ref_pts() {

    // 分配内存.
    m_data = malloc(m_iGroupCount * m_iGroupItemCount * m_iItemSize);
    CHECK_NEWPOINT(m_data);

    // 创建队列
    for (HN i = 0; i < m_iGroupCount; ++i) {

        RefQueue* ref_q = new RefQueue();
        CHECK_NEWPOINT(ref_q);

        // 创建内存块引用
        for (HN j = 0; j < m_iGroupItemCount; ++j) {

            HPTR data_ptr = get_mem_item(i, j);

            BufferRefPtr item_ptr = new BufferItemRef(i, j, m_iItemSize, data_ptr);

            ref_q->Push(item_ptr);
            m_all_ref_pts.push_back(item_ptr);

        }

        m_queues.push_back(ref_q);


    }

}


RollFixBuffer::~RollFixBuffer() {

    /*
        那里申请, 那里释放. 
        后申请, 先释放.
    */

    for (std::vector<BufferRefPtr>::size_type i = 0; i < m_all_ref_pts.size(); ++i) {
        BufferRefPtr ptr = m_all_ref_pts[i];
        HDELP(ptr);
    }
    m_all_ref_pts.clear();    

    for (queues_t::size_type i = 0; i < m_queues.size(); ++i) {
        RefQueue* qptr = m_queues[i];
        HDELP(qptr);
    }
    m_queues.clear();

    HFREE(m_data);

}


void RollFixBuffer::PullBack(BufferRefPtr ptr) {

    HASSERT_THROW(ptr != nullptr, INVL_PARA);

    RefQueue* queue_ptr = m_queues[ptr->GetGroupId()];

    queue_ptr->Push(ptr);

}


BufferRefPtr RollFixBuffer::Pop(HN iGroupId) {

    HASSERT_THROW(iGroupId < m_iGroupCount, INVL_PARA);

    RefQueue* queue_ptr = m_queues[iGroupId];

    BufferRefPtr res = queue_ptr->Pop();
    res->SetBuffer(this);

    return res;

}


HPTR RollFixBuffer::get_mem_item(HN iGroupId, HN index) {

    HASSERT_THROW(iGroupId < m_iGroupCount, INVL_PARA);

    HASSERT_THROW(index < m_iGroupItemCount, INVL_PARA);

    HASSERT_THROW(m_data != nullptr, SRC_ERR);

    HSZ pos = static_cast<HSZ>(m_data);

    pos += iGroupId * m_iGroupItemCount * m_iItemSize + index * m_iItemSize;

    return pos;
}


HCPTR RollFixBuffer::get_mem_item(HN iGroupId, HN index) const {
    
    HASSERT_THROW(iGroupId < m_iGroupCount, INVL_PARA);

    HASSERT_THROW(index < m_iGroupItemCount, INVL_PARA);

    HASSERT_THROW(m_data != nullptr, SRC_ERR);

    HCSZ pos = static_cast<HCSZ>(m_data);

    pos += iGroupId * m_iGroupItemCount * m_iItemSize + index * m_iItemSize;

    return pos;

}


RollFixBuffer::RefQueue::RefQueue() 
    : m_locker(), m_queue() {

}


RollFixBuffer::RefQueue::~RefQueue() {

}


void RollFixBuffer::RefQueue::Push(BufferRefPtr ptr) {

    {
        // 增加数据时, 先锁住队列
        HSCOPE_MUTEX(m_locker);
        m_queue.push(ptr);
    }

    // 广播信号
    HNOTOK_THROW(m_locker.Signal());

}


BufferRefPtr RollFixBuffer::RefQueue::Pop() {

    HSCOPE_MUTEX(m_locker);

    while(m_queue.empty()) {

        HNOTOK_THROW(m_locker.Wait());

    }

    // 安全判断, 防止平台差异性
    if (m_queue.empty()) {
        return nullptr;
    }

    BufferRefPtr res = m_queue.front();

    // 删除第一个
    m_queue.pop();

    return res;

}


}


