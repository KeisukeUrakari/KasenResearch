#include <boost/log/trivial.hpp>
#include <gtest/gtest.h>

#include <variant>

class VariantTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

template <typename Context, typename Param>
class AbstractState {
  public:
    virtual void onEvent(Context &context, const std::string &name, const Param &param) = 0;
};

template <typename Context, typename Param>
class AbstractDispatcer {
  public:
    AbstractDispatcer(AbstractState<Context, Param> *initState) : currentState_(initState) {
    }

    void notifyEvent(Context &context, const std::string &name, const Param &param) {
        currentState_->onEvent(context, name, param);
    }

  private:
    std::unique_ptr<AbstractState<Context, Param>> currentState_;
};

class Param1 {
  public:
    int value1 = 1;
};

class Param2 {
  public:
    int value2 = 2;
};

class Param3 {
  public:
    int value3 = 3;
};

using param_t = std::variant<std::monostate, Param1, Param2, Param3>;

class Ctrl;

class State : public AbstractState<Ctrl, param_t> {
  public:
    void onEvent(Ctrl &context, const std::string &name, const param_t &param);
};

class Dispatcher : public AbstractDispatcer<Ctrl, param_t> {
    using base = AbstractDispatcer<Ctrl, param_t>;

  public:
    Dispatcher(State *initState) : base(initState) {
    }
};

class Ctrl {
  public:
    Ctrl() : disp(new State()) {
    }

    int value = -1;
    Dispatcher disp;
    void tick() {
        if(cnt < 1) {
            disp.notifyEvent(*this, "", Param1());
        } else {
            disp.notifyEvent(*this, "123", Param2());
        }
        cnt++;
    }
    int cnt = 0;
};

void State::onEvent(Ctrl &context, const std::string &name, const param_t &param) {
//    BOOST_LOG_TRIVIAL(trace) << context.value;

    if(name == "") {
        ASSERT_TRUE(std::holds_alternative<Param1>(param));
        const auto p = std::get<Param1>(param);
        ASSERT_EQ(1, p.value1);
    } else if(name == "123") {
        ASSERT_TRUE(std::holds_alternative<Param2>(param));
        const auto p = std::get<Param2>(param);
        ASSERT_EQ(2, p.value2);
    }
}

TEST_F(VariantTest, test) {
    auto ctrl = Ctrl();
    ctrl.tick();
    ctrl.tick();

    FAIL();
}
