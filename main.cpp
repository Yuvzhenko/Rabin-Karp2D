#include <iostream>
#include <vector>
#include <string>

using namespace std;

static const long long MOD = 1000000007;
static const long long BASE_ROW = 911382323;
static const long long BASE_COL = 972663749;

// building tabel of powers for base
vector<long long> buildPows(long long base, int n) {
	vector<long long> p(n + 1, 1);
	for (int i = 1; i <= n; i++)
		p[i] = (p[i - 1] * base) % MOD;
	return p;
}

vector<vector<long long>> buildHash2D(const vector<string>& a) {
	int n = (int)a.size();
	int m = (int)a[0].size();
	vector<vector<long long>> H(n, vector<long long>(m, 0));

	for(int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			// current symbol value
			long long val = (unsigned char)a[i][j];

			long long up = (i ? (H[i - 1][j] * BASE_ROW) % MOD : 0);
			long long left = (j ? (H[i][j - 1] * BASE_COL) % MOD : 0);
			long long diag = (i && j) ? (H[i - 1][j - 1] * BASE_ROW) % MOD * BASE_COL % MOD : 0;

			long long cur = (val + up + left - diag) % MOD;
			if (cur < 0)
				cur += MOD; 
			H[i][j] = cur;
		}
	return H;
}

long long getSubHash(const vector<vector<long long>>& H, const vector<long long>& powRow,
					 const vector<long long>& powCol, int x, int y, int sz) {
	// (x, y) - upper left angle
	// (x2, y2) - bottom right angle
	int x2 = x + sz - 1;
	int y2 = y + sz - 1;

	long long res = H[x2][y2];

	if (x > 0) {
		res = (res - H[x - 1][y2] * powRow[sz]) % MOD;
		if (res < 0)
			res += MOD;
	}
	if (y > 0) {
		res = (res - H[x2][y - 1] * powCol[sz]) % MOD;
		if (res < 0)
			res += MOD;
	}
	if (x > 0 && y > 0) {
		long long addBack = H[x - 1][y - 1];
		addBack = (addBack * powRow[sz]) % MOD;
		addBack = (addBack * powCol[sz]) % MOD;
		res = (res + addBack) % MOD;
	}
	return res;
}

vector<pair<int, int>> rabinKarp2D(const vector<string>& text, const vector<string>& pattern) {
	int n = (int)text.size();
	int m = (int)pattern.size();
	if (n == 0 || m == 0 || m > n) return {};

	vector<long long> powRow = buildPows(BASE_ROW, n);
	vector<long long> powCol = buildPows(BASE_COL, n);

	vector<vector<long long>> Htext = buildHash2D(text);
	vector<vector<long long>> Hpat = buildHash2D(pattern);

	long long patHash = getSubHash(Hpat, powRow, powCol, 0, 0, m);

	vector<pair<int, int>> res;

	for (int i = 0; i + m <= n; i++) {
		for (int j = 0; j + m <= n; j++) {
			long long cur = getSubHash(Htext, powRow, powCol, i, j, m);
			if (cur == patHash) {
				bool ok = true;
				for (int x = 0; x < m && ok; x++) {
					for (int y = 0; y < m; y++) {
						if (text[i + x][j + y] != pattern[x][y]) {
							ok = false;
							break;
						}
					}
				}
				if (ok)
					res.emplace_back(i, j);
			}
		}
	}
	return res;
}

int main() {
	vector<string> text = {
		"abcde",
		"fGHij",
		"kLMno",
		"psGHt",
		"uxLMy" };

	vector<string> pat = { 
		"GH",
		"LM" };

	vector<pair<int, int>> ans = rabinKarp2D(text, pat);
	for (int i = 0; i < (int)ans.size(); i++) {
		int r = ans[i].first;
		int c = ans[i].second;
		cout << "found at (" << r << ", " << c << ")\n";
	}
}