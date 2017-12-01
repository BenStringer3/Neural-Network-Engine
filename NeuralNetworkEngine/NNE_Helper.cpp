#include "stdafx.h"
#include "NNE_Helper.h"


//using namespace NNE_Helper;
//NNE_Helper NNE_helper;
boost::asio::io_service io_service;
NNE_Helper NNE_helper(io_service, 4012);

//#pragma pack(push, 1)
//typedef struct MatrixSendMsg {
//	char m;
//	uint32_t rows;
//	uint32_t cols;
//	uint32_t id;
//	uint32_t batchNum;
//	uint32_t nameSize;
//	MatrixSendMsg() {
//		m = 'm';
//	}
//};
//#pragma pack(pop)


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

void NNE_Helper::printMat( const Matrix & mat, std::string name)
{
	//vector<double> tmp2 = mat.data
	vector<float> tmp(mat.data.begin(), mat.data.end());
	matSndMsg->id = indices;
	matSndMsg->batchNum = batchNum;
	matSndMsg->rows = mat.rows;
	matSndMsg->cols = mat.cols;
	matSndMsg->nameSize = name.size();

	write(socket_, boost::asio::buffer(matSndMsg, sizeof(MatrixSendMsg)));
	write(socket_, boost::asio::buffer(&name[0], name.size()));
	write(socket_, boost::asio::buffer(tmp, tmp.size() * sizeof(float)));

	indices++;
}

void NNE_Helper::printMat(const float & scalar, std::string name)
{
	static MatrixSendMsg matSndMsg[1];

	matSndMsg->id = indices;
	matSndMsg->batchNum = batchNum;
	matSndMsg->rows = 1;
	matSndMsg->cols = 1;
	matSndMsg->nameSize = name.size();

	write(socket_, boost::asio::buffer(matSndMsg, sizeof(MatrixSendMsg)));
	write(socket_, boost::asio::buffer(&name[0], name.size()));
	write(socket_, boost::asio::buffer(&scalar,  sizeof(float)));

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
