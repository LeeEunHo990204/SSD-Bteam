#include "ssd.h"

SSD::SSD(void) {
}

SSD::~SSD() {
	file.close();
}

void SSD::init(void) {
	file.open(filename, std::ios::in | std::ios::out);

	if (!file.is_open()) {
		ofstream outfile(filename);
		if (!outfile.is_open()) {
			std::cerr << "파일 생성에 실패했습니다!" << std::endl;
			return;
		}

		for (int i = 0; i < STORAGE_SIZE; i++) {
			outfile << setw(LINE_LENGTH - 1) << std::left << to_string(i) + " 0x00000000" << "\n";
		}

		outfile.close();

		file.open(filename);
		if (!file.is_open()) {
			cerr << "생성 후 파일 열기에 실패했습니다!" << endl;
			return;
		}
	}

	string line;

	for (int i = 0; i < STORAGE_SIZE; i++) {
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

void SSD::write(int idx, unsigned int value) {
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
	file << std::string(LINE_LENGTH - 1, ' ');
	file.seekp((LINE_LENGTH + 1) * idx, std::ios::beg);
	file << std::dec << idx << std::uppercase << " 0x" << std::hex << std::setw(8) << std::setfill('0') << storage[idx];
	storage[idx] = value;
}

unsigned int SSD::read(int idx) {
	if (!isAddressValid(idx)) {
		std::cerr << "유효하지 않은 주소입니다!" << std::endl;
		return 0;
	}
	ofstream outfile("ssd_output.txt");
	if (!outfile.is_open()) {
		std::cerr << "파일 생성에 실패했습니다!" << std::endl;
		return 0;
	}

	outfile << "0x" << std::uppercase << hex << std::setw(8) << std::setfill('0') << storage[idx] << endl;
	cout << "0x" << std::uppercase << hex << std::setw(8) << std::setfill('0') << storage[idx] << endl;
	outfile.close();
	return storage[idx];
}

bool SSD::erase(int idx, int size) {
	if (!isAddressValid(idx)) {
		std::cerr << "유효하지 않은 주소입니다!" << std::endl;
		return false;
	}
	if (size < 0 || size > 10) {
		cout << "size is wrong" << endl;
		return isAddressValid(-1);
	}
	string line;
	int startAddress = idx;
	int endAddress = (idx + size - 1 < STORAGE_SIZE) ? idx + size - 1 : STORAGE_SIZE - 1;
	if (!file.is_open()) {
		std::cerr << "파일 생성에 실패했습니다!" << std::endl;
		return false;
	}
	file.seekp((LINE_LENGTH + 1) * startAddress, std::ios::beg);
	for (int i = startAddress; i <= endAddress; i++) {
		storage[i] = 0;
		file << std::dec << i << " 0x00000000";
		getline(file, line);
	}

	return true;
}

bool SSD::isAddressValid(int idx) {
	if (idx >= 0 && idx < STORAGE_SIZE)
	{
		return true;
	}
	ofstream outfile("ssd_output.txt");
	if (!outfile.is_open()) {
		std::cerr << "파일 생성에 실패했습니다!" << std::endl;
		return false;
	}
	outfile << "ERROR" << endl;
	outfile.close();

	return false;
}

unsigned int SSD::get(int idx) {
	return storage[idx];
}

void SSD::set(int idx, unsigned int value) {
	storage[idx] = value;
}