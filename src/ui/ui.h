#pragma once

#ifndef __MODULE__
#define __MODULE__ "Ui"
#endif

#include "soundengine.h"
#include "model.h"
#include "image.h"
#include "engine.h"

#define SOL_CHECK_ARGUMENTS 1

#include "sol.hpp"

namespace sol {
    template <typename T>
    struct unique_usertype_traits<TRef<T>> {
        typedef T type;
        typedef TRef<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& value) {
            return value == nullptr;
        }

        static type* get(const actual_type& p) {
            T* result = p;
            return result;
        }
    };
}

#include "UiEngine.h"