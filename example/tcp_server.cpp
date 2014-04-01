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
        printf("Accepted: %s\n", c->remote_endpoint().address().to_string().c_str());
        if (!s.expired()) {
            try {
                s.lock()->accept();
            } catch (...) {}
        }
    }
    void did_send(weak_ptr<server> s,
                  shared_ptr<connection> c,
                  error e,
                  size_t size) override {
        
    }
    void did_receive(weak_ptr<server> s,
                     shared_ptr<connection> c,
                     error e,
                     size_t size) override {
        
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
