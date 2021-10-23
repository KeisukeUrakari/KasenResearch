#include <functional>
#include <gtest/gtest.h>
#include <iostream>

class IFTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
    }

    // virtual void TearDown() {}
};

class IF {
  public:
    virtual int req(int a) = 0;
};

class IF_impl : public IF {
  public:
    int req(int a) override {
        return a * 2;
    }
};

class IF2 : public IF {
  public:
    virtual int req2(int b) = 0;
};

class IF2_impl : public IF_impl, public IF2 {
  public:
    int req(int a) override {
        return IF_impl::req(a);
    }

    int req2(int b) override {
        return b * 3;
    }
};

TEST_F(IFTest, test) {
    IF *impl = new IF_impl();
    ASSERT_EQ(2, impl->req(1));

    IF2 *impl2 = new IF2_impl();
    ASSERT_EQ(3, impl2->req2(1));
}

class Model {
  public:
    int value() const { return 1; }
};

class IPanel {
  public:
    virtual void kido(int no) = 0;
};

class ImplIF {
  public:
    virtual void tick(int elapse) = 0;
};

class IScreen {
  public:
    virtual void dispDone() = 0;
    using drawfunc_t = std::function<void(int time, const std::string &image)>;
    virtual void drawfunc(drawfunc_t func) = 0;
};

class Ctrl : public IPanel, public IScreen {
  public:
    Ctrl(Model &model) : model_(model) {}

    void kido(int no) override {
        std::cout << "kido " << no << std::endl;
    }

    void dispDone() override {
        std::cout << "dispdone " << std::endl;
    }

    void drawfunc(drawfunc_t func) override {
        drawFunc_ = func;
    }

  private:
    Model &model_;
    drawfunc_t drawFunc_;
};

class Panel : public ImplIF {
  public:
    Panel(IPanel &panelIF) : panelIF_(panelIF) {
    }

    void tick(int elapse) override {
        panelIF_.kido(5);
    }

  private:
    IPanel &panelIF_;
};

class Screen : public ImplIF {
  public:
    Screen(IScreen &interface) : interface_(interface) {
        interface_.drawfunc([](int time, const std::string &image) {
            std::cout << "draw " << image << std::endl;
        });
    }

    void tick(int elapse) override {
        interface_.dispDone();
    }

  private:
    IScreen &interface_;
};

TEST_F(IFTest, test2) {
    auto model = Model();
    auto ctrl = Ctrl(model);
    auto panel = Panel(ctrl);
    panel.tick(10);
}

class ITelecom {
  public:
    virtual void open() = 0;
    using sendfunc_t = std::function<void(const std::vector<uint8_t> &)>;
    virtual void sendCallback(sendfunc_t callback) = 0;
};

class TelecomModel : public Model {
  public:
    int value2() const { return 2; }
};

class TelecomCtrl : public Ctrl, public ITelecom {
  public:
    TelecomCtrl(TelecomModel &model) : Ctrl(model), model_(model) {
    }
    void open() override {
        std::cout << "open" << std::endl;
    }

    void sendCallback(ITelecom::sendfunc_t callback) override {
        sendFunc_ = callback;
    }

  protected:
    ITelecom::sendfunc_t sendFunc_;

  private:
    TelecomModel &model_;
};

class Telecom : public ImplIF {
  public:
    Telecom(ITelecom &telecomIF) : telecomIF_(telecomIF) {
        telecomIF_.sendCallback([](const std::vector<uint8_t> &data) {
            std::cout << "send " << data.size() << std::endl;
        });
    }

    void tick(int elapse) override {
        telecomIF_.open();
    }

  private:
    ITelecom &telecomIF_;
};

TEST_F(IFTest, test3) {
    auto model = TelecomModel();
    auto ctrl = TelecomCtrl(model);
    auto panel = Panel(ctrl);
    auto telecom = Telecom(ctrl);

    panel.tick(10);
    telecom.tick(5);
}

class IKasenPanel : public IPanel {
  public:
    virtual void lamp(void) = 0;
};

class Kasen : public TelecomCtrl, public IKasenPanel {
  public:
    Kasen(TelecomModel &model) : TelecomCtrl(model), model_(model) {}

    void lamp(void) override {
        std::cout << "lamp " << model_.value() << " " << model_.value2() << std::endl;
    }

    void kido(int no) override {
        TelecomCtrl::kido(no);
        sendFunc_(std::vector<uint8_t>(10));
    }

  private:
    TelecomModel &model_;
};

class KasenPanel : public Panel {
  public:
    KasenPanel(IKasenPanel &interface)
        : Panel(interface), interface_(interface) {
    }

  public:
    void tick(int elapse) override {
        Panel::tick(elapse);
        interface_.lamp();
    }

  private:
    IKasenPanel &interface_;
};

template <typename M>
class IObserver {
  public:
    virtual void show(const M &model) = 0;
};

template <typename M>
class Observer : public IObserver<M> {
  public:
    virtual void show(const M &model) override {
        std::cout << "show " << model.value() << std::endl;
    }
};

template <typename M>
class TelecomObserver : public IObserver<M> {
  public:
    virtual void show(const M &model) override {
        std::cout << "show " << model.value2() << std::endl;
    }
};

TEST_F(IFTest, test4) {
    auto model = TelecomModel();
    auto ctrl = Kasen(model);
    auto panel = std::make_shared<KasenPanel>(ctrl);
    auto telecom = std::make_shared<Telecom>(ctrl);
    auto screen = std::make_shared<Screen>(ctrl);

    auto obs = std::make_shared<Observer<TelecomModel>>();
    auto tobs = std::make_shared<TelecomObserver<TelecomModel>>();
    auto observers = std::vector<std::shared_ptr<IObserver<TelecomModel>>>({obs, tobs});
 
    auto impls = std::vector<std::shared_ptr<ImplIF>>({panel, telecom, screen});
    for(auto i : impls) {
        i->tick(1);
    }

    for(auto o : observers) {
        o->show(model);
    }
}
