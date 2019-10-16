/*
 * threadManager.h
 * buddyEngine threading
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#ifndef _THREAD_MANAGER_H
#define _THREAD_MANAGER_H

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

class threadGroup {

public:

	// Constructor for thread group
	threadGroup(void);
	
	template<typename... Args> inline int addTask(Args... args) {
		
		// Add a task to the end of the vector
		threads.push_back(thread(args...));

		// Add active flag to end of vector
		isActive.push_back(true);

		// Return thread id (same as thread size for my purposes)
		int currentSize = threads.size();

		// Make sure dimensions match
		if (currentSize == isActive.size()) {
			return currentSize;
		}

		else {
			return -1;
		}
	}

	// Removes a task thread from the vector, computationally inefficient
	//int delTask(int taskId);

	// Toggles execution of a task without removing it from vector of tasks
	// this saves time
	int toggleTask(int taskId, bool active);

	// iterate through the assigned tasks
	int run(void);

	// Delete entire thread group
	//int delGroup();

private:

	// Vectors which hold threads and their active flags
	vector<thread>	threads;
	vector<bool>	isActive;
};

#endif