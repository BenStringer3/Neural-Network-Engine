#pragma once

#include <boost/asio.hpp>
#include "Matrix.h"


class NNE_Helper {

public:
	//NNE_Helper(boost::asio::io_service& io_service);
	NNE_Helper(boost::asio::io_service & io_service, short port);
	~NNE_Helper();
	void printMat(const Matrix & mat, std::string name);
	void printMat(const float & scalar, std::string name);
	//void printMat(const double & mat, std::string name);
	//void printMat(const Matrix & mat);

	
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;
	//boost::asio::ip::tcp::socket & socket();

	void newBatch();
	unsigned int batchNum;
private:
#pragma pack(push, 1)
	typedef struct MatrixSendMsg {
		char m;
		uint32_t rows;
		uint32_t cols;
		uint32_t id;
		uint32_t batchNum;
		uint32_t nameSize;
		MatrixSendMsg() {
			m = 'm';
		}
	};
#pragma pack(pop)
	
	unsigned int indices;
	MatrixSendMsg matSndMsg[1];
	//boost::asio::io_service& io_service_;
	
};

//extern NNE_Helper NNE_Helper;