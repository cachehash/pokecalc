#include <tidy/tidy.h>
#include <tidy/tidybuffio.h>
#include <curl/curl.h>
#include "structs.h"
#include "stat.h"
#include "sqlite_helper.h"

int write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
	uint r;
	r = size * nmemb;
	tidyBufAppend(out, in, r);
	return r;
}
int getStatID(char* str) {
	if (str == NULL) {
		return -1;
	}
	for (int i = 0; i < NUM_STATS; i++) {
		if (strcmp(str, STAT_LONG_NAMES[i]) == 0) {
			return i;
		}
	}
	return -1;
}
void proc(TidyDoc doc, TidyNode tnod, void* vp) {
	pokeinfo* pkinf = vp;
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
				int statID = getStatID(str);
				if (statID != -1) {
					TidyBuffer buf;
					tidyBufInit(&buf);
					child = tidyGetNext(child);
					tidyNodeGetText(doc, child, &buf);
					char* str = strtok((char*)buf.bp, delim);
					str = strtok(NULL, delim);
					int baseStat = atoi(str);
					if (statID == Hp) {
						pkinf->numForms++;
						pkinf->forms = realloc(pkinf->forms, pkinf->numForms*sizeof(*pkinf->forms));
					}
					int formID = pkinf->numForms-1;
					pkinf->forms[formID].stats[statID] = baseStat;
				}
			}
		}
		proc(doc, child, vp);
	}
}
void doSQL(int nid, char* name);
void extractNames(TidyDoc doc, TidyNode tnod, void* vp) {
	int* count = vp;
	char* delim = "/";
	for (TidyNode child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name) {
			if (strcmp("a", name) == 0) {
				for (TidyAttr attr = tidyAttrFirst(child); attr; attr = tidyAttrNext(attr)) {
					const char* attrName = tidyAttrName(attr);
					if (strcmp(attrName, "class") == 0) {
						const char* value = tidyAttrValue(attr);
						if (value && strcmp(value, "pkg") == 0) {
							for (TidyAttr attr = tidyAttrFirst(child); attr; attr = tidyAttrNext(attr)) {
								attrName = tidyAttrName(attr);
								if (strcmp(attrName, "href") == 0) {
									char* str = strdup(tidyAttrValue(attr));
									char* tok = strtok(str, delim);
									tok = strtok(NULL, delim);
									++*count;
									doSQL(*count, tok);
									free(str);
									break;
								}
							}
							break;
						}
					}
				}
			}
		}
		extractNames(doc, child, vp);
	}
}
void processUrl(char *url, void(*fp)(TidyDoc,TidyNode,void*), void* vp) {

	char curl_err[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = {0};
	TidyBuffer tidy_errbuf = {0};
	int err;

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
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
					fp(tdoc, tidyGetRoot(tdoc), vp);
				}
			}
		}
	}
}
void doSQL(int nid, char* name) {
	const int BSZ = 2048;
	char sql[BSZ];
	snprintf(sql, BSZ, "https://pokemondb.net/pokedex/%s", name);
	pokeinfo pkinf;
	memset(&pkinf, 0, sizeof(pkinf));
	processUrl(sql, proc, &pkinf);
	for (int i = 0; i < pkinf.numForms; i++) {
		pokeform* f = &pkinf.forms[i];
		//TODO sanitize name before constructing query
		snprintf(sql, BSZ, "insert into pokemon values(%d, '%s', %d, %d, %d, %d, %d, %d, %d);",nid, name, i, f->stats[Hp], f->stats[Atk], f->stats[Def], f->stats[SpA], f->stats[SpD], f->stats[Spe]);
		sql_exec(sql);
	}
}
int initializeDataBase() {
	printf("Initializing database...\n");
	char *sql = "drop table if exists pokemon;\n"
		"create table pokemon(nid int, name varchar(255), formID int, Hp int, Atk int, Def int, SpA int, SpD int, Spe int);";
	int rc = sql_exec(sql);
	if (rc) {
		return rc;
	}
	int count = 0;
	processUrl("https://pokemondb.net/pokedex/national", extractNames, &count);
	return 0;
}
