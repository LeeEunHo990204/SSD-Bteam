#include "ssd.h"

SSD::SSD(void) {
	init();
}

void SSD::init(void) {
	file.open(filename, std::ios::in | std::ios::out);

	if (!file.is_open()) {
		ofstream outfile(filename);
		if (!outfile.is_open()) {
			std::cerr << "파일 생성에 실패했습니다!" << std::endl;
			return;
		}

		for (int i = 0; i < 100; i++) {
			outfile << setw(LINE_LENGTH - 1) << std::left << i << "\n";
		}

		outfile.close();

		file.open(filename);
		if (!file.is_open()) {
			cerr << "생성 후 파일 열기에 실패했습니다!" << endl;
			return;
		}
	}

	string line;

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

void SSD::write(int idx, int value) {
	if (!isAddressValid(idx)) {
		std::cerr << "유효하지 않은 주소입니다!" << std::endl;
		return;
	}
	storage[idx] = value;
	if (!file.is_open()) {
		std::cerr << "파일 생성에 실패했습니다!" << std::endl;
		return;
	}

	file.seekp((LINE_LENGTH + 1) * idx, std::ios::beg);
	file << std::string(20, ' ');
	file.seekp((LINE_LENGTH + 1) * idx, std::ios::beg);
	file << std::left << std::dec << idx << std::uppercase << " 0x" << std::hex << storage[idx];
	storage[idx] = value;
}

unsigned int SSD::read(int idx) {
	if (!isAddressValid(idx)) {
		std::cerr << "유효하지 않은 주소입니다!" << std::endl;
		return 0;
	}
	ofstream outfile("ssd_output.txt", std::ios::in | std::ios::trunc);
	if (!outfile.is_open()) {
		std::cerr << "파일 생성에 실패했습니다!" << std::endl;
		return 0;
	}

	outfile << "0x" << std::uppercase << hex << storage[idx] << endl;
	outfile.close();
	return storage[idx];
}

bool SSD::isAddressValid(int idx) {
	if (idx >= 0 && idx < 100)
	{
		return true;
	}
	ofstream outfile("ssd_output.txt", std::ios::in | std::ios::trunc);
	if (!outfile.is_open()) {
		std::cerr << "파일 생성에 실패했습니다!" << std::endl;
		return false;
	}
	outfile << "ERROR" << endl;
	outfile.close();

	return false;
}