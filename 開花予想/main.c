#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include<time.h>
#include "sqlite3.h"

#define OBS_NUM sizeof(observations)/sizeof(observations[0])
#define YEAR 2025

enum MENU {
	VIEW, SEARCH, ERASE, EXIT
};

// �������Ƀ��X�|���X���i�[����\����
struct Memory {
	char* response;
	size_t size;
};

typedef struct {
	char name[15];
	int no;
	int menu;
}OBSERVATION;

OBSERVATION observations[] = {
	{"�t��", 47401, 0}, {"�k���}�K", 47402, 1}, {"�H�y", 47404, 2}, {"�Y��", 47405, 3},
	{"���G", 47406, 4}, {"����", 47407, 5}, {"�ԑ�", 47409, 6}, {"���M", 47411, 7},
	{"�D�y", 47412, 8}, {"�〈��", 47413, 9}, {"�эL", 47417, 10}, {"���H", 47418, 11},
	{"����", 47420, 12}, {"���s", 47421, 13}, {"����", 47423, 14}, {"�Ϗ��q", 47424, 15},
	{"�Y��", 47426, 16}, {"�]��", 47428, 17}, {"����", 47430, 18}, {"��m��", 47433, 19},
	{"���", 47435, 20}, {"�L��", 47440, 21}, {"��D�n", 47512, 22}, {"�V��", 47520, 23},
	{"�ᏼ", 47570, 24}, {"�[�Y", 47574, 25}, {"�X", 47575, 26}, {"�ނ�", 47576, 27},
	{"����", 47581, 28}, {"�H�c", 47582, 29}, {"����", 47584, 30}, {"�{��", 47585, 31},
	{"��c", 47587, 32}, {"�R�`", 47588, 33}, {"���", 47590, 34}, {"�Ί�", 47592, 35},
	{"����", 47595, 36}, {"����", 47597, 37}, {"�����l", 47598, 38}, {"�֓�", 47600, 39},
	{"����", 47602, 40}, {"�V��", 47604, 41}, {"����", 47605, 42}, {"����", 47606, 43},
	{"�x�R", 47607, 44}, {"����", 47610, 45}, {"���c", 47612, 46}, {"�F�s�{", 47615, 47},
	{"����", 47616, 48}, {"���R", 47617, 49}, {"���{", 47618, 50}, {"�z�K", 47620, 51},
	{"�y���", 47622, 52}, {"�O��", 47624, 53}, {"�F�J", 47626, 54}, {"����", 47629, 55},
	{"�։�", 47631, 56}, {"��", 47632, 57}, {"���É�", 47636, 58}, {"�ѓc", 47637, 59},
	{"�b�{", 47638, 60}, {"�x�m�R", 47639, 61}, {"�͌���", 47640, 62}, {"����", 47641, 63},
	{"����", 47646, 64}, {"���q", 47648, 65}, {"���", 47649, 66}, {"��", 47651, 67},
	{"�ɗǌ�", 47653, 68}, {"�l��", 47654, 69}, {"��O��", 47655, 70}, {"�É�", 47656, 71},
	{"�O��", 47657, 72}, {"����", 47662, 73}, {"���h", 47663, 74}, {"�ΘL��", 47666, 75},
	{"�ԑ�", 47668, 76}, {"���l", 47670, 77}, {"�َR", 47672, 78}, {"���Y", 47674, 79},
	{"�哇", 47675, 80}, {"�O�", 47677, 81}, {"���䓇", 47678, 82}, {"��t", 47682, 83},
	{"�l���s", 47684, 84}, {"������", 47690, 85}, {"����", 47740, 86}, {"���]", 47741, 87},
	{"��", 47742, 88}, {"�Ďq", 47744, 89}, {"����", 47746, 90}, {"�L��", 47747, 91},
	{"����", 47750, 92}, {"�ɐ��R", 47751, 93}, {"��", 47754, 94}, {"�l�c", 47755, 95},
	{"�ÎR", 47756, 96}, {"���s", 47759, 97}, {"�F��", 47761, 98}, {"����", 47762, 99},
	{"�L��", 47765, 100}, {"��", 47766, 101}, {"���R", 47767, 102}, {"���R", 47768, 103},
	{"�P�H", 47769, 104}, {"�_��", 47770, 105}, {"���", 47772, 106}, {"�F�{", 47776, 107},
	{"�a�̎R", 47777, 108}, {"����", 47778, 109}, {"�ޗ�", 47780, 110}, {"�R��", 47784, 111},
	{"����", 47800, 112}, {"����", 47805, 113}, {"����", 47807, 114}, {"�ђ�", 47809, 115},
	{"������", 47812, 116}, {"����", 47813, 117}, {"���c", 47814, 118}, {"�啪", 47815, 119},
	{"����", 47817, 120}, {"�_��x", 47818, 121}, {"�F�{", 47819, 122}, {"���h�R", 47821, 123},
	{"����", 47822, 124}, {"���v��", 47823, 125}, {"�l�g", 47824, 126}, {"������", 47827, 127},
	{"�s��", 47829, 128}, {"�{��", 47830, 129}, {"����", 47831, 130}, {"����", 47835, 131},
	{"���v��", 47836, 132}, {"��q��", 47837, 133}, {"���[", 47838, 134}, {"���]", 47843, 135},
	{"���R", 47887, 136}, {"���x��", 47890, 137}, {"����", 47891, 138}, {"�F�a��", 47892, 139},
	{"���m", 47893, 140}, {"���R", 47894, 141}, {"����", 47895, 142}, {"�h��", 47897, 143},
	{"����", 47898, 144}, {"���˖�", 47899, 145}, {"����", 47909, 146}, {"�^�ߍ���", 47912, 147},
	{"�Ί_��", 47918, 148}, {"�{�Ó�", 47927, 149}, {"�v�ē�", 47929, 150}, {"�ߔe", 47936, 151},
	{"����", 47940, 152}, {"���i�Ǖ�", 47942, 153}, {"��哌", 47945, 154}, {"����", 47971, 155},
	{"�쒹��", 47991, 156}
};

