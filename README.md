# echo_server
an echo server base on boost asio.

It's just a demo show how to use boost asio.



# used awesome feature  from boost.asio and c++1x

1、use boost::spawn to create coroutines .

2、passed lamda to the coroutine handles.

3、show how coroutines  worked

4、show how std::enable_shared_from_this and shared_from_this used

5、test the case without coroutine ，use default lamda callback

6、show the basic asio method and structure in the demo ，ip::tcp::endpoint， ip::tcp::socket， async_accept， async_receive， async_send
asio::buffer etc.



# coroutines


Use boost::spawn to create stackfull coroutines


# Requirements

boost 1.64

