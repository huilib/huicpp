

namespace HUICPP{

namespace detail {


template<typename _Ty>
template<typename tag>
struct LazySingletonHolder<_Ty>::Impl: private LazySingletonHolder<_Ty> {
    Impl() 
        : LazySingletonHolder<_Ty> ({typeid(_Ty), typeid(tag)}) { }
};


template<typename _Ty>
template<typename tag>
inline LazySingletonHolder<_Ty>&  LazySingletonHolder<_Ty>::Singleton() {
    return *static_cast<LazySingletonHolder<_Ty>*>(static_singleton_manager::Instance().Create<Impl<tag> >());
}


template<typename _Ty>
typename LazySingletonHolder<_Ty>::pointer_type LazySingletonHolder<_Ty>::Get() {

    if (HLIKELY(m_state.load(std::memory_order_acquire) == SingletonHolderState::Living)) {
        return m_instance;
    }

    create_instance();

    return m_instance;

}



template<typename _Ty>
LazySingletonHolder<_Ty>::LazySingletonHolder(const TypeDescriptor& td) noexcept 
    : m_type_desc(td), m_mutex(), 
    m_state(LazySingletonHolder<_Ty>::SingletonHolderState::Dead), 
    m_instance(nullptr) { }



template<typename _Ty>
void LazySingletonHolder<_Ty>::create_instance() {    

    std::lock_guard<std::mutex> lg(m_mutex);
    if (m_state.load(std::memory_order_acquire) == SingletonHolderState::Living) {
        return ;
    }

    pointer_type pointer = new value_type();
    CHECK_NEWPOINT(pointer);
    m_instance = pointer;

    m_state.store(SingletonHolderState::Living, std::memory_order_release);

}

}

}
