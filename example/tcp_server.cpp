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
    controller(condition_variable &server_has_been_shutdown):
        server_has_been_shutdown_(server_has_been_shutdown) {}
public:
    void did_accept(weak_ptr<server> s,
                    shared_ptr<connection> c,
                    error e) override {
        printf("[Accepted] %s\n", c->remote_endpoint().address().to_string().c_str());
        if (!s.expired()) {
            s.lock()->accept();
            s.lock()->send(c, "Welcome to mashiroLab!!!");
            s.lock()->receive(c, 100);
            std::basic_string<char> s;
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
        string message = d.get<char, 100>().data();
        printf("[Received] %s: %s\n", c->remote_endpoint().address().to_string().c_str(), message.c_str());
        if (message == "shutdown") {
            s.lock()->shutdown();
            return;
        }
        s.lock()->send(c, d);
        s.lock()->receive(c, 100);
    }
    virtual void did_run(std::weak_ptr<server> s, error e) {
        printf("[Running] %s\n", e.message().c_str());
        s.lock()->accept();
    }
    virtual void did_shutdown(std::weak_ptr<server> s, error e) {
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
