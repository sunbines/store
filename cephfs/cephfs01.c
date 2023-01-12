#include <cephfs/libcephfs.h>
#include <stdio.h>
#include <cstddef>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct ceph_mount_info *admin;
    int r = ceph_create(&admin, NULL);
    if (r < 0)
        return r;
    ceph_conf_read_file(admin, NULL);
    ceph_conf_parse_env(admin, NULL);
    ceph_conf_set(admin, "client_permissions", "false");
    r = ceph_mount(admin, "/");
    if (r < 0)
        goto out;
    r = ceph_chmod(admin, "/", 01777);
out:
    ceph_shutdown(admin);
    return r;
}

// g++ test01.cc -lcephfs -D _FILE_OFFSET_BITS=64