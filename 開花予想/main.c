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

// メモリにレスポンスを格納する構造体
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
	{"稚内", 47401, 0}, {"北見枝幸", 47402, 1}, {"羽幌", 47404, 2}, {"雄武", 47405, 3},
	{"留萌", 47406, 4}, {"旭川", 47407, 5}, {"網走", 47409, 6}, {"小樽", 47411, 7},
	{"札幌", 47412, 8}, {"岩見沢", 47413, 9}, {"帯広", 47417, 10}, {"釧路", 47418, 11},
	{"根室", 47420, 12}, {"寿都", 47421, 13}, {"室蘭", 47423, 14}, {"苫小牧", 47424, 15},
	{"浦河", 47426, 16}, {"江差", 47428, 17}, {"函館", 47430, 18}, {"倶知安", 47433, 19},
	{"紋別", 47435, 20}, {"広尾", 47440, 21}, {"大船渡", 47512, 22}, {"新庄", 47520, 23},
	{"若松", 47570, 24}, {"深浦", 47574, 25}, {"青森", 47575, 26}, {"むつ", 47576, 27},
	{"八戸", 47581, 28}, {"秋田", 47582, 29}, {"盛岡", 47584, 30}, {"宮古", 47585, 31},
	{"酒田", 47587, 32}, {"山形", 47588, 33}, {"仙台", 47590, 34}, {"石巻", 47592, 35},
	{"福島", 47595, 36}, {"白河", 47597, 37}, {"小名浜", 47598, 38}, {"輪島", 47600, 39},
	{"相川", 47602, 40}, {"新潟", 47604, 41}, {"金沢", 47605, 42}, {"伏木", 47606, 43},
	{"富山", 47607, 44}, {"長野", 47610, 45}, {"高田", 47612, 46}, {"宇都宮", 47615, 47},
	{"福井", 47616, 48}, {"高山", 47617, 49}, {"松本", 47618, 50}, {"諏訪", 47620, 51},
	{"軽井沢", 47622, 52}, {"前橋", 47624, 53}, {"熊谷", 47626, 54}, {"水戸", 47629, 55},
	{"敦賀", 47631, 56}, {"岐阜", 47632, 57}, {"名古屋", 47636, 58}, {"飯田", 47637, 59},
	{"甲府", 47638, 60}, {"富士山", 47639, 61}, {"河口湖", 47640, 62}, {"秩父", 47641, 63},
	{"つくば", 47646, 64}, {"銚子", 47648, 65}, {"上野", 47649, 66}, {"津", 47651, 67},
	{"伊良湖", 47653, 68}, {"浜松", 47654, 69}, {"御前崎", 47655, 70}, {"静岡", 47656, 71},
	{"三島", 47657, 72}, {"東京", 47662, 73}, {"尾鷲", 47663, 74}, {"石廊崎", 47666, 75},
	{"網代", 47668, 76}, {"横浜", 47670, 77}, {"館山", 47672, 78}, {"勝浦", 47674, 79},
	{"大島", 47675, 80}, {"三宅島", 47677, 81}, {"八丈島", 47678, 82}, {"千葉", 47682, 83},
	{"四日市", 47684, 84}, {"奥日光", 47690, 85}, {"西郷", 47740, 86}, {"松江", 47741, 87},
	{"境", 47742, 88}, {"米子", 47744, 89}, {"鳥取", 47746, 90}, {"豊岡", 47747, 91},
	{"舞鶴", 47750, 92}, {"伊吹山", 47751, 93}, {"萩", 47754, 94}, {"浜田", 47755, 95},
	{"津山", 47756, 96}, {"京都", 47759, 97}, {"彦根", 47761, 98}, {"下関", 47762, 99},
	{"広島", 47765, 100}, {"呉", 47766, 101}, {"福山", 47767, 102}, {"岡山", 47768, 103},
	{"姫路", 47769, 104}, {"神戸", 47770, 105}, {"大阪", 47772, 106}, {"洲本", 47776, 107},
	{"和歌山", 47777, 108}, {"潮岬", 47778, 109}, {"奈良", 47780, 110}, {"山口", 47784, 111},
	{"厳原", 47800, 112}, {"平戸", 47805, 113}, {"福岡", 47807, 114}, {"飯塚", 47809, 115},
	{"佐世保", 47812, 116}, {"佐賀", 47813, 117}, {"日田", 47814, 118}, {"大分", 47815, 119},
	{"長崎", 47817, 120}, {"雲仙岳", 47818, 121}, {"熊本", 47819, 122}, {"阿蘇山", 47821, 123},
	{"延岡", 47822, 124}, {"阿久根", 47823, 125}, {"人吉", 47824, 126}, {"鹿児島", 47827, 127},
	{"都城", 47829, 128}, {"宮崎", 47830, 129}, {"枕崎", 47831, 130}, {"油津", 47835, 131},
	{"屋久島", 47836, 132}, {"種子島", 47837, 133}, {"牛深", 47838, 134}, {"福江", 47843, 135},
	{"松山", 47887, 136}, {"多度津", 47890, 137}, {"高松", 47891, 138}, {"宇和島", 47892, 139},
	{"高知", 47893, 140}, {"剣山", 47894, 141}, {"徳島", 47895, 142}, {"宿毛", 47897, 143},
	{"清水", 47898, 144}, {"室戸岬", 47899, 145}, {"名瀬", 47909, 146}, {"与那国島", 47912, 147},
	{"石垣島", 47918, 148}, {"宮古島", 47927, 149}, {"久米島", 47929, 150}, {"那覇", 47936, 151},
	{"名護", 47940, 152}, {"沖永良部", 47942, 153}, {"南大東", 47945, 154}, {"父島", 47971, 155},
	{"南鳥島", 47991, 156}
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
		puts("時間生成失敗");
		exit(1);
	}
	return t;
}
// レスポンスデータをメモリに保存
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


