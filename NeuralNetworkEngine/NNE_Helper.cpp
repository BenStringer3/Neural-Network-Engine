#include "stdafx.h"
#include "NNE_Helper.h"


//using namespace NNE_Helper;
//NNE_Helper NNE_helper;
boost::asio::io_service io_service;
NNE_Helper NNE_helper(io_service, 4012);

#pragma pack(push, 1)
typedef struct MatrixSendMsg {
	char m;
	uint32_t rows;
	uint32_t cols;
	uint32_t id;
	uint32_t batchNum;
	MatrixSendMsg() {
		m = 'm';
	}
};
#pragma pack(pop)


NNE_Helper::NNE_Helper(boost::asio::io_service& io_service, short port)
	: socket_(io_service),
	acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{

	//boost::asio::io_service io_service;
	//acceptor = boost::asio::ip::tcp::acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4012));
	//socket = boost::asio::ip::tcp::socket (io_service);

	batchNum = 0;
	indices = 1;

}

NNE_Helper::~NNE_Helper()
{
}

void NNE_Helper::printMat( const Matrix & mat)
{
	static MatrixSendMsg matSndMsg[1];

	matSndMsg->id = indices;
	matSndMsg->batchNum = batchNum;
	matSndMsg->rows = mat.rows;
	matSndMsg->cols = mat.cols;

	write(socket_, boost::asio::buffer(matSndMsg, sizeof(MatrixSendMsg)));
	write(socket_, boost::asio::buffer(mat.data.data(), mat.data.size() * sizeof(double)));

	indices++;
}



//boost::asio::ip::tcp::socket & NNE_Helper::socket()
//{
//	return socket_;
//}

void NNE_Helper::newBatch()
{
	batchNum++;
	indices = 1;
}
