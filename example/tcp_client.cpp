#include <iostream>
#include <msr/network.hpp>
#include <msr/memory.hpp>

struct information {
    char name[10] = "Darren";
    uint16_t age = 19;
};

using namespace std;
using namespace msr;
using namespace network;
using protocol::ip::tcp;

class controller: public observer<client<tcp>> {
public:
    using self = controller;
    using base = observer;
public:
    controller(condition_variable &client_has_been_shutdown):
        client_has_been_shutdown_(client_has_been_shutdown) {}
public:
    void client_did_connect(client_weak_ptr c, connection_ptr n, error e) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
            printf("[Connected] %s\n", n->remote_endpoint().address().to_string().c_str());
            client_->receive(n, 25);
        }
    }
    void client_did_send(client_weak_ptr c, connection_ptr n, error e, data d) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
            auto i = d.get<information>();
            printf("[Sent] %s: Name: %s, Age: %d\n", n->remote_endpoint().address().to_string().c_str(), i.name, i.age);
            client_->shutdown();
        }
        
    }
    void client_did_receive(client_weak_ptr c, connection_ptr n, error e, data d) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
            client_->send(n, information());
        }
    }
    void client_did_disconnect(client_weak_ptr c, endpoint p, error e) override {
        printf("[Disconnected] %s\n", p.address().to_string().c_str());
    }
    void client_did_cancel(client_weak_ptr c, connection_ptr n, error e) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
        }
    }
    void client_did_run(client_weak_ptr c) override {
        if (auto client_ = try_lock(c)) {
            printf("[Running]\n");
            client_->connect("localhost", 9999);
        }
    }
    void client_did_shutdown(client_weak_ptr c) override {
        client_has_been_shutdown_.notify_all();
    }
public:
    ~controller() {}
private:
    condition_variable &client_has_been_shutdown_;
};

int main(int argc, const char *argv[]) {
    mutex m;
    unique_lock<mutex> lock(m);
    condition_variable client_has_been_shutdown;
    auto s = make_shared<client<tcp>>();
    s->create_observer<controller>(client_has_been_shutdown);
    s->run();
    client_has_been_shutdown.wait(lock);
}