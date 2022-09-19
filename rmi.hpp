#ifndef RMI_HPP
#define RMI_HPP

#include <map>
#include <array>
#include <functional>
#include <stdint.h>

namespace protocol::rmi {
    // I'm sorry for whoever has to maintain this code. It shouldn't have been like this, but they forced my hand into creating this monstrosity
    constexpr uint8_t MAX_ARGS = 3;

    namespace ___impl {
        // Function type helpers
        template<typename ReturnType, typename ArgsType, std::size_t N, typename = std::make_index_sequence<N>>
        struct FuncTypeGenerator;

        template<typename T, size_t n>
        struct TypeWrapper {
            using type = T;
        };

        template<typename ReturnType, typename ArgsType, std::size_t N, std::size_t... S>
        struct FuncTypeGenerator<ReturnType, ArgsType, N, std::index_sequence<S...>> {
            using type = std::function<ReturnType(typename TypeWrapper<ArgsType, S>::type ...)>;
        };

        // Call shenanigans
        template<typename ReturnType, typename ArgsType, size_t... Indices>
        ReturnType ___call(std::index_sequence<Indices...>, void* f_ptr, ArgsType* args) {
            auto converted_f_ptr = reinterpret_cast<___impl::FuncTypeGenerator<ReturnType, ArgsType, sizeof...(Indices)>::type*>(f_ptr);
            return converted_f_ptr->operator()(args[Indices] ...);
        }

    }

    template<typename ReturnType, typename ArgsType>
    class FunctionContainer {
        public:
            template<typename... Args>
            void register_method(uint32_t hash, const std::function<ReturnType(Args...)>&& function) {
                std::array<uint8_t, sizeof(function)> func_buffer;
                new (func_buffer.data()) std::function<ReturnType(Args...)>(function);

                //m_registered_methods.emplace(hash, MethodWrapper(std::move(func_buffer), sizeof...(Args)));
                m_registered_methods.emplace(std::piecewise_construct, std::make_tuple(hash), std::make_tuple(std::move(func_buffer), sizeof...(Args)));
            }

            ReturnType call(uint32_t hash, ArgsType* args) {
                MethodWrapper& wrapper = m_registered_methods[hash];
                return caller_methods[wrapper.n_args](wrapper.function.data(), args);
            }
            
        private:
            struct MethodWrapper {
                std::array<uint8_t, sizeof(std::function<void()>)> function;
                uint8_t n_args;

                ~MethodWrapper() {
                    method_destructors[n_args](function.data());
                }
            };

            std::map<uint32_t, MethodWrapper> m_registered_methods;
            static std::array<ReturnType(*)(void*, ArgsType*), MAX_ARGS + 1> caller_methods;
            static std::array<void(*)(void*), MAX_ARGS + 1> method_destructors;
    };

    template<typename ReturnType, typename ArgsType>
    std::array<ReturnType(*)(void*, ArgsType*), MAX_ARGS + 1> FunctionContainer<ReturnType, ArgsType>::caller_methods = []<size_t... Indices>(std::index_sequence<Indices...>) -> std::array<ReturnType(*)(void* , ArgsType*), MAX_ARGS + 1> {
        return {
            [](void* f_ptr, ArgsType* args) {
                return ___impl::___call<ReturnType>(std::make_index_sequence<Indices>(), f_ptr, args);
            } ...
        };
    }(std::make_index_sequence<MAX_ARGS + 1>());

    template<typename ReturnType, typename ArgsType>
    std::array<void(*)(void*), MAX_ARGS + 1> FunctionContainer<ReturnType, ArgsType>::method_destructors = []<size_t... Indices>(std::index_sequence<Indices...>) -> std::array<void(*)(void*), MAX_ARGS + 1> {
        return {
            [](void* f_ptr) {
                using FType = ___impl::FuncTypeGenerator<ReturnType, ArgsType, Indices>::type;
                (*(reinterpret_cast<FType*>(f_ptr))).~FType();
            } ...
        };
    }(std::make_index_sequence<MAX_ARGS + 1>());


// End of pain
}

#endif
