#!/bin/sh
sudo virt-install \
    --check path_in_use=off \
    --import \
    -n e205723-fedora-35 \
    --description "Fedora 35" \
    --os-type=Linux \
    --os-variant=fedora31 \
    --ram=2048 \
    --vcpus=2 \
    --disk path=/ie-ryukyu/kvm/images/rental/e205723-os11-1.qcow2,bus=virtio \
    --graphics vnc,listen=0.0.0.0 --noautoconsole \
