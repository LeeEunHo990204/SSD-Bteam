#include "ssd.h"

void Ssd::init(void) {
	string filename = "ssd_nand.txt";

	ifstream file(filename);
	string line;
	
	if (!file.is_open()) {
		ofstream outfile(filename);
		if (!outfile.is_open()) {
			std::cerr << "파일 생성에 실패했습니다!" << std::endl;
			return;
		}

		for (int i = 0; i < 100; i++) {
			outfile << i << " " << endl;
		}

		outfile.close();

		file.open(filename);
		if (!file.is_open()) {
			cerr << "생성 후 파일 열기에 실패했습니다!" << endl;
			return;
		}
	}

	for (int i = 0; i < 100; i++) {
		getline(file, line);
		std::istringstream iss(line);
		std::string index_part, hex_part;

		iss >> index_part >> hex_part; // [3] 와 0x12345678 분리

		if (!hex_part.empty() && hex_part.rfind("0x", 0) == 0) {  // "0x"로 시작하면
			try {
				unsigned int value = std::stoul(hex_part, nullptr, 16);
				storage[i] = value;
			}
			catch (const std::exception& e) {
				std::cerr << "변환 오류: '" << hex_part << "' → 0으로 대체됨" << std::endl;
				storage[i] = 0;
			}
		}
		else {
			storage[i] = 0;
		}
	}
}

void Ssd::write(int idx, int value) {
	storage[idx] = value;
}

unsigned Ssd::read(int idx) {
	return storage[idx];
}