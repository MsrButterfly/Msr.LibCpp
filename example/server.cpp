//
//  main.cpp
//  msr-cpp-libraries-debug
//
//  Created by Darren Liu on 14-3-20.
//  Copyright (c) 2014年 msrlab.org. All rights reserved.
//

#include <iostream>
#include <msr/network.hpp>

using namespace std;
using namespace msr::network;
using namespace boost::asio;
using ip::tcp;

class controller: public observer<server<tcp>> {
public:
    using self = controller;
    using base = observer;
public:
    controller() {}
public:
    void did_accept(weak_ptr<server> s,
                    shared_ptr<server::connection> c,
                    error_code error) {
        printf("Accepted: %s", c->remote_endpoint().address().to_string().c_str());
        if (!s.expired()) {
            shared_ptr<server> _s;
            try {
                _s = s.lock();
            } catch (...) {
                _s = nullptr;
            }
            if (_s) {
                _s -> accept();
            }
        }
    }
    void did_send(weak_ptr<server> s,
                  shared_ptr<server::connection> c,
                  error_code error,
                  size_t size) {
        
    }
    void did_receive(weak_ptr<server> s,
                     shared_ptr<server::connection> c,
                     error_code error,
                     size_t size) {
        
    }
public:
    ~controller() {}
};

int main(int argc, const char *argv[]) {
    auto tcp_server = make_shared<server<tcp>>(tcp::endpoint(tcp::v4(), 10000));
    tcp_server->create_observer<controller>();
    tcp_server->accept();
    getchar();
    return 0;
}
