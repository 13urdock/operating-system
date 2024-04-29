// when compile, gcc thread.c -lpthread
// meaning using pthread library
// advantage: less overhead than fork process
// disadvantage: if a thread gets error entire process dies

#include <stdio.h>
#include <pthread.h>

int g_val = 10;

void *worker(void *param){
	g_val++; // g_val in main thread also changes => threads share same memory
	printf("thread g_val = %d\n", g_val);
	pthread_exit(0);
}

void thread_one(){
	printf("one thread\n");
	pthread_t tid; // store pthread id
	pthread_attr_t attr; //pthread의 attribute 지정	
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, worker, NULL); // worker: thread 생성 후 사용할 함수
	pthread_join(tid, NULL); // wait until thread ends
}

void thread_multiple(){
	printf("10 threads\n");
	pthread_t tid[10];
	pthread_attr_t attr[10];

	for (int i = 0; i < 10; i++){
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i], &attr[i], worker, NULL);
	}

	for (int i = 0; i < 10; i++)
		pthread_join(tid[i], NULL);
}

int main(int argc, char *argv[]){
	//gettimeofday() -> for checking time

	pthread_t tid[10];
	pthread_attr_t attr[10];

	for (int i = 0; i < 10; i++){
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i], &attr[i], worker, NULL);
	}

	for (int i = 0; i < 10; i++)
		pthread_join(tid[i], NULL);

	printf("main thread g_val = %d\n", g_val);

	return 0;
}