// APIからデータを取得
char* fetch_data(int no, int year, int month, int thisYear) {
	CURL* curl;
	CURLcode res;
	struct Memory chunk = { NULL, 0 };  // malloc(1) をやめて NULL に初期化
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
			return NULL;  // エラー時は NULL を返す
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

// コールバック関数
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

	// SQLを準備
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
			printf("地域名: %s, 開花予想日: %s\n", name, date);
		} while (sqlite3_step(stmt) == SQLITE_ROW);
		flg = 1;
	}
	else {
		flg = 0;
	}
	putchar('\n');
	// ステートメントの解放
	sqlite3_finalize(stmt);
	return flg;
}

void  sqlDeleteExecute(const char* sql, sqlite3* db, char* observationName) {
	sqlite3_stmt* stmt;
	int rc;

	// SQLを準備
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
	// ステートメントの解放
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
		printf("\nデータベースに日付が登録されました。\n\n");
	}
	//ステートメントを解放
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
	time_t now = time(NULL);				// 暦時刻（現在の暦時刻）
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
		printf("一覧表示...(0) / 検索...(1) / 削除...(2) / 終了...(3):");
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
				printf("地域番号 (no) を入力(0~%d): ", (int)(OBS_NUM - 1));
				(void)scanf("%d", &no);
			} while (no<0 || no>OBS_NUM - 1);

			sql = "SELECT Name,date(Date,'localtime') AS Date FROM Dates WHERE Name=?";

			if (sqlSelectExecute(sql, db, observations[no].name)) {
				break;
			}

			printf("%sの開花予想日を計算しています...\n", observations[no].name);

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
						//昨日までのデータが存在する
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
									printf("データ取得に失敗しました\n");
								}
							}
						}
						sprintf(dayString, "%d-%d-%d", year, month, day);
					}
				}
				else {
					printf("データ取得に失敗しました\n");
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
			printf("\n2/1からの平均気温が合計400度を超えた日%d/%d\n最高気温が合計600度を超えた日%d/%d\n", monthOfAve, dayOfAve, monthOfHi, dayOfHi);
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
			printf("開花予想日->%d月%d日\n", monthOfForecast, dayOfForecast);

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
			printf("行楽の最適日->%d月%d日\n", tmForecast.tm_mon + 1, tmForecast.tm_mday);

			sql = "INSERT INTO Dates(Name,Date) VALUES(?,?)";

			sqlInsertExecute(sql, db, observations[no].name, forecastDayString);

			break;
		case ERASE:
			do {
				printf("地域番号 (no) を入力(0~%d): ", (int)(OBS_NUM - 1));
				(void)scanf("%d", &no);
			} while (no<0 || no>OBS_NUM - 1);
			sql = "DELETE FROM Dates WHERE Name=?";
			printf("%sのデータを削除します...\n", observations[no].name);
			sqlDeleteExecute(sql, db, observations[no].name);
			break;
		}
	} while (menu != EXIT);


	// データベースを閉じる
	sqlite3_close(db);
	return 0;
}