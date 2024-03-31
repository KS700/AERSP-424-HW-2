#include "Question2.h"
#include <iostream>
#include <chrono>
#include <thread>

// Global Variables
int available_tools = 5;
int current_robot = 1;
int robots_finished = 0;
std::chrono::steady_clock::time_point start_time;

// Function to perform the task by each robot
void performTask(int robotId) {
    std::unique_lock<std::mutex> lock(tool_mutex);
    
    // Wait until it's the current robot's turn to acquire tools
    while ((robotId % 2 != 0 && current_robot != 1) ||
        (robotId % 2 == 0 && current_robot != 2)) {
        tool_cv.wait(lock);
    }

    // Wait until there are at least 2 available tools
    while (available_tools < 2) {
        tool_cv.wait(lock);
    }

    // Acquire 2 tools
    available_tools -= 2;
    std::cout << "Robot " << robotId << " acquired tools and starts performing a task." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lock.unlock();

    // Simulate task execution time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Return the tools
    lock.lock();
    available_tools += 2;
    std::cout << "Robot " << robotId << " finished the task and returned the tools." << std::endl;
    lock.unlock();
    tool_cv.notify_all();

    // Update the current robot
    if (robotId % 2 == 0) {
        current_robot = 1;
    }
    else {
        current_robot = 2;
    }
    tool_cv.notify_all();
    std::lock_guard<std::mutex> counter_lock(tool_mutex);
    robots_finished++;
}

int main () {
std::cout << "############### Question 2 ###############" << std::endl;
    // Start and Record time
    std::chrono::steady_clock::time_point start_time;
    start_time = std::chrono::steady_clock::now();

    // Create a thread for each robot
    std::thread robots[5];
    for (int i = 0; i < 5; ++i) {
        robots[i] = std::thread(performTask, i + 1);
    }

    // Join the threads
    for (int i = 0; i < 5; ++i) {
        robots[i].join();
    }

    // Stop recording the time and calculate the duration for every robot to finish their task
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    std::cout << "Total duration: " << duration << " seconds" << std::endl;
std::cout << "############# End Question 2 #############\n" << std::endl;

    return 0;
}