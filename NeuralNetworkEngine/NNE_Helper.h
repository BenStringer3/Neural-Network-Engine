#pragma once

#include <boost/asio.hpp>
#include "Matrix.h"


class NNE_Helper {

public:
	//NNE_Helper(boost::asio::io_service& io_service);
	NNE_Helper(boost::asio::io_service & io_service, short port);
	~NNE_Helper();
	void printMat(const Matrix & mat, std::string name);
	//void printMat(const Matrix & mat);

	
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;
	//boost::asio::ip::tcp::socket & socket();

	void newBatch();
private:
	unsigned int batchNum;
	unsigned int indices;
	//boost::asio::io_service& io_service_;
	
};

//extern NNE_Helper NNE_Helper;