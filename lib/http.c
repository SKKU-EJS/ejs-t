#include "http.h"

struct cookie session = {.isStore = 0};

size_t storeCookie(char *buffer, size_t size, size_t nitems, void *userdata)
{
	if(!strncmp(buffer,"Set-Cookie",10)){
		int i;
		for(i = 0; buffer[i]!=';'; ++i)
			;
		buffer[i] = '\0';
		sprintf(session.data,"%s",buffer+12);
		session.isStore = 1;
	}
	return nitems * size;
}

int login(char home[], char id[], char pw[])
{
	char url[URLSIZE], payload[URLSIZE];
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;

	memset(url,0,URLSIZE);
	sprintf(url,"%s/auth/login",home);

	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5000L);

		list = curl_slist_append(list, "Accept: */*");
		list = curl_slist_append(list, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		
		sprintf(payload,"{\"studentId\": \"%s\",\"password\":\"%s\"}",id,pw);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,payload);
		
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE,"./tmp/cookie.txt");
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR,"./tmp/cookie.txt");

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, storeCookie);

		curl_easy_perform(curl);

		curl_easy_cleanup(curl);

	}else{
		perror("Error on curl...\n");
	}
	return 0;
}

int logout(char home[])
{
	char url[URLSIZE],cookie[BUFSIZE];
	CURL *curl;
	CURLcode res;
	struct curl_slist *list = NULL;

	memset(url,0,URLSIZE);
	sprintf(url,"%s/auth/logout",home);

	memset(cookie,0,BUFSIZE);
	sprintf(cookie,"Cookie: %s",session.data);

	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

		list = curl_slist_append(list, "Accept: */*");
		list = curl_slist_append(list, "Content-Type: application/json");
		list = curl_slist_append(list, cookie);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		printf("\nurl: %s\ncookie: %s\n",url,session.data);
		curl_easy_perform(curl);

		printf("\nurl: %s\ncookie: %s\n",url,session.data);
		curl_easy_cleanup(curl);
		printf("logout...\n");
	}else{
		perror("Error on curl...\n");
	}
	return 0;
}

int initRepo(char home[])
{
	return 0;
}
