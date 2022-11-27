#include "states.hpp"

namespace protocol::states {
    namespace ___impl {
        struct {

        } ctx;
    }

    /**
    * State machine
    */
    using ProtocolSM = StateMachine<decltype(___impl::ctx)>;

    template class State<decltype(___impl::ctx)>;
    using ProtocolState = State<decltype(___impl::ctx)>;
    // ==================================================
    // States definitions
    // ==================================================

    class TestState : public ProtocolState {
        public:
            TestState(ProtocolSM* sm) : ProtocolState{sm} {}

            void on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };

}
