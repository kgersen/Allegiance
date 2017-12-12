#include <type_traits>
#include "event.h"

template <typename EntryType>
class UiList;

namespace sol {

    namespace stack {

        template <typename T>
        bool check_exact(lua_State* L, int index, record& tracking) {
            const type indextype = type_of(L, index);
            if (checker<detail::as_value_tag<T>, type::userdata>{}.check(types<T>(), L, index, indextype, sol::no_panic, tracking)) {
                return true;
            }
            return false;
        }

        //TStaticValue

        template <typename T>
        struct checker<TRef<TStaticValue<T>>, type::userdata> {
            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                if (getter<TRef<TStaticValue<T>>>::get(L, index, tracking)) {
                    return true;
                }

                handler(L, index, type::userdata, type_of(L, index), "value at this index does not properly reflect the desired type");
                return false;
            }
        };

        template <typename T>
        struct getter<TRef<TStaticValue<T>>> {
            static TRef<TStaticValue<T>> get(lua_State* L, int index, record& tracking) {
                if (check_exact<TRef<TStaticValue<T>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<TStaticValue<T>>>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<SimpleModifiableValue<T>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<SimpleModifiableValue<T>>>>{}.get(L, index, tracking);
                }
                if (sol::stack::check<T>(L, index, sol::no_panic, record())) {
                    return new TStaticValue<T>(sol::stack::get<T>(L, index, tracking));
                }
                return nullptr;
            }
        };

        template <>
        struct getter<TRef<TStaticValue<ZString>>> {
            typedef TRef<TStaticValue<ZString>> ReturnType;
            static ReturnType get(lua_State* L, int index, record& tracking) {
                if (check_exact<ReturnType>(L, index, record())) {
                    return getter<detail::as_value_tag<ReturnType>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<SimpleModifiableValue<ZString>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<SimpleModifiableValue<ZString>>>>{}.get(L, index, tracking);
                }
                if (sol::stack::check<std::string>(L, index, sol::no_panic, record())) {
                    return new TStaticValue<ZString>(sol::stack::get<std::string>(L, index, tracking).c_str());
                }
                return nullptr;
            }
        };

        // Sinks

        template <>
        struct checker<TRef<TEvent<bool>::Sink>, type::userdata> {
            typedef TRef<TEvent<bool>::Sink> ReturnType;
            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                if (getter<ReturnType>::get(L, index, tracking)) {
                    return true;
                }

