#include <thread>
#include <vector>
#include <cstdarg>

using namespace std

// Constructor for thread group
threadGroup::threadGroup() {
	// Nothing here?
}

// Adds a task thread to the vector 
template <typename... Args> int threadGroup::addTask(bool active, Args... args)
{

	// Add a task to the end of the vector
	threads.push_back(thread(args...));

	// Add active flag to end of vector
	isActive.push_back(active);

	// Return thread id (same as thread size)
	int currentSize = threads.size();
	
	if (currentSize == isActive.size()) {
		return currentSize;
	}

	else {
		return -1;
	}
}

// Removes a task thread from the vector, computationally inefficient
int threadGroup::delTask(int taskId) {

	// Remove task from vector
	threads.erase(taskID);
	
	// Remove task's active flag from active vector
	isActive.erase(taskID);

	return 0;
}

// Toggles execution of a task without removing it from vector of tasks
// this saves time in time-sensitive operations
int threadGroup::toggleTask(int taskId, bool active) {

	// Set task's active flag
	bool *p = isActive.data();
	p + (taskId - 1) = active;

	return 0;

}

// Run all threads in the group
int threadGroup::run() {

	// Declare pointers to each vector
	bool* p = isActive.data();
	thread* tp = threads.data();

	// go through vector and join all threads that are active
	for (int i = 0; i < threads.size(); i++) {
		if (p + i) {
			tp->join();
		}
	}

}

// should be a destructor?
int threadGroup::delGroup() {
	// Deletes entire thread group


}