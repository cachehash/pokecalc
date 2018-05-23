#include <tidy/tidy.h>
#include <tidy/tidybuffio.h>
#include <curl/curl.h>
#include "stat.h"

int write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
	uint r;
	r = size * nmemb;
	tidyBufAppend(out, in, r);
	return r;
}
int isStatName(char* str) {
	if (str == NULL) {
		return 0;
	}
	for (int i = 0; i < NUM_STATS; i++) {
		if (strcmp(str, STAT_LONG_NAMES[i]) == 0) {
			return 1;
		}
	}
	return strcmp(str, "HP") == 0;
}
void proc(TidyDoc doc, TidyNode tnod) {
	char* delim = "<>";
	for (TidyNode child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name) {
			if (strcmp("th", name) == 0) {
				TidyBuffer buf;
				tidyBufInit(&buf);
				tidyNodeGetText(doc, child, &buf);
				char* str = strtok((char*)buf.bp, delim);
				str = strtok(NULL, delim);
				if (isStatName(str)) {
					TidyBuffer buf;
					tidyBufInit(&buf);
					child = tidyGetNext(child);
					tidyNodeGetText(doc, child, &buf);
					char* str = strtok((char*)buf.bp, delim);
					str = strtok(NULL, delim);
					printf("%d\n", atoi(str));
				}
			}
		}
		proc(doc, child);
	}
}
int main() {
	char curl_err[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = {0};
	TidyBuffer tidy_errbuf = {0};
	int err;

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "https://pokemondb.net/pokedex/regigigas");
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_err);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	tdoc = tidyCreate();
	tidySetErrorBuffer(tdoc, &tidy_errbuf);
	tidyBufInit(&docbuf);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
	err = curl_easy_perform(curl);
	if (!err) {
		err = tidyParseBuffer(tdoc, &docbuf);
		if (err >= 0) {
			err = tidyCleanAndRepair(tdoc);
			if (err >= 0) {
				err = tidyRunDiagnostics(tdoc);
				if (err >= 0) {
					proc(tdoc, tidyGetRoot(tdoc));
				}
			}
		}
	}
	return 0;
}
