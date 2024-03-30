#pragma once

#include <mutex>
#include <condition_variable>

std::mutex tool_mutex;
std::condition_variable tool_cv;
std::mutex mtx;
std::condition_variable cv;

void performTask(int robotId);
