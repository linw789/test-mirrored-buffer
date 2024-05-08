#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define CHECK_RETURN(cond) \
    if (cond) {} else { printf("failed at line: %d. errno: %d", __LINE__, errno); exit(EXIT_FAILURE); }

int main(int argc, char** argv)
{
    int phy_mem_fd = memfd_create("test_buffer", 0);
    CHECK_RETURN(phy_mem_fd != -1);

    const uint32_t buffer_size = 4 * 1024;
    int err = ftruncate(phy_mem_fd, buffer_size);
    CHECK_RETURN(err != -1);

    void* whole_buf = 0;
    void* first_half_buf = 0;
    void* second_half_buf = 0;

    const uint32_t virtual_mem_size = buffer_size * 2;

    whole_buf = mmap(0, virtual_mem_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    CHECK_RETURN(whole_buf != 0);
    printf("whole_buf addr: %p\n", whole_buf);

    first_half_buf = mmap(whole_buf, buffer_size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, phy_mem_fd, 0);
    CHECK_RETURN(first_half_buf != 0);
    printf("first_half_buf addr: %p\n", first_half_buf);

    // second_half_buf = mmap(
    //     (uint8_t*)whole_buf + buffer_size, buffer_size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, phy_mem_fd, 0);
    // CHECK_RETURN(second_half_buf != 0);
    // printf("second_half_buf addr: %p\n", second_half_buf);

    close(phy_mem_fd);

    sleep(UINT32_MAX);

    return 0;
}
