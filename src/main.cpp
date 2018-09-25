#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


#include "AppManager.h"

bool withNvEnc = false;

int main(int argc, const char* argv[])
{

	boost::log::core::get()->set_filter (
			boost::log::trivial::severity >= boost::log::trivial::debug
			);


	if(argc == 2) {
		std::string key = argv[1];
		if(key.compare("+nvenc") == 0) {
			withNvEnc = true;
		}
	}

	BOOST_LOG_TRIVIAL(info) << " Simple Remote desktop server version 0.2";
	AppManager* app = new AppManager(withNvEnc);
	app->start();

}
