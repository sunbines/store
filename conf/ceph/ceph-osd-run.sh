#!/bin/bash
# Please do not change this file directly since it is managed by Ansible and will be overwritten


########
# MAIN #
########

/usr/bin/docker run \
  --rm \
  --net=host \
  --privileged=true \
  --pid=host \
  --ipc=host \
  --cpu-quota=400000 \
  -v /dev:/dev \
  -v /etc/localtime:/etc/localtime:ro \
  -v /var/lib/ceph:/var/lib/ceph:z \
  -v /etc/ceph:/etc/ceph:z \
  -v /var/run/ceph:/var/run/ceph:z \
  -v /var/run/udev/:/var/run/udev/ \
  -v /var/log/ceph:/var/log/ceph:z \
  -e OSD_DMCRYPT=0 \
  -e CLUSTER=ceph \
  -v /run/lvm/:/run/lvm/ \
  -e CEPH_DAEMON=OSD_CEPH_VOLUME_ACTIVATE \
  -e CONTAINER_IMAGE=ceph/daemon:latest-nautilus \
  -e OSD_ID="$1" \
  --name=ceph-osd-"$1" \
   \
  ceph/daemon:latest-nautilus
  
