#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <iostream>
#include <ctime>
/* For older cURL versions you will also need
#include <curl/types.h>
#include <curl/easy.h>
*/
#include <string>

using namespace std;

//http://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

typedef struct info{
  unsigned char *ptr;
  size_t len;
}t_info;

void init_string(t_info *s) {
  s->len = 0;
  s->ptr = (unsigned char*)malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

//http://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string
size_t write(void *ptr, size_t size, size_t nmemb, t_info *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = (unsigned char*)realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

//Salva o arquivo da url, no arquivo outfilename e retorna o tempo gasto para baixar o arquivo, em milissegundos, -1 caso tenha ocorrido algum erro
long long int init_curl(CURL *curl, char outfilename[], char url[], char errbuf[], FILE *fp, const void* func)
{
    time_t start_t, end_t;
    double diff_t;
    curl = curl_easy_init();
    CURLcode res;
    if (curl) {
        printf("Baixando de %s - para %s...\n", url, outfilename);
        start_t = clock();
        fp = fopen(outfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, true);

        //http://www.linuxquestions.org/questions/programming-9/curl-not-downloading-full-webpage-921586/
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, false);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        //https://curl.haxx.se/libcurl/c/CURLOPT_ERRORBUFFER.html
        errbuf[0] = 0;

        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        end_t = clock();
        diff_t = difftime(end_t, start_t);

        if(strlen(errbuf)){
            printf("%s\n", errbuf);
            return -1;
        }
        fclose(fp);
    }
    else{
        printf("Erro na conexao");
        return -1;
    }
    return (long long int)diff_t;
}

int main(int argv, char *argc[]) {
    CURL *curl;
    FILE *fp;

    char url_reso[] = "http://testemultimidia.netne.net/Agnelo/reso.dat"; //Arquivo contendo as resoluções disponíveis
    char url[] = "http://testemultimidia.netne.net/Agnelo/1080p/"; //pasta com os fragmentos divididos por resolução
    char url_info[] = "http://testemultimidia.netne.net/Agnelo/1080p/info.dat";

    char errbuf[CURL_ERROR_SIZE];
    char outfilename_info[FILENAME_MAX] = "C:\\Users\\Germano\\Desktop\\info.dat"; //trocar pelo diretório desejado

    char outfilename_video[FILENAME_MAX] = "C:\\Users\\Germano\\Desktop\\"; //trocar pelo diretório desejado

    //Baixa o arquivo com as informações
    init_curl(&curl, outfilename_info, url_info, errbuf, fp, (const void*)write_data);

    //Lê a quantidade de fragmentos e o tamanho em bytes de cada um
    fp = fopen(outfilename_info, "r");
    int qtd_fragmentos;
    fscanf(fp,"%d\n", &qtd_fragmentos);
    int fragmentos[qtd_fragmentos];
    for(int i=0; i<qtd_fragmentos; i++){
        fscanf(fp, "%d\n", &fragmentos[i]);
        printf("Fragmento %d - %d\n", (i+1), fragmentos[i]);
    }

    long long int tempo;
    const char nome[] = "res5_";
    const char formato[] = ".mp4";
    char indice[10];
    const int tam_nv = sizeof(nome)+sizeof(formato)+sizeof(indice);
    char nome_video[tam_nv];
    char url_video[FILENAME_MAX];
    char nome_arq[FILENAME_MAX];

    for(int i=0; i<qtd_fragmentos; i++){
        strcpy(nome_video, nome);
        strcpy(url_video, url);
        strcpy(nome_arq, outfilename_video);
        itoa((i+1), indice, 10);
        strcat(nome_video, indice);
        strcat(nome_video, formato);
        strcat(url_video, nome_video);
        tempo = init_curl(&curl, strcat(nome_arq, nome_video), url_video, errbuf, fp, (const void*)write_data);
        printf("TEMPO: %lld milissegundos\n", tempo);
    }

    return 0;
}
