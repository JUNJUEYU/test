#include "update.h"
#include <curl/curl.h>
#include "log.h"
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
#include "base64.h"
#include <sys/stat.h>
#include <dirent.h>

#define FILE_DIR "/root/gwCore/up/"

static char fileBuffer[10 * 1024 *1024] = {0};

static void Free(void **obj)
{
    if (NULL == *obj)
    {
        return;
    }

    // log_e("Free ptr = %p  ", *obj);
    free(*obj);
    *obj = NULL;
}

static uint8_t emptyDir(char *destDir)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(destDir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", destDir);
        return -1;
    }
    chdir(destDir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISREG(statbuf.st_mode))
        {
            remove(entry->d_name);
        }
    }

    closedir(dp);
    return 0;
}

static int32_t saveUpgradeFile(char *name, char *file, uint32_t len)
{
    char *fileName = NULL;

    emptyDir(FILE_DIR);
    fileName = (char *)malloc(strlen(FILE_DIR) + strlen(name) + 1);
    strcpy(fileName, FILE_DIR);
    strcat(fileName, name);
    FILE *fd = fopen(fileName, "wb");
    fwrite(file, len, 1, fd);
    fclose(fd);

    free(fileName);
}


static int32_t decodeReponse(char *str, char *otaKey)
{
    int32_t ret = -1;
    uint32_t fileLen = 0;

    cJSON *obj = cJSON_Parse(str);
    if (NULL == obj)
    {
        log_e("cJSON_Parse err, obj = %p", obj);
        cJSON_Delete(obj);
        return ret;
    }

    cJSON *result = cJSON_GetObjectItem(obj, "result");
    if (NULL == result)
    {
        log_e("cJSON_GetObjectItem err, result = %p", result);
        cJSON_Delete(obj);
        return ret;
    }
    if (0 != result->valueint)
    {
        log_e("result->valuestring err, result = %d", result->valuestring);
        cJSON_Delete(obj);
        return ret;
    }

    cJSON *data = cJSON_GetObjectItem(obj, "data");
    if (NULL == data)
    {
        log_e("cJSON_GetObjectItem data err, data = %s", result->valuestring);
        cJSON_Delete(obj);
        return ret;
    }

    cJSON *zigbee_id = cJSON_GetObjectItem(data, "zigbee_id");
    cJSON *uu_id = cJSON_GetObjectItem(data, "collect_address");
    cJSON *file_name = cJSON_GetObjectItem(data, "file_name");
    cJSON *base64_data = cJSON_GetObjectItem(data, "base64_data");
    if (NULL == zigbee_id ||
        NULL == uu_id ||
        NULL == file_name ||
        NULL == base64_data)
    {
        cJSON_Delete(obj);
        return ret;
    }

    memset(fileBuffer, 0, sizeof(fileBuffer));
    char *filePtr = strstr(base64_data->valuestring, "base64,");
    if (NULL == filePtr)
    {
        cJSON_Delete(obj);
        return ret;
    }

    filePtr += strlen("base64,");
    int res = base64_decode(filePtr, strlen(filePtr), fileBuffer, (int *)&fileLen);
    if (0 != res)
    {
        log_e("base64_decode err res = %d", res);
        cJSON_Delete(obj);
        return ret;
    }
    log_e("fileLen = %d", fileLen);
    saveUpgradeFile(file_name->valuestring, fileBuffer, fileLen);
}


static char *getResponseBody(char *response)
{
    return strstr(response, "\r\n\r\n") + strlen("\r\n\r\n");
}

static cJSON *newGetFileJsonObj(char *otaKey)
{
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddItemToObject(obj, "ota_key", cJSON_CreateString(otaKey));
    return obj;
}

static size_t replyFunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t memSize = size * nmemb;

    if (NULL == *(char **)stream)
    {
        *(char **)stream = (char *)malloc(memSize + 1);
        memcpy(*(char **)stream, (char *)ptr, memSize);
        (*(char **)stream)[memSize] = '\0';
    }
    else
    {
        size_t len = strlen(*(char **)stream) + memSize + 1;
        *(char **)stream = (char *)realloc(*(char **)stream, len);
        (*(char **)stream)[len - 1] = '\0';
        memcpy(*(char **)stream + strlen(*(char **)stream), (char *)ptr, memSize);
    }

    return size * nmemb;
}


static CURL *httpInit()
{
    return curl_easy_init();
}

static CURLcode httpPost(CURL *curl, const char *url, const char *param, void *response)
{
    CURLcode res;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type:application/json");

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, replyFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

        res = curl_easy_perform(curl);
    }

    curl_easy_cleanup(curl);
    return res;
}


Update::Update(const string &url):
    url_m(url)
{
}


Update::~Update()
{
}

static size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    vector<uint8_t> *data = (vector<uint8_t> *)userdata;
    data->insert(data->end(), ptr, ptr + size * nmemb);
    return size * nmemb;
}

bool Update::run()
{
    CURL *curl = httpInit();
    cJSON *postPara = newGetFileJsonObj("otaKey");
    char *response = NULL;

    CURLcode retCode = httpPost(curl, url_m.c_str(), cJSON_Print(postPara), &response);
    if (CURLE_OK != retCode)
    {
        log_e("httpPost  retCode = %d", retCode);
        cJSON_Delete(postPara);
        return false;
    }

    char *body = getResponseBody(response);
    // if(0 != decodeReponse(body, otaKey, sendFunc))

    
}