struct tm makeTimeStruct(int y, int m, int d) {
	struct tm t;

	t.tm_year = y;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_isdst = -1;

	if (mktime(&t) == (time_t)-1) {
		puts("���Ԑ������s");
		exit(1);
	}
	return t;
}
// ���X�|���X�f�[�^���������ɕۑ�
static size_t write_callback(void* data, size_t size, size_t nmemb, void* userp) {
	size_t total_size = size * nmemb;
	struct Memory* mem = (struct Memory*)userp;

	if (!mem->response) {
		mem->response = malloc(total_size + 1);
	}
	else {
		char* ptr = realloc(mem->response, mem->size + total_size + 1);
		if (ptr == NULL) {
			fprintf(stderr, "Memory allocation failed!\n");
			return 0;
		}
		mem->response = ptr;
	}

	memcpy(&(mem->response[mem->size]), data, total_size);
	mem->size += total_size;
	mem->response[mem->size] = '\0';

	return total_size;
}


// API����f�[�^���擾
char* fetch_data(int no, int year, int month, int thisYear) {
	CURL* curl;
	CURLcode res;
	struct Memory chunk = { NULL, 0 };  // malloc(1) ����߂� NULL �ɏ�����
	char url[256];

	snprintf(url, sizeof(url),
		thisYear ? "https://api.cultivationdata.net/twoweek?no=%d"
		: "https://api.cultivationdata.net/past?no=%d&year=%d&month=%d",
		no, year, month);

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			return NULL;  // �G���[���� NULL ��Ԃ�
		}

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	if (chunk.size == 0) {
		fprintf(stderr, "No data received from server.\n");
		return NULL;
	}

	return chunk.response;
}

// �R�[���o�b�N�֐�
int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int sqlCreateExecute(const char* sql, sqlite3* db) {
	char* errMsg = 0;
	int rc;
	rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
		return 1;
	}
	else {
		printf("Table Open successfully\n");
		return 0;
	}
}

int  sqlSelectExecute(const char* sql, sqlite3* db, char* observationName) {
	sqlite3_stmt* stmt;
	int rc, flg;

	// SQL������
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
	}
	if (observationName != NULL) {
		sqlite3_bind_text(stmt, 1, observationName, -1, SQLITE_STATIC);
	}
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		putchar('\n');
		do {
			const char* name = (const char*)sqlite3_column_text(stmt, 0);
			const char* date = (const char*)sqlite3_column_text(stmt, 1);
			printf("�n�於: %s, �J�ԗ\�z��: %s\n", name, date);
		} while (sqlite3_step(stmt) == SQLITE_ROW);
		flg = 1;
	}
	else {
		flg = 0;
	}
	putchar('\n');
	// �X�e�[�g�����g�̉��
	sqlite3_finalize(stmt);
	return flg;
}

void  sqlDeleteExecute(const char* sql, sqlite3* db, char* observationName) {
	sqlite3_stmt* stmt;
	int rc;

	// SQL������
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
	}
	else {
		sqlite3_bind_text(stmt, 1, observationName, -1, SQLITE_STATIC);

		if (sqlite3_step(stmt) == SQLITE_DONE) {
			printf("Table Delete successfully\n");
		}
		else {
			printf("Failed to delete\n");
		}
	}
	putchar('\n');
	// �X�e�[�g�����g�̉��
	sqlite3_finalize(stmt);
}

