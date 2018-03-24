#include "stdafx.h"
#include "test.Logger.h"
#include "Logger.h"

int test_Logger(){
	printf("\n>>>>>>>>>>> %s Test: %s <<<<<<<<<<<\n\n", "Start", "Logger");

	Logger* logger = Logger::getLogger();
	logger->setFileName("test.logger.txt");
	logger->setLevel(DEBUG_LOG);
	logger->setPrintScreen(true);

	logger->debug("Logger test.");
	logger->info("Logger test.");
	logger->warning("Logger test.");
	logger->error("Logger test.");
	logger->fatal("Logger test.");

	logger->closeLogger();
	
	printf("\n>>>>>>>>>>> %s Test: %s <<<<<<<<<<<\n\n", "End", "Logger");

	return 0;
}