#pragma once
#include "json/json.h"
#include <string>
using namespace std;

Json::Value AddDecoration(int floor, string deco_tag);
Json::Value MoveDecorations(int floor, double angle_offset, string file_name, string deco_tag, string event_tag);
Json::Value RepeatEvents(int floor, string event_tag, double interval, int repetitions);