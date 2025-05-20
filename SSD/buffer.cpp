#include "buffer.h"

void Buffer::printCmdBuffer(void) {
	int idx = 0;
	for (const auto& name : cmdBuffer) {
		if (name.cmd != "") {
			cout << "[" << idx++ << "] "
				<< name.cmd << " " << name.LBA
				<< " " << toHexString(name.value) << endl;
		}
		else {
			cout << "[" << idx++ << "] empty" << endl;
		}
	}
}

void Buffer::printTxtBuffer(void) {
	for (const auto& name : txtBuffer) {
		cout << name << endl;
	}
}

void Buffer::convertCmdToTxt(void) {
	txtBuffer.clear();
	for (int i = 0; i < 5; i++) {
		if (cmdBuffer[i].cmd != "") {
			txtBuffer.push_back(to_string(i)
				+ "_"
				+ cmdBuffer[i].cmd
				+ "_"
				+ to_string(cmdBuffer[i].LBA)
				+ "_"
				+ to_string(cmdBuffer[i].value)
				+ ".txt");
		}
		else {
			txtBuffer.push_back(to_string(i)
				+ "_empty.txt");
		}
	}
}

void Buffer::convertTxtToCmd(void) {
	cmdBuffer.clear();
	for (const auto& name : txtBuffer) {
		string filename;
		size_t pos = name.find(".txt");
		if (pos != string::npos) {
			filename = name.substr(0, pos);
		}

		vector<string> parts;
		stringstream ss(filename);
		string token;

		while (getline(ss, token, '_')) {
			parts.push_back(token);
		}

		if (parts.size() == 2) {
			cmdBuffer.push_back({ "", 0, 0 });
			continue;
		}

		if (parts.size() < 4) {
			cerr << "txt filename is filename is invalid : " << name << endl;
			return;
		}

		cmdBuffer.push_back({ parts[1], stoi(parts[2]), stoi(parts[3]) });
	}
}

void Buffer::initBuffer(void) {
	if (_mkdir(dirName.c_str()) != 0) {
		cerr << "폴더 생성 실패!" << endl;
		return;
	}

	for (int i = 0; i < 5; ++i) {
		string filePath = dirName + "\\" + to_string(i) + "_empty.txt";
		ofstream out(filePath);
		if (out.is_open()) {
			out << i << "_empty.txt\n";
			out.close();
		}
		else {
			cerr << filePath << " 생성 실패!" << endl;
		}
	}
	cout << "폴더 및 파일 생성 완료!" << endl;
}

void Buffer::updateBuffer(void) {

}

void Buffer::readBufferDir(void) {
	LPCWSTR path = L"buffer\\*.txt";
	WIN32_FIND_DATAW findFileData;
	HANDLE hFind = FindFirstFileW(path, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		wcerr << L"파일을 찾을 수 없습니다." << endl;
		return;
	}

	do {
		// wstring -> string 변환
		wstring ws(findFileData.cFileName);
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		string filename = converter.to_bytes(ws);

		txtBuffer.push_back(filename);
	} while (FindNextFileW(hFind, &findFileData) != 0);

	FindClose(hFind);

	sort(txtBuffer.begin(), txtBuffer.end());
}

void Buffer::readBuffer(void) {
	if (!isBufferDir()) {
		initBuffer();
	}
	txtBuffer.clear();
	readBufferDir();
	convertTxtToCmd();
}

void Buffer::writeBuffer(void) {
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA("buffer\\*.txt", &findFileData);

	// 1. buffer 폴더 안의 .txt 파일 삭제
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string filePath = "buffer\\" + std::string(findFileData.cFileName);
			DeleteFileA(filePath.c_str());
		} while (FindNextFileA(hFind, &findFileData));
		FindClose(hFind);
	}

	// 2. txtBuffer에 있는 이름으로 파일 생성
	for (const auto& name : txtBuffer) {
		std::string filePath = "buffer\\" + name;
		std::ofstream outfile(filePath);
		if (!outfile.is_open()) {
			std::cerr << "Create File Failed: " << filePath << std::endl;
		}
		else {
			std::cout << "Create File Success: " << filePath << std::endl;
			outfile.close();
		}
	}
}