#include "sys/ipc.h"
#include "sys/shm.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "fcntl.h"
#include <unistd.h>
#include <cstring>

#define SHM_NAME "Bagnara"
struct SharedData
{
    static const size_t SIZE = 5;  // Static const member
    int array1[SIZE];
    int array2[SIZE];
};


int main(){
    int array1[] = {1,2,3,4,5};
    int array2[] = {1,2,3,4,5};
    size_t SIZE_SHD = sizeof(SharedData);
    key_t key = ftok("/tmp", 65);
    int shm_fd = shmget(key,SIZE_SHD, IPC_CREAT | 0666);

    if(shm_fd == -1)
        throw "error creating shm";

    SharedData* shared_data = (SharedData*) shmat(shm_fd, NULL, 0);
    if (shared_data == (SharedData*)-1)
        throw "error creating shm";
    

    for(int i = 0 ; i < shared_data->SIZE ; i++){
        shared_data->array1[i] = array1[i];
        shared_data->array2[i] = array2[i];
    }

    munmap(shared_data, shared_data->SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    return 0;
}
  