                handler(L, index, type::userdata, type_of(L, index), "value at this index does not properly reflect the desired type");
                return false;
            }
        };

        template <>
        struct getter<TRef<TEvent<bool>::Sink>> {
            typedef TRef<TEvent<bool>::Sink> ReturnType;
            static ReturnType get(lua_State* L, int index, record& tracking) {
                if (check_exact<ReturnType>(L, index, record())) {
                    return getter<detail::as_value_tag<ReturnType>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<SimpleModifiableValue<bool>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<SimpleModifiableValue<bool>>>>{}.get(L, index, tracking);
                }
                return nullptr;
            }
        };

        template <>
        struct checker<TRef<TEvent<float>::Sink>, type::userdata> {
            typedef TRef<TEvent<float>::Sink> ReturnType;
            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                if (getter<ReturnType>::get(L, index, tracking)) {
                    return true;
                }

                handler(L, index, type::userdata, type_of(L, index), "value at this index does not properly reflect the desired type");
                return false;
            }
        };

        template <>
        struct getter<TRef<TEvent<float>::Sink>> {
            typedef TRef<TEvent<float>::Sink> ReturnType;
            static ReturnType get(lua_State* L, int index, record& tracking) {
                if (check_exact<ReturnType>(L, index, record())) {
                    return getter<detail::as_value_tag<ReturnType>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<SimpleModifiableValue<float>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<SimpleModifiableValue<float>>>>{}.get(L, index, tracking);
                }
                return nullptr;
            }
        };

        template <>
        struct checker<TRef<TEvent<ZString>::Sink>, type::userdata> {
            typedef TRef<TEvent<ZString>::Sink> ReturnType;
            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                if (getter<ReturnType>::get(L, index, tracking)) {
                    return true;
                }

                handler(L, index, type::userdata, type_of(L, index), "value at this index does not properly reflect the desired type");
                return false;
            }
        };

        template <>
        struct getter<TRef<TEvent<ZString>::Sink>> {
            typedef TRef<TEvent<ZString>::Sink> ReturnType;
            static ReturnType get(lua_State* L, int index, record& tracking) {
                if (check_exact<ReturnType>(L, index, record())) {
                    return getter<detail::as_value_tag<ReturnType>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<SimpleModifiableValue<ZString>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<SimpleModifiableValue<ZString>>>>{}.get(L, index, tracking);
                }
                return nullptr;
            }
        };

        template <>
        struct checker<TRef<TEvent<Point>::Sink>, type::userdata> {
            typedef TRef<TEvent<Point>::Sink> ReturnType;
            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                if (getter<ReturnType>::get(L, index, tracking)) {
                    return true;
                }

                handler(L, index, type::userdata, type_of(L, index), "value at this index does not properly reflect the desired type");
                return false;
            }
        };

        template <>
        struct getter<TRef<TEvent<Point>::Sink>> {
            typedef TRef<TEvent<Point>::Sink> ReturnType;
            static ReturnType get(lua_State* L, int index, record& tracking) {
                if (check_exact<ReturnType>(L, index, record())) {
                    return getter<detail::as_value_tag<ReturnType>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<SimpleModifiableValue<Point>>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<SimpleModifiableValue<Point>>>>{}.get(L, index, tracking);
                }
                return nullptr;
            }
        };

        //Image
        template <>
        struct checker<TRef<Image>> {
            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                if (getter<TRef<Image>>::get(L, index, tracking)) {
                    return true;
                }

                handler(L, index, type::userdata, type_of(L, index), "value at this index does not properly reflect the desired type");
                return false;
            }
        };

        template <>
        struct getter<TRef<Image>> {
            static TRef<Image> get(lua_State* L, int index, record& tracking) {
                if (check_exact<TRef<Image>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<Image>>>{}.get(L, index, tracking);
                }
                if (check_exact<TRef<ConstantImage>>(L, index, record())) {
                    return getter<detail::as_value_tag<TRef<ConstantImage>>>{}.get(L, index, tracking);
                }
                return nullptr;
            }
        };

        //list

        template <typename T>
        struct checker<TRef<UiList<TRef<T>>>, type::userdata> {
            typedef TRef<UiList<TRef<T>>> return_type;

            template <typename Handler>
            static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
                int absolute_index = lua_absindex(L, index);
                // Get the first element

                if (check_exact<return_type>(L, absolute_index, record())) {
                    tracking.use(1);
                    return true;
                }
                record inner_tracking = record();
                if (stack::check<table>(L, absolute_index, sol::no_panic, inner_tracking)) {
                    record inner_tracking = record();
                    table table_list = stack::get<table>(L, absolute_index, inner_tracking);

                    int count = table_list.size();

                    for (int i = 1; i <= count; ++i) {
                        sol::object entry = table_list.get<sol::object>(i);
                        if (entry.is<TRef<T>>() == false) {
                            return false;
                        }
                    }

                    tracking.use(inner_tracking.used);
                    return true;
                }
                handler(L, index, type::userdata, type::userdata, "Either table or UiList");
                return false;
            }
        };

        template <typename T>
        struct getter<TRef<UiList<TRef<T>>>> {
            typedef TRef<UiList<TRef<T>>> return_type;

            static return_type get(lua_State* L, int index, record& tracking) {
                int absolute_index = lua_absindex(L, index);

                if (check_exact<return_type>(L, absolute_index, record())) {
                    return getter<detail::as_value_tag<return_type>>{}.get(L, index, tracking);
                }
                if (stack::check<table>(L, absolute_index)) {
                    table table_list = stack::get<table>(L, absolute_index, tracking);

                    std::list<TRef<T>> list = {};

                    int count = table_list.size();

                    for (int i = 1; i <= count; ++i) {
                        auto entry = table_list.get<TRef<T>>(i);
                        list.push_back(entry);
                    }

                    return_type result = new UiList<TRef<T>>(list);
                    return result;
                }

                throw std::runtime_error("Unknown type to cast");
            }
        };

        /*
        template <typename T>
        struct getter<TRef<UiList<TRef<TStaticValue<T>>>>> {
            typedef TRef<UiList<TRef<TStaticValue<T>>>> return_type;

            static return_type get(lua_State* L, int index, record& tracking) {
                int absolute_index = lua_absindex(L, index);

                tracking.use(1);

                if (stack::check<return_type>(L, absolute_index)) {
                    return stack::get<return_type>(L, absolute_index);
                }
                if (stack::check<table>(L, absolute_index)) {
                    table table_list = stack::get<table>(L, absolute_index);

                    std::list<TRef<TStaticValue<T>>> list = {};

                    int count = table_list.size();

                    for (int i = 1; i <= count; ++i) {
                        auto entry = wrapValue<T>(table_list.get<sol::object>(i));
                        if (!entry) {
                            throw std::runtime_error("Entry should not be null");
                        }
                        list.push_back(entry);
                    }

                    return_type result = new UiList<TRef<TStaticValue<T>>>(list);
                    return result;
                }

                throw std::runtime_error("Unknown type to cast");
            }
        };*/
    }

}