void sqlInsertExecute(const char* sql, sqlite3* db, char* observationName, char* forecastDayString) {
	sqlite3_stmt* stmt;
	int rc;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	sqlite3_bind_text(stmt, 1, observationName, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, forecastDayString, -1, SQLITE_STATIC);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
	}
	else {
		printf("\n�f�[�^�x�[�X�ɓ��t���o�^����܂����B\n\n");
	}
	//�X�e�[�g�����g�����
	sqlite3_finalize(stmt);
}

int main() {
	int no, year, month, day = 1, flg1 = 0, flg2 = 0, dayOfAve = 0, dayOfHi = 0, monthOfAve = 0, monthOfHi = 0, monthOfForecast = 0, dayOfForecast = 0, menu, rc;
	double aveSum = 0.0, hiSum = 0.0;
	char* p, * data, * pre;
	char dayString[10], ave[10], hi[10], forecastDayString[11];
	int daysOfMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	sqlite3* db;
	//sqlite3_stmt* stmt;
	time_t now = time(NULL);				// ����i���݂̗���j
	struct tm* current = localtime(&now);
	struct tm tmHi, tmAve;

	for (int i = 0; i < 30; i++) {
		printf("%10s...%5d", observations[i].name, observations[i].menu);
		if (i + 30 < OBS_NUM) {
			printf("%10s...%5d", observations[i + 30].name, observations[i + 30].menu);
		}
		if (i + 60 < OBS_NUM) {
			printf("%10s...%5d", observations[i + 60].name, observations[i + 60].menu);
		}
		if (i + 90 < OBS_NUM) {
			printf("%10s...%5d", observations[i + 90].name, observations[i + 90].menu);
		}
		if (i + 120 < OBS_NUM) {
			printf("%10s...%5d", observations[i + 120].name, observations[i + 120].menu);
		}
		if (i + 150 < OBS_NUM) {
			printf("%10s...%5d", observations[i + 150].name, observations[i + 150].menu);
		}
		putchar('\n');
	}
	rc = sqlite3_open("kaikayoso.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		printf("Opened database successfully\n");
	}
	const char* sql = "CREATE TABLE IF NOT EXISTS Dates("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		"Name TEXT NOT NULL,"
		"Date TEXT NOT NULL);";

	sqlCreateExecute(sql, db);

	do {
		printf("�ꗗ�\��...(0) / ����...(1) / �폜...(2) / �I��...(3):");
		do {
			(void)scanf("%d", &menu);
		} while (menu < 0 || menu>3);

		switch (menu) {
		case VIEW:
			sql = "SELECT Name,date(Date,'localtime') AS Date FROM Dates ORDER BY Date";
			sqlSelectExecute(sql, db, NULL);
			break;
		case SEARCH:
			do {
				printf("�n��ԍ� (no) �����(0~%d): ", (int)(OBS_NUM - 1));
				(void)scanf("%d", &no);
			} while (no<0 || no>OBS_NUM - 1);

			sql = "SELECT Name,date(Date,'localtime') AS Date FROM Dates WHERE Name=?";

			if (sqlSelectExecute(sql, db, observations[no].name)) {
				break;
			}

			printf("%s�̊J�ԗ\�z�����v�Z���Ă��܂�...\n", observations[no].name);

			day = 1;
			aveSum = hiSum = flg1 = flg2 = 0;
			year = YEAR;

			for (month = 2; !(flg1 * flg2); month++) {
				data = fetch_data(observations[no].no, year, month, 0);
				sprintf(dayString, "%d-%d-%d", year, month, day);
				pre = data;

				if (data) {
					while ((p = strstr(pre, dayString)) != NULL && !(flg1 * flg2)) {
						p += 165;
						p = strchr(p, ':') + 2;
						aveSum += atof(p);
						if (aveSum >= 400 && !flg1) {
							dayOfAve = day;
							monthOfAve = month;
							flg1 = 1;
						}
						p = strchr(p, ':') + 2;
						hiSum += atof(p);
						if (hiSum >= 600 && !flg2) {
							dayOfHi = day;
							monthOfHi = month;
							flg2 = 1;
						}
						pre = p;
						day++;
						//����܂ł̃f�[�^�����݂���
						if (day == current->tm_mday) {
							if (year == current->tm_year + 1900 && month == current->tm_mon + 1) {
								free(data);
								data = fetch_data(observations[no].no, year, month, 1);
								pre = data;
								if (data) {
									for (int i = 0; i < 13 && !flg1; i++) {
										sprintf(ave, "ave%d", i);
										p = strstr(pre, ave) + strlen(ave) + 3;
										aveSum += atof(p);
										if (aveSum >= 400) {
											dayOfAve = day + i;
											monthOfAve = month;
											flg1 = 1;
										}
										pre = p;
									}
									for (int i = 0; i < 13 && !flg2; i++) {
										sprintf(hi, "hi%d", i);
										p = strstr(pre, hi) + strlen(hi) + 3;
										hiSum += atof(p);
										if (hiSum >= 600) {
											dayOfHi = day + i;
											monthOfHi = month;
											flg2 = 1;
										}
										pre = p;
									}
								}
								else {
									printf("�f�[�^�擾�Ɏ��s���܂���\n");
								}
							}
						}
						sprintf(dayString, "%d-%d-%d", year, month, day);
					}
				}
				else {
					printf("�f�[�^�擾�Ɏ��s���܂���\n");
				}
				if (flg1 * flg2) {
					break;
				}
				else if (year == current->tm_year + 1900 && month == current->tm_mon + 1) {
					year--;
					month--;
					day += 13;
				}
				else {
					day = 1;
				}
				free(data);
			}
			printf("\n2/1����̕��ϋC�������v400�x�𒴂�����%d/%d\n�ō��C�������v600�x�𒴂�����%d/%d\n", monthOfAve, dayOfAve, monthOfHi, dayOfHi);
			if (monthOfAve != monthOfHi) {
				tmAve = makeTimeStruct(current->tm_year, monthOfAve, dayOfAve);
				tmHi = makeTimeStruct(current->tm_year, monthOfHi, dayOfHi);
				if (tmAve.tm_yday < tmHi.tm_yday) {
					monthOfForecast = monthOfAve;
					dayOfForecast = (tmHi.tm_yday - tmAve.tm_yday) / 2 + dayOfAve;
					while (dayOfForecast > daysOfMonth[monthOfForecast - 1]) {
						dayOfForecast -= daysOfMonth[monthOfForecast - 1];
						monthOfForecast++;
					}
				}
				else {
					monthOfForecast = monthOfHi;
					dayOfForecast = (tmHi.tm_yday - tmAve.tm_yday) / 2 + dayOfHi;
					while (dayOfForecast > daysOfMonth[monthOfForecast - 1]) {
						dayOfForecast -= daysOfMonth[monthOfForecast - 1];
						monthOfForecast++;
					}
				}
			}
			else {
				monthOfForecast = monthOfAve;
				dayOfForecast = (dayOfAve + dayOfHi) / 2;
			}
			printf("�J�ԗ\�z��->%d��%d��\n", monthOfForecast, dayOfForecast);

			snprintf(forecastDayString, sizeof(forecastDayString), "%d-%02d-%02d", current->tm_year + 1900, monthOfForecast, dayOfForecast);

			dayOfForecast += 7;
			if (!(dayOfForecast < daysOfMonth[monthOfForecast - 1])) {
				dayOfForecast = dayOfForecast - daysOfMonth[monthOfForecast - 1];
				monthOfForecast += 1;
			}
			struct tm tmForecast = makeTimeStruct(current->tm_year, monthOfForecast, dayOfForecast);
			tmForecast.tm_mday += 7 - tmForecast.tm_wday;
			if (!(tmForecast.tm_mday <= daysOfMonth[tmForecast.tm_mon])) {
				tmForecast.tm_mday = tmForecast.tm_mday - daysOfMonth[tmForecast.tm_mon];
				tmForecast.tm_mon += 1;
			}
			printf("�s�y�̍œK��->%d��%d��\n", tmForecast.tm_mon + 1, tmForecast.tm_mday);

			sql = "INSERT INTO Dates(Name,Date) VALUES(?,?)";

			sqlInsertExecute(sql, db, observations[no].name, forecastDayString);

			break;
		case ERASE:
			do {
				printf("�n��ԍ� (no) �����(0~%d): ", (int)(OBS_NUM - 1));
				(void)scanf("%d", &no);
			} while (no<0 || no>OBS_NUM - 1);
			sql = "DELETE FROM Dates WHERE Name=?";
			printf("%s�̃f�[�^���폜���܂�...\n", observations[no].name);
			sqlDeleteExecute(sql, db, observations[no].name);
			break;
		}
	} while (menu != EXIT);


	// �f�[�^�x�[�X�����
	sqlite3_close(db);
	return 0;
}