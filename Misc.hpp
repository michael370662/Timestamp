#ifndef __PonyTech_FND_Base_Misc_hpp__
#define __PonyTech_FND_Base_Misc_hpp__

#include "../trait/AddTraits.hpp"
#include "../trait/SizetTraits.hpp"

BEGIN_NAMESPACE(FND)

class Misc 
{
    FND_STATIC_CLASS(Misc)
public:
    
    template<class T>   inline static constexpr typename AddRvalueRef<T>::Type declval() noexcept;
    // move and forward
    template <class T>
    inline static constexpr 
        typename RemoveReference<T>::Type && move(T && arg) noexcept { return static_cast<typename RemoveReference<T>::Type&&>(arg); }
    template <class T>inline static constexpr 
        T&& forward(typename RemoveReference<T>::Type& arg) noexcept { return static_cast<T &&>(arg); }
    template <class T> inline static constexpr 
        T&& forward(typename RemoveReference<T>::Type&& arg) noexcept { return static_cast<T &&>(arg); }

    // max and min
    template<typename T> static T maximum(T v1, T v2) {return v1 < v2 ? v2: v1;}
    template<typename T> static T minimum(T v1, T v2) {return v1 < v2 ? v1: v2;}

    // return the position that is the most minimum in the list
    template<typename T>  static size_t maximum(const T* list, size_t count);
    template<typename T>  static size_t minimum(const T* list, size_t count);

    // fix array count
    template<typename T, size_t N> static size_t count_of(T(&)[N]) {return N;}

    // alignment
    constexpr static size_t align_with(size_t size, size_t align) 
    {
        return (size + align - 1) & (~(align-1));
    } 
}; 


// ===== template ==========
template<typename T> 
size_t Misc::maximum(const T* list, size_t count)
{
    if (count == 0) return SizetTraits::not_valid;
    size_t pos = 0;
    for(size_t i=1; i<count; i++) 
    {
        if (list[i] <= list[pos]) continue;
        pos = i;
    }
    return pos;
}

template<typename T> 
size_t Misc::minimum(const T* list, size_t count)
{
    if (count == 0) return SizetTraits::not_valid;
    size_t pos = 0;
    for(size_t i=1; i<count; i++) 
    {
        if (list[i] >= list[pos]) continue;
        pos = i;
    }
    return pos;
}

END_NAMESPACE(FND)

#endif // __PonyTech_FND_Base_Misc_hpp__