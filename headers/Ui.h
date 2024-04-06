#ifndef FIRST_PROJECT_DA_UI_H
#define FIRST_PROJECT_DA_UI_H

#include "Manager.h"
#include <chrono>

#ifdef __linux__
# define CLEAR (void)system("clear")
#else
# define CLEAR (void)system("cls")
#endif

class UI {
	private:
		long loadtime;
		Manager manager;
	public:
		UI(bool useSmallSet);

		void mainMenu();
		// Basic Service
		void maxFlowMenu();
		void meetDemandsMenu();
		void balancedFlowMenu();

		// Utils
		void testMenu();
		static bool strFind(const std::string &one, const std::string &two);
		void helpMsg(std::string error, std::string usage);
};

#endif
