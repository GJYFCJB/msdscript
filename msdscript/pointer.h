//
// Created by 谷金 on 4/24/22.
//

#ifndef CS6015PROJECT_POINTER_H
#define CS6015PROJECT_POINTER_H

#include <memory>

#define USE_PLAIN_POINTERS 0
#if USE_PLAIN_POINTERS

# define NEW(T)    new T
# define PTR(T)    T*
# define CAST(T)   dynamic_cast<T*>
# define CLASS(T)  class T
# define THIS     this

#else

# define NEW(T)    std::make_shared<T>
# define PTR(T)    std::shared_ptr<T>
# define CAST(T)   std::dynamic_pointer_cast<T>
# define CLASS(T) : public std::enable_shared_from_this<T>
# define THIS      shared_from_this()


#endif

#endif
