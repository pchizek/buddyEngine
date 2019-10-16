/*
 * threadManager.cpp
 * buddyEngine threading
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include <thread>
#include <vector>
#include <cstdarg>
#include <iostream>

#include "threadManager.h"

using namespace std;

// Constructor for thread group
threadGroup::threadGroup(void) {
	// Nothing here?

	cout << "Group formed\n";
}


// Removes a task thread from the vector, computationally inefficient
/*
int threadGroup::delTask(int taskId) {

	// Remove task from vector
	threads.erase(threads.begin+(taskId-1));
	
	// Remove task's active flag from active vector
	isActive.erase(isActive.begin+(taskId-1));

	return 0;
}
*/
// Toggles execution of a task without removing it from vector of tasks
// this saves time in time-sensitive operations
int threadGroup::toggleTask(int taskId, bool active) {

	// Set task's active flag
	isActive.at(taskId) = active;

	return 0;

}

// Run all threads in the group
int threadGroup::run(void) {

	// Declare pointers to each vector
	thread* tp = threads.data();

	// go through vector and join all threads that are active
	for (unsigned int i = 0; i < threads.size(); i++) {
		if (isActive.at(i)) {
			tp->join();
		}
		tp++;
	}

	return 0;

}
 