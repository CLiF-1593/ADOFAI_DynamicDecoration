#pragma once
#include "json/json.h"

enum FileType { ADOFAI, VIDEO };

void ReadJson(const std::string& file, Json::Value& root);
void WriteJson(const Json::Value& value, std::string location);
std::string OpenFileName(FileType type);