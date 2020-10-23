#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

//
// https://elecs.tistory.com/314 참고
//

using boost::asio::ip::tcp;
using namespace std;

enum { max_length = 1024};

class Client {
    private:
        tcp::socket socket_;
        char data[max_length];

    public:
        Client(boost::asio::io_service &io_service,
               const string &host,
               const string &port):socket_(io_service) {
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::ip::tcp::resolver::query query(host, port);
            boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            boost::asio::async_connect(socket_,
                                       endpoint_iterator,
                                       boost::bind(&Client::handle_connect,
                                                   this,
                                                   boost::asio::placeholders::error)
            );
        }

        void handle_connect(const boost::system::error_code &e) {
            if (!e) {
                cout << "Connected!" << endl;
                string msg = "Hello! Server!";

                boost::asio::async_write(socket_,
                                         boost::asio::buffer(msg, msg.length()),
                                         boost::bind(&Client::handle_write,
                                                     this,
                                                     boost::asio::placeholders::error)
                );
            }
        }

        void handle_write(const boost::system::error_code &e) {
            if (!e) {
                cout << "Done!" << endl;
            }
        }
};

int main(int argc, char *argv[]) {
    try {
        if (argc != 3) {
            cerr << "Usage: boost_client <host> <port>" << endl;

            return 1;
        }

        boost::asio::io_service io_service;
        Client c(io_service, argv[1], argv[2]);

        io_service.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
