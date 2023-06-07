#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <vector>
#include <filesystem>
#include "color.h"
#include "File.h"
#include "ImageProcessing.h"
#include "EventJson.h"
using namespace std;

int main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	cout << "[ ADOFAI Dynamic Decoration Creator - 얼불춤 동적 장식 제작 도구 ]" << endl << endl;
	cout << "Developer : CLiF (https://www.youtube.com/@clif-1593)" << endl;
	cout << "Version : v1.3" << endl << endl;
	int parts;

	cout << "이 프로그램은 움직이는 장식을 쉽게 만들도록 도와드리기 위해 제작되었습니다." << endl;
	cout << "This program was developed to help you to make dynamic decoration easier." << endl << endl;
	cout << "오류를 발견하시거나 추가 기능을 요청하시고 싶으시다면 디스코드 DM 부탁드립니다. (CLiF [클리프] #8404)" << endl;
	cout << "If you find an error or want to request an additional function, please contact by Discord DM." << endl << endl;
	cout << "또한 장식이나 태그 이름 등은 무조건 영어 혹은 기본적인 특수문자만 사용해주세요. (아스키 코드 1byte로 표현되는 문자)" << endl << endl;
	system("pause");
	cout << endl << endl;

	cout << "< Choose the Video File - 영상 파일 선택 >" << endl << endl;
	string video_path = OpenFileName(VIDEO);
	if (video_path.empty())
		return 0;
	cout << "> Path : " << video_path << endl << endl << endl;
	int frame_num = GetVideoFrame(video_path);

	cout << "< Choose the ADOFAI File - 얼불춤 파일 선택 >" << endl;
	string adofai_path = OpenFileName(ADOFAI);
	if (adofai_path.empty())
		return 0;
	string folder_path = adofai_path;
	while (folder_path.back() != '\\') {
		folder_path.pop_back();
	}

	Json::Value adofai;
	ReadJson(adofai_path, adofai);

	if (!adofai.isMember("decorations")) {
		cout << "파일 버전이 낮습니다. Map version is too low." << endl;
		cout << "이 프로그램은 \'decorations\' 항목을 지원하는 맵에 대해서 사용 가능합니다." << endl;
		cout << "This program is available for maps that support \'decorations\'." << endl;
		system("pause");
		exit(0);
	}

	int total_tile_num = 0;
	if (adofai.isMember("angleData")) {
		total_tile_num = adofai["angleData"].size() + 1;
	}
	else {
		total_tile_num = adofai["pathData"].size() + 1;
	}

	cout << endl << endl;
	cout << "< Decoration Setting - 장식 설정 >" << endl << endl;
	ERR_FRAME_NUM:
	cout << "> Number of Frames - 목표 프레임 수 (?/" << frame_num <<") : ";
	int frame;
	cin >> frame;
	if (frame > frame_num || frame <= 0 || cin.fail()) {
		cin.clear();
		cin.ignore(100000, '\n');
		cout << "> The number of Frames is out of the range. - 잘못된 입력 (범위 넘어감)" << endl;
		goto ERR_FRAME_NUM;
	}
	cout << endl;

	ERR_DECO_NUM:
	cout << "> Number of Decorations - 장식 개수 : ";
	int deco_num;
	cin >> deco_num;
	if (deco_num <= 0 || cin.fail()) {
		cin.clear();
		cin.ignore(100000, '\n');
		cout << "> The Number of Decorations is out of the range. - 잘못된 입력 (범위 넘어감)" << endl;
		goto ERR_DECO_NUM;
	}
	cout << endl;

	cin.ignore();
	string univ_tag = "";
	if (deco_num != 1) {
	ERR_UNIV_TAG:
		cout << "> Universal Decoration Tag - 범용 장식 태그 : ";
		getline(cin, univ_tag);
		if (univ_tag.empty()) {
			cout << "> Decoration Tag is empty. - 태그값이 비어있음" << endl;
			goto ERR_UNIV_TAG;
		}
		if (find(univ_tag.begin(), univ_tag.end(), ' ') != univ_tag.end()) {
			cout << "> Decoration Tag should not include space. - 태그는 공백이 없어야함" << endl;
			goto ERR_UNIV_TAG;
		}
		univ_tag += " ";
		cout << endl;
	}

	vector<string> deco_tag;
	for (int i = 0; i < deco_num; i++) {
		ERR_TAG:
		if(deco_num == 1)
			cout << "> Decoration Tag - 장식 태그 : ";
		else if(i == 0)
			cout << "> 1st Decoration Tag - 1번 장식 태그 : ";
		else if (i == 1)
			cout << "> 2nd Decoration Tag - 2번 장식 태그 : ";
		else if (i == 2)
			cout << "> 3rd Decoration Tag - 3번 장식 태그 : ";
		else
			cout << "> " << i + 1 << "th Decoration Tag - " << i + 1 << "번 장식 태그 : ";
		string tag;
		getline(cin, tag);
		if (tag.empty()) {
			cout << "> Decoration Tag is empty. - 태그값이 비어있음" << endl;
			goto ERR_TAG;
		}
		if (find(tag.begin(), tag.end(), ' ') != tag.end()) {
			cout << "> Decoration Tag should not include space. - 태그는 공백이 없어야함" << endl;
			goto ERR_TAG;
		}
		if (find(deco_tag.begin(), deco_tag.end(), tag) != deco_tag.end() && tag != univ_tag) {
			cout << "> Decoration Tag should be unique. - 태그는 중복될 수 없음" << endl;
			goto ERR_TAG;
		}
		deco_tag.push_back(tag);
		cout << endl;
	}

	ERR_FILENAME:
	cout << "> Decoration File Name - 장식 파일 이름 : ";
	string deco_file_name;
	getline(cin, deco_file_name);
	deco_file_name = "[Dynamic Decoration] " + deco_file_name;
	if (deco_file_name.empty()) {
		cout << "> File Name is empty. - 파일 이름이 비어있음" << endl;
		goto ERR_FILENAME;
	}
	cout << " └  File Name Format : " << deco_file_name << "\\(Frame_Number).png" << endl;
	cout << endl;

	vector<int> tile_num;
	for (int i = 0; i < deco_num; i++) {
		ERR_TILE_NUM:
		if (deco_num == 1)
			cout << "> Decoration Begin Tile Number - 장식 시작 타일 번호 : ";
		else if (i == 0)
			cout << "> 1st Decoration Begin Tile Number - 1번 장식 시작 타일 번호 : ";
		else if (i == 1)
			cout << "> 2nd Decoration Begin Tile Number - 2번 장식 시작 타일 번호 : ";
		else if (i == 2)
			cout << "> 3rd Decoration Begin Tile Number - 3번 장식 시작 타일 번호 : ";
		else
			cout << "> " << i + 1 << "th Decoration Begin Tile Number - " << i + 1 << "번 장식 시작 타일 번호 : ";
		int num;
		cin >> num;
		if (num < 0 || num > total_tile_num || cin.fail()) {
			cin.clear();
			cin.ignore(100000, '\n');
			cout << "> Decoration Begin Tile Number is out of the range. - 잘못된 입력 (범위 넘어감)" << endl;
			goto ERR_TILE_NUM;
		}
		tile_num.push_back(num);
		cout << endl;
	}

	vector<int> begin_offset;
	for (int i = 0; i < deco_num; i++) {
		ERR_BEGIN_OFFSET:
		if (deco_num == 1)
			cout << "> Decoration Beginning Angle Offset - 장식 시작 각도 오프셋 : ";
		else if (i == 0)
			cout << "> 1st Decoration Beginning Angle Offset - 1번 장식 시작 각도 오프셋 : ";
		else if (i == 1)
			cout << "> 2nd Decoration Beginning Angle Offset - 2번 장식 시작 각도 오프셋 : ";
		else if (i == 2)
			cout << "> 3rd Decoration Beginning Angle Offset - 3번 장식 시작 각도 오프셋 : ";
		else
			cout << "> " << i + 1 << "th Decoration Beginning Angle Offset - " << i + 1 << "번 장식 시작 각도 오프셋 : ";
		int num;
		cin >> num;
		if (num < 0 || cin.fail()) {
			cin.clear();
			cin.ignore(100000, '\n');
			cout << "> Angle Offset is out of the range. - 잘못된 입력 (범위 넘어감)" << endl;
			goto ERR_BEGIN_OFFSET;
		}
		begin_offset.push_back(num);
		cout << endl;
	}

	ERR_OFFSET:
	cout << "> Animation Angle Offset - 장식 애니메이션 각도 오프셋 : ";
	double angle_offset;
	cin >> angle_offset;
	if (angle_offset <= 0 || cin.fail()) {
		cin.clear();
		cin.ignore(100000, '\n');
		cout << "> Angle Offset is out of the range. - 잘못된 입력 (범위 넘어감)" << endl;
		goto ERR_OFFSET;
	}
	cout << endl;

	ERR_LOOP:
	cout << "> Number of Repetitions - 반복 횟수 : ";
	double loop;
	cin >> loop;
	if (loop < 0 || cin.fail()) {
		cin.clear();
		cin.ignore(100000, '\n');
		cout << "> The Number of Repetitions is out of the range. - 잘못된 입력 (범위 넘어감)" << endl;
		goto ERR_LOOP;
	}

	cout << endl << endl;
	cout << "< Generating >" << endl << endl;

	vector<Mat> images;
	cout << "> Extracting Frames from the Video . . ." << endl;
	ExtractImages(video_path, images, (double)frame_num / (double)frame);
	while (frame < images.size()) {
		images.pop_back();
	}

	cout << "> Setting ADOFAI File . . ." << endl;
	auto previous_adofai = adofai;
	for (int i = 0; i < deco_num; i++) {
		string event_tag = "[DynamicDecorationEventTag_" + deco_tag[i] + "]";
		adofai["decorations"].append(AddDecoration(tile_num[i], univ_tag + deco_tag[i]));
		double offset = begin_offset[i];
		for (int j = 0; j < images.size(); j++) {
			adofai["actions"].append(MoveDecorations(tile_num[i], offset, DecoFileName(deco_file_name, j), deco_tag[i], event_tag));
			offset += angle_offset;
		}
		if (loop) {
			adofai["actions"].append(RepeatEvents(tile_num[i], event_tag, (double)images.size() * angle_offset / 180.0, loop - 1));
		}
	}

	cout << "> Saving ADOFAI File . . ." << endl;

	WriteJson(adofai, adofai_path);

	cout << "> Saving Decoration Files . . ." << endl;
	filesystem::create_directory(folder_path + deco_file_name);
	SaveImages(folder_path, deco_file_name, images);
	cout << endl;
	cout << "> Completed" << endl << endl;

	cout << "< Please Check Your New ADOFAI File - 얼불춤 맵을 확인해보세요 >" << endl << endl;
	ERR4:
	cout << "> Undo (되돌리기) (Y/N) : ";
	string reply;
	cin >> reply;
	if (reply != "Y" && reply != "N") {
		cout << "> Reply should be Y or N. - 잘못된 입력 (Y 또는 N 이 아님)" << endl;
		goto ERR4;
	}

	if (reply == "Y") {
		cout << endl;
		cout << "> Undoing ADOFAI File . . ." << endl;
		WriteJson(previous_adofai, adofai_path);
		cout << "> Deleting Decoration Files . . ." << endl;
		std::filesystem::remove_all(folder_path + deco_file_name);
		//RemoveImages(folder_path, deco_file_name, images.size());
	}

	cout << endl << endl;
	cout << "< Completed!! - 완료되었습니다 >" << endl << endl;
	system("pause");
	return 0;
}