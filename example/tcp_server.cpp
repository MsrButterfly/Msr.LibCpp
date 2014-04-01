#include <iostream>
#include <msr/network.hpp>

using namespace std;
using namespace msr::network;
using protocol::ip::tcp;

class controller: public observer<server<tcp>> {
public:
    using self = controller;
    using base = observer;
public:
    controller() {}
public:
    void did_accept(weak_ptr<server> s,
                    shared_ptr<connection> c,
                    error e) override {
        printf("[Accepted] %s\n", c->remote_endpoint().address().to_string().c_str());
        if (!s.expired()) {
            try {
                 s.lock()->accept();
                s.lock()->send(c, "Welcome to mashiroLab!!!");
                s.lock()->receive(c, 100);
            } catch (...) {}
        }
    }
    void did_send(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e, data d) {
        if (d.size() == 25) {
            printf("[Sent] %s: %s\n", c->remote_endpoint().address().to_string().c_str(), d.get<char, 25>().data());
        } else {
            printf("[Sent] %s: %s\n", c->remote_endpoint().address().to_string().c_str(), d.get<char, 100>().data());
        }
    }
    void did_receive(std::weak_ptr<server> s, std::shared_ptr<connection> c, error e, data d) {
        printf("[Received] %s: %s\n", c->remote_endpoint().address().to_string().c_str(), d.get<char, 100>().data());
        s.lock()->send(c, d);
        s.lock()->receive(c, 100);
    }
public:
    ~controller() {}
};

int main(int argc, const char *argv[]) {
    auto tcp_server = make_shared<server<tcp>>(tcp::endpoint(tcp::v4(), 9999));
    tcp_server->create_observer<controller>();
    tcp_server->accept();
    getchar();
    return 0;
}
