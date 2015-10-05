#include <iostream>
#include <msr/network.hpp>
#include <msr/memory.hpp>

using namespace msr::network;

struct information {
    char name[10];
    uint16_t age;
};

class controller: public tcp_ip_server_observer {
    MSR_CLASS_TYPE_DEFINATIONS(controller);
public:
    controller(std::condition_variable &server_has_been_shutdown):
        server_has_been_shutdown_(server_has_been_shutdown) {}
public:
    void server_did_accept(server::weak_ptr s, connection::shared_ptr c, error e) override {
        if (auto server_ = try_lock(s)) {
            if (e) {
                server_->disconnect(c);
                return;
            }
            printf("[Accepted] %s\n", c->remote_endpoint().address().to_string().c_str());
            server_->accept();
            server_->send(c, "Welcome to MsrLab!!!");
            server_->receive(c, sizeof(information));
        }
    }
    void server_did_send(server::weak_ptr s, connection::shared_ptr c, error e, data d) override {
        if (auto server_ = try_lock(s)) {
            if (e) {
                server_->disconnect(c);
                return;
            }
            if (d.size() == sizeof("Welcome to MsrLab!!!")) {
                printf("[Sent] %s: %s\n", c->remote_endpoint().address().to_string().c_str(), d.get<char, 21>().data());
            }
        }
    }
    void server_did_receive(server::weak_ptr s, connection::shared_ptr c, error e, data d) override {
        if (auto server_ = try_lock(s)) {
            if (e) {
                server_->disconnect(c);
                return;
            }
            auto i = d.get<information>();
            printf("[Received] %s: Name: %s, Age: %d\n", c->remote_endpoint().address().to_string().c_str(), i.name, i.age);
            server_->receive(c, sizeof(information));
        }
    }
    void server_did_disconnect(server::weak_ptr s, endpoint p, error e) override {
        printf("[Disconnected] %s\n", p.address().to_string().c_str());
    }
    void server_did_cancel(server::weak_ptr s, connection::shared_ptr c, error e) override {
        if (auto server_ = try_lock(s)) {
            if (e) {
                server_->disconnect(c);
                return;
            }
        }
    }
    void server_did_run(server::weak_ptr s, error e) override {
        if (auto server_ = try_lock(s)) {
            printf("[Running] %s\n", e.message().c_str());
            server_->accept();
        }
    }
    void server_did_shutdown(server::weak_ptr s, error e) override {
        printf("[Shutdown] %s\n", e.message().c_str());
        server_has_been_shutdown_.notify_all();
    }
public:
    ~controller() {}
private:
    std::condition_variable &server_has_been_shutdown_;
};

int main(int argc, const char *argv[]) {
    using namespace std;
    using tcp = protocol::ip::tcp;
    mutex m;
    unique_lock<mutex> lock(m);
    condition_variable server_has_been_shutdown;
    auto tcp_server = make_shared<tcp_ip_server>(tcp::endpoint(tcp::v4(), 9999));
    tcp_server->add_observer(make_shared<controller>(server_has_been_shutdown));
    tcp_server->run();
    server_has_been_shutdown.wait(lock);
    tcp_server = nullptr;
    return 0;
}
