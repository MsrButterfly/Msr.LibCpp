#include <iostream>
#include <msr/network.hpp>
#include <msr/memory.hpp>

struct information {
    char name[10] = "Airi";
    uint16_t age = 18;
};

using namespace msr::network;

class controller: public tcp_ip_client_observer {
public:
    controller(std::condition_variable &client_has_been_shutdown):
        client_has_been_shutdown_(client_has_been_shutdown) {}
public:
    void client_did_connect(client::weak_ptr c, connection::shared_ptr n, error e) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
            printf("[Connected] %s\n", n->remote_endpoint().address().to_string().c_str());
            client_->receive(n, 21);
        }
    }
    void client_did_send(client::weak_ptr c, connection::shared_ptr n, error e, data d) override {
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
    void client_did_receive(client::weak_ptr c, connection::shared_ptr n, error e, data d) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
            client_->send(n, information());
        }
    }
    void client_did_disconnect(client::weak_ptr c, endpoint p, error e) override {
        printf("[Disconnected] %s\n", p.address().to_string().c_str());
    }
    void client_did_cancel(client::weak_ptr c, connection::shared_ptr n, error e) override {
        if (auto client_ = try_lock(c)) {
            if (e) {
                client_->disconnect(n);
                return;
            }
        }
    }
    void client_did_run(client::weak_ptr c) override {
        if (auto client_ = try_lock(c)) {
            printf("[Running]\n");
            client_->connect("localhost", 9999);
        }
    }
    void client_did_shutdown(client::weak_ptr c) override {
        client_has_been_shutdown_.notify_all();
    }
public:
    ~controller() {}
private:
    std::condition_variable &client_has_been_shutdown_;
};

int main(int argc, const char *argv[]) {
    using namespace std;
    mutex m;
    unique_lock<mutex> lock(m);
    condition_variable client_has_been_shutdown;
    auto s = make_shared<tcp_ip_client>();
    s->add_observer(make_shared<controller>(client_has_been_shutdown));
    s->run();
    client_has_been_shutdown.wait(lock);
}
