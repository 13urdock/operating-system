#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define TIMES 10000000
#define THREADS 5

void* repeat_single(void *param){
	for (int i = 0 ; i < TIMES; i++){
		printf(".");
	}
}

void *repeat_multi(void *param){
	for (int i = 0 ; i < TIMES/THREADS; i++){
		printf(".");
	}
}

struct timeval do_single_thread(){
	struct timeval start, end;
	gettimeofday(&start, NULL);
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, repeat_single, NULL);
	pthread_join(tid, NULL);
	gettimeofday(&end, NULL);

	struct timeval new;
	new.tv_sec = end.tv_sec - start.tv_sec;
	new.tv_usec = end.tv_usec - start.tv_usec;
	return new;
}

struct timeval do_multi_thread(){
	struct timeval start, end;

	gettimeofday(&start, NULL);
	pthread_t tid[THREADS];
	pthread_attr_t attr[THREADS];
	for (int i = 0; i < THREADS; i++){
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i], &attr[i], repeat_multi, NULL);
	}

	for (int i = 0; i < THREADS; i++){
		pthread_join(tid[i], NULL);
	}	
	gettimeofday(&end, NULL);

	struct timeval new;
	new.tv_sec = end.tv_sec - start.tv_sec;
	new.tv_usec = end.tv_usec - start.tv_usec;
	return new;
}

void print_diff(struct timeval single, struct timeval multi){
	double s_duration = single.tv_sec + (double)single.tv_usec/1000000;
	double m_duration = multi.tv_sec + (double)multi.tv_usec/1000000;
	printf("\nprinting %d dots\n", TIMES);
	printf("single_thread:\n - 소요시간: %f\n", s_duration);
	printf("multi_thread:\n -  소요시간: %f\n", m_duration);
	
	printf("시간 비교:\nmulti thread가 single thread대비 %f배 빠르게 수행되었음.\n", s_duration / m_duration);
}

int main(){
	struct timeval single_thread_processing_time = do_single_thread();
	struct timeval multi_thread_processing_time = do_multi_thread();

	print_diff(single_thread_processing_time, multi_thread_processing_time);
	return 0;
}
