#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <list>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
//include perror
#include <stdio.h>
//include exit
#include <stdlib.h>

using namespace std;

//插件管理器
void findAllPlugins(const std::string& path, std::list<std::string>& pluginNames) 
{
    DIR *dir;
    struct dirent *ptr;
    if ((dir = opendir(path.c_str())) == NULL) {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_type == 8) { //file
            std::string fileName = ptr->d_name;
            if (fileName.find(".so") != std::string::npos) {
                pluginNames.push_back(fileName);
            }
        }
    }
    closedir(dir);
}
    

//泛型阻塞消息队列
template<typename T>
class MessageQueue {
public:
    MessageQueue() {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
    }

    ~MessageQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void push(T value) {
        pthread_mutex_lock(&mutex);
        queue.push(value);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    T pop() {
        pthread_mutex_lock(&mutex);
        while (queue.empty()) {
            pthread_cond_wait(&cond, &mutex);
        }
        T value = queue.front();
        queue.pop();
        pthread_mutex_unlock(&mutex);
        return value;
    }

    int size() {
        pthread_mutex_lock(&mutex);
        int size = queue.size();
        pthread_mutex_unlock(&mutex);
        return size;
    }

private:
    std::queue<T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};




int main(int argc, char **argv) {
    MessageQueue<int> queue;
    MessageQueue< std::string > queue2;
    MessageQueue< std::vector<int> > q3;

    vector<int> v1(10, 1);
    q3.push(v1);
    q3.push(vector<int>(10, 2));

    vector<int> v3;
    v3.insert(v3.end(), v1.begin(), v1.end());
    q3.push(v3);


    while(q3.size() > 0) {
        vector<int> v = q3.pop();
        for(int i = 0; i < v.size(); i++) {
            std::cout << v[i];
        }
        std::cout << std::endl;
    }
   
    while(queue2.size() > 0) {
        std::cout << queue2.pop() << std::endl;
    }

    return 0;
}