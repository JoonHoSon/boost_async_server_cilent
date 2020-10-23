#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

//
// https://elecs.tistory.com/314 참고
// 

using boost::asio::ip::tcp;
using namespace std;

class Session {
    private:
        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];

        void handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
            if (!error) {
                cout << data_ << endl;
            } else {
                delete this;
            }
        }

    public:
        Session(boost::asio::io_service &io_service):socket_(io_service) {

        }

        tcp::socket &socket() {
            return socket_;
        }

        void start() {
            cout << "connected" << endl;
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&Session::handle_read,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred)
            );
        }

};

class Server {
    private:
        boost::asio::io_service &io_service_;
        tcp::acceptor acceptor_;

        void start_accept() {
            Session *new_session = new Session(io_service_);
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&Server::handle_accept,
                                               this,
                                               new_session,
                                               boost::asio::placeholders::error)
            );
        }

        void handle_accept(Session *new_session, const boost::system::error_code &error) {
            if (!error) {
                new_session->start();
            } else {
                delete new_session;
            }

            start_accept();
        }

    public:
        Server(boost::asio::io_service &io_service, short port):io_service_(io_service),acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
            start_accept();
        }
};

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            cerr << "Usage: boost_server <port>\n";

            return 1;
        }

        boost::asio::io_service io_service;
        Server s(io_service, atoi(argv[1]));
        io_service.run();
    } catch (exception &e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
