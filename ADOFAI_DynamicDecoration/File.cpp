#include <iostream>
#include <fstream>
#include <atlstr.h>
#include <shlobj_core.h>
#include <filesystem>
#include "json/json.h"
#include "File.h"

#include "ProtableFileDialogs.h"
#define _HAS_STD_BYTE 0
using namespace std;

void ReadJson(const std::string& file, Json::Value& root) {
	std::ifstream config_doc(filesystem::path(filesystem::u8path(file)), std::ifstream::binary);
	if (!config_doc.is_open()) {
		cout << endl;
		cout << "!! Error : Cannot open the file. !!" << endl;
		cout << "Details : " << "File not found" << endl;
		cout << "맵을 읽어오는 과정에서 문제가 발생하였습니다. (There was a problem reading the map.)" << endl;
		cout << "위 오류 위치를 참고하여 맵 파일을 직접 수정해주세요. (Please correct the map file directly by referring to the error location above.)" << endl << endl;
		system("pause");
		exit(-1);
	}
	//check for UTF-8 BOM and remove if needed
	char b[4] = { 0, 0, 0, 0 };
	char bom[] = { (char)239, (char)187, (char)191, 0 };
	config_doc.read(b, 3);
	if (strcmp(b, bom) != 0) {
		// If no UTF-8 BOM is found reposition stream
		config_doc.seekg(0, std::ios::beg);
	}
	std::string errs;
	Json::CharReaderBuilder* reader = new Json::CharReaderBuilder();
	bool res = Json::parseFromStream(*reader, config_doc, &root, &errs);
	if (!res) {
		cout << endl;
		cout << "!! Error : Cannot open the file. !!" << endl;
		cout << "Details : " << errs << endl;
		cout << "맵을 읽어오는 과정에서 문제가 발생하였습니다. (There was a problem reading the map.)" << endl;
		cout << "위 오류 위치를 참고하여 맵 파일을 직접 수정해주세요. (Please correct the map file directly by referring to the error location above.)" << endl << endl;
		system("pause");
		exit(-1);
	}
}

void WriteJson(const Json::Value& value, string location) {
	Json::StreamWriterBuilder builder;
	builder.settings_["emitUTF8"] = true;
	builder.settings_["indentation"] = " ";
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream outputFileStream(filesystem::path(filesystem::u8path(location)));
	char bom[] = { (char)239, (char)187, (char)191 };
	outputFileStream.write(bom, 3);
	writer->write(value, &outputFileStream);
}

string OpenFileName(FileType type) {
	vector<string> selection;
	switch (type) {
	case ADOFAI:
		selection = pfd::open_file("Open File", "", { "ADOFAI Map File(*.adofai)", "*.adofai" }, pfd::opt::force_path).result();
		break;
	case VIDEO:
		selection = pfd::open_file("Open File", "", { "Video File(*.mp4)", "*.mp4" }, pfd::opt::force_path).result();
		break;
	}
	if (selection.empty()) {
		return "";
	}
	return selection.front();
}

string OpenFileNameADOFAI() {
	HDC hdc;
	PAINTSTRUCT ps;
	OPENFILENAME OFN;
	wchar_t lpstrFile[MAX_PATH] = L"";
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = L"ADOFAI Map File(*.adofai)\0*.adofai\0";
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = NULL;
	OFN.nMaxFile = 256;
	OFN.nFilterIndex = 1;
	OFN.lpstrFileTitle = NULL;
	OFN.nMaxFileTitle = 0;
	OFN.lpstrInitialDir = NULL;
	OFN.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	string str = "";
	str.clear();
	if (GetOpenFileName(&OFN) != 0)
		str = CW2A(OFN.lpstrFile);
	return str;
}