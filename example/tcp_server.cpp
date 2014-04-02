#include <iostream>
#include <msr/network.hpp>

using namespace std;
using namespace msr::network;
using protocol::ip::tcp;

struct information {
    char name[10];
    uint16_t age;
};

class controller: public observer<server<tcp>> {
public:
    using self = controller;
    using base = observer;
public:
    controller(condition_variable &server_has_been_shutdown):
        server_has_been_shutdown_(server_has_been_shutdown) {}
public:
    void server_did_accept(server_ptr s, connection_ptr c, error e) override {
        if (e) {
            s.lock()->disconnect(c);
            return;
        }
        printf("[Accepted] %s\n", c->remote_endpoint().address().to_string().c_str());
        if (!s.expired()) {
            s.lock()->accept();
            s.lock()->send(c, "Welcome to mashiroLab!!!");
            s.lock()->receive(c, sizeof(information) / sizeof(data::unit_t));
        }
    }
    void server_did_send(server_ptr s, connection_ptr c, error e, data d) override {
        if (e) {
            s.lock()->disconnect(c);
            return;
        }
        if (d.size() == sizeof("Welcome to mashiroLab!!!")) {
            printf("[Sent] %s: %s\n", c->remote_endpoint().address().to_string().c_str(), d.get<char, 25>().data());
        } else {
            auto i = d.get<information>();
            printf("[Sent] %s: Name: %s, Age: %d\n", c->remote_endpoint().address().to_string().c_str(), i.name, i.age);
        }
    }
    void server_did_receive(server_ptr s, connection_ptr c, error e, data d) override {
        if (e) {
            s.lock()->disconnect(c);
            return;
        }
        auto i = d.get<information>();
        printf("[Received] %s: Name: %s, Age: %d\n", c->remote_endpoint().address().to_string().c_str(), i.name, i.age);
        s.lock()->send(c, i);
        s.lock()->receive(c, sizeof(information) / sizeof(data::unit_t));
    }
    void server_did_disconnect(server_ptr s, endpoint p, error e) override {
        printf("[Disconnected] %s\n", p.address().to_string().c_str());
    }
    void server_did_cancel(server_ptr s, connection_ptr c, error e) override {}
    void server_did_run(server_ptr s, error e) override {
        printf("[Running] %s\n", e.message().c_str());
        s.lock()->accept();
    }
    void server_did_shutdown(server_ptr s, error e) override {
        printf("[Shutdown] %s\n", e.message().c_str());
        server_has_been_shutdown_.notify_all();
    }
public:
    ~controller() {}
private:
    condition_variable &server_has_been_shutdown_;
};

int main(int argc, const char *argv[]) {
    mutex m;
    unique_lock<mutex> lock(m);
    condition_variable server_has_been_shutdown;
    auto tcp_server = make_shared<server<tcp>>(tcp::endpoint(tcp::v4(), 9999));
    tcp_server->create_observer<controller>(server_has_been_shutdown);
    tcp_server->run();
    server_has_been_shutdown.wait(lock);
    tcp_server = nullptr;
    return 0;
}
