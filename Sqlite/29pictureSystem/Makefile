thread_queue_picture:thread_queue_picture.o queue.o
	gcc $^ -o $@  -lpthread

thread_picture:thread_picture.o
	gcc $^ -o $@  -lpthread

normal_picture:normal_picture.o
	gcc $^ -o $@ 

pfun:pfun.o
	gcc $^ -o $@

.PHONY:clean
clean:
	rm -rf pfun normal_picture thread_picture thread_queue_picture *.o